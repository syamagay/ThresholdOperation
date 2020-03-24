#define tune_evaluation_cxx
#include "tune_evaluation_via.h"
#include "Plotting_single.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <TGaxis.h>
#include <TSystem.h>

#define graph_upper 2020

int tune_evaluation::PixelSelection(int col, int row){
  //std::string target_pixel="good";

  int bool_pixel=10;
  int diff_start=264;
  std::array<std::array<unsigned, 8>, 8> mask;
  mask[0] = {{0, 0, 1, 1, 0, 0, 0, 0}};
  mask[1] = {{0, 0, 0, 1, 0, 0, 0, 0}};
  mask[2] = {{0, 0, 0, 1, 0, 0, 0, 0}};
  mask[3] = {{0, 0, 0, 1, 1, 0, 0, 0}};
  mask[4] = {{0, 0, 0, 1, 1, 0, 0, 0}};
  mask[5] = {{0, 0, 0, 1, 1, 0, 0, 0}};
  mask[6] = {{0, 0, 0, 1, 0, 0, 0, 0}};
  mask[7] = {{0, 0, 0, 0, 1, 0, 0, 0}};
  if(col<diff_start){
    bool_pixel=0;
  }
  else{
    if(target_pixel=="all"){
      bool_pixel=1;
    }
    else if(target_pixel=="good"){
      if(mask[row%8][col%8]==1) bool_pixel=1;
      else bool_pixel=0;
    }
  }
  return bool_pixel;
}

