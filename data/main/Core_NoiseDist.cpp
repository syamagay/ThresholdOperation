#define Core_NoiseDist_cxx
#include "Core_NoiseDist.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "GayaStyle.h"

#define core_col 8
#define core_row 8

void Core_NoiseDist::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Core_NoiseDist.C
//      root> Core_NoiseDist t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;
   SetGayaStyle();
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   TCanvas*canvas=new TCanvas("canvas","");

   TH1F* core_noisehist[core_col][core_row];
   for(int i=0;i<core_col;i++){
     for(int j=0;j<core_row;j++){
       core_noisehist[i][j]=new TH1F(Form("Noise_%d_%d",i,j),"",100,0.5,200.5);
     }
   }
   TH2F* CoreNoise2D=new TH2F("CoreNoiseDist","",core_col,0,core_col,core_row,0,core_row);
   TH2F* CoreNoiseDev2D=new TH2F("CoreNoiseDevDist","",core_col,0,core_col,core_row,0,core_row);
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if(jentry==0){
	CoreNoise2D->SetTitle(Form("CoreNoiseDist_%d;Column;Row",target_threshold));
	CoreNoiseDev2D->SetTitle(Form("CoreNoiseDevDist_%d;Column;Row",target_threshold));
      }
      core_noisehist[Column%core_col][Row%core_row]->Fill(Noise);
   }
   TString each_pdf=output_dir+"NoiseDist1D.pdf";
   TString each_pdf_open=each_pdf+"[";
   canvas->Print(each_pdf_open);
   for(int i=0;i<core_col;i++){
     for(int j=0;j<core_row;j++){
       CoreNoise2D->Fill(i,j,core_noisehist[i][j]->GetMean());
       CoreNoiseDev2D->Fill(i,j,core_noisehist[i][j]->GetStdDev());
       core_noisehist[i][j]->Draw();
       canvas->Print(each_pdf);
     }
   }
   TString each_pdf_close=each_pdf+"]";
   canvas->Print(each_pdf_close);

   TString pdf_open=output_dir+"/NoiseDist2D.pdf[";
   canvas->Print(pdf_open);

   gStyle->SetOptStat(0);
   CoreNoise2D->Draw("text colz");
   TString pdf=output_dir+"/NoiseDist2D.pdf";
   canvas->Print(pdf);
   CoreNoiseDev2D->Draw("text colz");
   canvas->Print(pdf);

   TString pdf_close=output_dir+"/NoiseDist2D.pdf]";
   canvas->Print(pdf_close);
}
std::string dirname(std::string path) {
  return path.substr(0, path.find_last_of('/'))+"/";
}

void run(TTree*tree, std::string output_dir){
  Core_NoiseDist * cnd = new Core_NoiseDist(tree, output_dir);
  cnd->Loop();  
}

int main(int argc, char* argv[]){
  TString rootfile=argv[1];
  std::string arg_rootfile=argv[1];
  FileStat_t info;
  if(arg_rootfile.find(".root")==std::string::npos){
    std::cout << arg_rootfile <<" don't include \".root\"" << std::endl;
    return 1;
  }
  else if (gSystem->GetPathInfo(rootfile.Data(), info)!=0) {
    printf("File '%s' does not exist.\n", rootfile.Data());
    return 1;
  }

  std::string output_dir=dirname(arg_rootfile);

  TFile* file=new TFile(rootfile);
  TTree* tree=(TTree*)file->Get("tree");
  run(tree, output_dir);
}