void tune_evaluation::Loop()
{
  /*
  Int_t fontid=132; //論文用 132、スライド用 62
  //Int_t italic=12; //イタリック用フォント 通常12 太文字32
  
  gStyle->SetStatFont(fontid); 
  gStyle->SetLabelFont(fontid,"XYZ"); 
  gStyle->SetLabelFont(fontid,""); 
  gStyle->SetTitleFont(fontid,"XYZ"); 
  gStyle->SetTitleFont(fontid,""); 
  gStyle->SetTextFont(fontid); 
  gStyle->SetLegendFont(fontid); 
  */
//   In a ROOT session, you can do:
//      root> .L tune_evaluation.C
//      root> tune_evaluation t
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


   LowerLimitDist* LLD=new LowerLimitDist(1);
   LLD->SetOutputDir(output_dir);
   LLD->add_histogram();
   LLD->pdf_open();
   LLD->SetTitle("LowerLimit (from target);target_threshold;# pixels");

   ThresholdDist* TD=new ThresholdDist();
   TD->SetOutputDir(output_dir);
   TD->add_histogram();
   TD->pdf_open();


   std::vector <Plotting_single*> Plottings;
   Plottings.push_back(new TDACDist());
   Plottings.push_back(new TDAC2Dist());
   Plottings.push_back(new NoiseDist());
   Plottings.push_back(new Threshold2Dist());
   Plottings.push_back(new Threshold_viaDist());
   Plottings.push_back(new Noise_viaDist());
   Plottings.push_back(new Noise_via2Dist());
   Plottings.push_back(new LowerLimitDist());
   Plottings.push_back(new Threshold_via2Dist());
   Plottings.push_back(new TDAC_viaDist());
   Plottings.push_back(new DiffTDACDist());
   Plottings.push_back(new DiffThresholdDist());
   Plottings.push_back(new DiffNoiseDist());
   Plottings.push_back(new DiffNoise2Dist());

   double Threshold_vec[400][192]={{0}};
   double Noise_vec[400][192]={{0}};
   double TDAC_vec[400][192]={{0}};
   double Enable_vec[400][192]={{0}};
   double Threshold_via_vec[400][192]={{0}};
   double Noise_via_vec[400][192]={{0}};
   double TDAC_via_vec[400][192]={{0}};

   double threshold_mean=0;

   const int hist2d_num=3;
   // Threshold, TDAC, Threshold_vs_d_Vth,
   TH2F* hist2d[hist2d_num];
   TCanvas*h2d_c[hist2d_num];
   TString hist2d_title[hist2d_num]={"Threshold_vs_target;target_threshold[e];Threshold[e]",
				     "TDAC_vs_target;target_threshold[e];TDAC[Register Value]",
				     "Threshold_vs_#DeltaVTH;#DeltaVTH;Threshold[e]"};
   Int_t hist2d_min[hist2d_num];
   Int_t hist2d_max[hist2d_num];
   Int_t hist2d_bin[hist2d_num];
   Int_t hist2d_ymin[hist2d_num];
   Int_t hist2d_ymax[hist2d_num];
   Int_t hist2d_ybin[hist2d_num];

   for(Int_t i_hist2d;i_hist2d<hist2d_num;i_hist2d++){
     hist2d_min[i_hist2d]=0;
     hist2d_max[i_hist2d]=0;
     hist2d_bin[i_hist2d]=0;
     hist2d_ymin[i_hist2d]=0;
     hist2d_ymax[i_hist2d]=0;
     hist2d_ybin[i_hist2d]=0;
   }
   hist2d_min[2]=-50; hist2d_max[2]=500; hist2d_bin[2]=55;// X-axis is VTH1-VTH2
   hist2d_ymin[1]=-15; hist2d_ymax[1]=16; hist2d_ybin[1]=31; // Y-axis is TDAC

   double hist2d_maximum=0;
   if(target_pixel=="all"){
     //hist2d_maximum=10000;
     hist2d_maximum=10;
   }
   else{
     //hist2d_maximum=3000;
     //hist2d_maximum=2;
     hist2d_maximum=2;
   }
   
   for(Int_t i_hist2d=0;i_hist2d<hist2d_num;i_hist2d++){
     //X-axis : target_Threshold Status
     if(hist2d_min[i_hist2d]==0){
       hist2d_min[i_hist2d]=-10;
     }
     if(hist2d_max[i_hist2d]==0){
       hist2d_max[i_hist2d]=graph_upper;
     }
     
     if(hist2d_bin[i_hist2d]==0){
       hist2d_bin[i_hist2d]=101;
     }
     //Y-Axis : threshold
     if(hist2d_ymin[i_hist2d]==0){
       hist2d_ymin[i_hist2d]=0;
     }
     if(hist2d_ymax[i_hist2d]==0){
       hist2d_ymax[i_hist2d]=3000;
     }
     
     if(hist2d_ybin[i_hist2d]==0){
       hist2d_ybin[i_hist2d]=100;
     }
     hist2d[i_hist2d] = new TH2F(Form("hist_2d_%d",i_hist2d),hist2d_title[i_hist2d],hist2d_bin[i_hist2d],hist2d_min[i_hist2d],hist2d_max[i_hist2d],hist2d_ybin[i_hist2d],hist2d_ymin[i_hist2d],hist2d_ymax[i_hist2d]);
     /*
     hist2d[i_hist2d]->SetTitleSize(0.045,"x");
     hist2d[i_hist2d]->SetTitleSize(0.045,"y");
     hist2d[i_hist2d]->SetTitleSize(0.045,"z");
     hist2d[i_hist2d]->GetXaxis()->SetTitleOffset(0.85);
     hist2d[i_hist2d]->GetYaxis()->SetTitleOffset(0.95);
     */
     hist2d[i_hist2d]->GetZaxis()->SetTitle("# pixels[#times 1000]");
     //hist2d[i_hist2d]->GetZaxis()->SetTitleOffset(0.95);
     h2d_c[i_hist2d]=new TCanvas(Form("2d_c_%d",i_hist2d),"");
     //h2d_c[i_hist2d]->SetRightMargin(0.15);
   }
   Long64_t nentries = fChain->GetEntriesFast();

   for(unsigned it=0;it<Plottings.size();it++){
     Plottings[it]->SetOutputDir(output_dir);
     Plottings[it]->add_histogram();
     Plottings[it]->pdf_open();
   }

   Long64_t nbytes = 0, nb = 0;
   unsigned totalpixel=0;
   int bool_pixel=10;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     // if (Cut(ientry) < 0) continue;
     if(Column==0&&Row==0){
       totalpixel=0;
       TD->init(target_threshold);
       LLD->init(target_threshold);
       for(unsigned it=0;it<Plottings.size();it++){
	 Plottings[it]->init(target_threshold);
       }
     }
     // pixel selection
     
     //if(Column>263){
     //if( Column>263 && ( (Column%8==2&&Row%8==0) || (Column%8==3&&Row%8<7) || (Column%8==4&&(Row%8>3&&Row%8<6)) || (Column%8==4&&(Row%8==7)) ) ){ //GoodPixel
     bool_pixel=PixelSelection(Column,Row);
     //if(Column>263 && (Column%8==3&&Row%8<7)){
     
     //if(Column>263 && (Column%8==0&&Row%8==5)){
     if(bool_pixel==1){
       //   std::cout << ientry << " Enable = " << Enable<< " target = " << target_threshold<< std::endl;
       hist2d[0]->Fill(target_threshold,Threshold,1);
       hist2d[1]->Fill(target_threshold,TDAC,1);
       hist2d[2]->Fill(DiffVth1-DiffVth2,Threshold,1);
	 
       TD->sort_value(Column,Row,target_threshold,Threshold,Noise,TDAC,Enable,Threshold_via,Noise_via,DiffVth1);
       LLD->sort_value(Column,Row,target_threshold,Threshold,Noise,TDAC,Enable,Threshold_via,Noise_via,TDAC_via);

       //       for(unsigned it=0;it<Plottings.size();it++){
       //	 Plottings[it]->sort_value(Column,Row,target_threshold,Threshold,Noise,TDAC,Enable,Threshold_via,Noise_via,TDAC_via);
       //}
	 
       Threshold_vec[Column][Row]=Threshold;
       Noise_vec[Column][Row]=Noise;
       TDAC_vec[Column][Row]=TDAC;
       Enable_vec[Column][Row]=Enable;
       Threshold_via_vec[Column][Row]=Threshold_via;
       Noise_via_vec[Column][Row]=Noise_via;
       TDAC_via_vec[Column][Row]=TDAC_via;
	 
       totalpixel++;
     }
     else{
       Threshold_vec[Column][Row]=-1;
     }
     if(Column==399&&Row==191){
       TD->Draw();
       threshold_mean=TD->get_threshold_mean();
       //       std::cout << target_threshold << std::endl;
       for(int i_col=264;i_col<400;i_col++){
	 for(int i_row=0;i_row<192;i_row++){
	   if(Threshold_vec[i_col][i_row]!=-1){
	     for(unsigned it=0;it<Plottings.size();it++){
	       Plottings[it]->sort_value(i_col,i_row,threshold_mean,Threshold_vec[i_col][i_row],Noise_vec[i_col][i_row],TDAC_vec[i_col][i_row],Enable_vec[i_col][i_row],Threshold_via_vec[i_col][i_row],Noise_via_vec[i_col][i_row],TDAC_via_vec[i_col][i_row]);
	     }
	     /*
	     Threshold_vec[i_col][i_row]=0;	     
	     Noise_vec[i_col][i_row]=0;
	     TDAC_vec[i_col][i_row]=0;	     
	     Enable_vec[i_col][i_row]=0;
	     Threshold_via_vec[i_col][i_row]=0;
	     Noise_via_vec[i_col][i_row]=0;
	     TDAC_via_vec[i_col][i_row]=0;
	     */
	   }
	 }
       }
       //std::cout << TD->get_threshold_mean() << std::endl;
       for(unsigned it=0;it<Plottings.size();it++){
	 //std::cout << target_threshold << std::endl;
	 Plottings[it]->Draw();
       }
     }
   }
   std::string print_name=output_dir+"tune_evaluation.pdf[";
   gPad->Print(print_name.c_str());
   for(unsigned it=0;it<Plottings.size();it++){
     Plottings[it]->resultDraw();
     Plottings[it]->pdf_close();
   }
   LLD->resultDraw();
   LLD->pdf_close();
   TD->resultDraw();
   TD->pdf_close();


   for(Int_t i_hist2d=0;i_hist2d<hist2d_num;i_hist2d++){
     h2d_c[i_hist2d]->cd();
     if(i_hist2d==1) h2d_c[i_hist2d]->SetLogz();
     else h2d_c[i_hist2d]->SetLogz(0);
     h2d_c[i_hist2d]->SetRightMargin(0.14);
     hist2d[i_hist2d]->Scale(1.0/1000.0);
     hist2d[i_hist2d]->SetMaximum(hist2d_maximum);
     hist2d[i_hist2d]->GetXaxis()->SetNdivisions(510);
     //((TGaxis*)hist2d[i_hist2d]->GetXaxis())->SetMaxDigits(5);
     //((TGaxis*)hist2d[i_hist2d]->GetYaxis())->SetMaxDigits(5);
     //((TGaxis*)hist2d[i_hist2d]->GetZaxis())->SetMaxDigits(4);
     hist2d[i_hist2d]->Draw("colz");
     //sleep(3);                                                                                                          
     print_name=output_dir+"tune_evaluation.pdf";
     h2d_c[i_hist2d]->Print(print_name.c_str());
   }
   print_name=output_dir+"tune_evaluation.pdf]";
   gPad->Print(print_name.c_str());


}

void run(TTree* tree, std::string target_pixel, std::string output_dir="picture/"){
  tune_evaluation * te = new tune_evaluation(tree, target_pixel, output_dir);
  te->Loop();
}

//void run(){
//  tune_evaluation * te = new tune_evaluation(tree);
//  te->Loop();
//}
std::string dirname(std::string path) {
  return path.substr(0, path.find_last_of('/'))+"/";
}


int main(int argc,char* argv[]){
  std::string target_pixel;
  if(argc>=2){
    if(argc==2){
      std::string answer;
      std::cout << "target pixel [all/good] is not selected" << std::endl;
      std::cout << "Can I start analysis with all pixels? [y/N]" << std::endl;
      std::cin >> answer;
      if(answer=="y"){
	target_pixel="all";
      }
      else return 0;
    }
    else if(argc==3){
      target_pixel=argv[2];
      if(target_pixel!="all"&&target_pixel!="good"){
	std::cout << "target_pixel should be all/good." << std::endl;
	return 0;
      }
    }
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
    run(tree, target_pixel, output_dir);
  }
  else{
    return 0;
  }
  return 0;
}
