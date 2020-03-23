#include <math.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TChain.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TSystem.h>
#include <TEventList.h>
#include <TText.h>
#include <iostream>
#include <stdio.h>

//#include "../GayaStyle.C"
#include "VthMap.h"

#define Column_num 267
#define Row_min 0
#define Row_max 45
#define Diff_VTH 150
void SetGayaStyle ()
{
  /*                                                                                                                                               
  static TStyle* gayaStyle = 0;                                                                                                                    
  //std::cout << "\nApplying ATLAS style settings...\n" << std::endl ;                                                                             
  if ( gayaStyle==0 ) gayaStyle = GayaStyle();                                                                                                     
  gROOT->SetStyle("GAYA");                                                                                                                         
  gROOT->ForceStyle();                                                                                                                             
  */

  gStyle->SetPadTopMargin(0.10); //original 0.16                                                                                                   
  gStyle->SetPadRightMargin(0.12); //original 0.05                                                                                                 
  gStyle->SetPadBottomMargin(0.12); //original 0.16                                                                                                
  gStyle->SetPadLeftMargin(0.12); //original 0.05                                                                                                  

  gStyle->SetTitleXOffset(1.0);
  gStyle->SetTitleYOffset(1.2);
  gStyle->SetTitleOffset(1.2,"z");

  Int_t font=42; // Helvetica                                                                                                                      
  Double_t tsize=0.05;
  gStyle->SetTextFont(font);

  gStyle->SetTextSize(tsize);
  gStyle->SetLabelFont(font,"x");
  gStyle->SetTitleFont(font,"x");
  gStyle->SetLabelFont(font,"y");
  gStyle->SetTitleFont(font,"y");
  gStyle->SetLabelFont(font,"z");
  gStyle->SetTitleFont(font,"z");

  gStyle->SetLabelSize(tsize,"x");
  gStyle->SetTitleSize(tsize,"x");
  gStyle->SetLabelSize(tsize,"y");
  gStyle->SetTitleSize(tsize,"y");
  gStyle->SetLabelSize(tsize,"z");
  gStyle->SetTitleSize(tsize,"z");

  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);
  gStyle->SetHistLineWidth(1.); //original:2. gaya changed(2020/01/20)                                                                             
  gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes                                                                                    

  gStyle->SetNdivisions(50505);

  //gStyle->SetStripDecimals(false);                                                                                                               

  gStyle->SetOptStat("eMinoru");
  gStyle->SetStatX(0.97);
  gStyle->SetStatY(0.7);
  gStyle->SetStatH(0.2);
  gStyle->SetStatW(0.25);

  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
}
makeVthMap::makeVthMap(std::string dir, TChain*tr, int col, int row){
  target_dir=dir;
  tree=tr;
  target_column=col;
  target_row=row;

  tree->Draw(">>elist",Form("Column==%d&&Row==%d",target_column,target_row));
  elist=(TEventList*)gROOT->FindObject("elist");
  total_entry=elist->GetN();

  this->init();
}
void makeVthMap::init(){
  Threshold=0;
  Noise=0;
  Column=0;
  Row=0;
  DiffPrecomp=0;
  DiffVth1=0;
  DiffVth2=0;
  TDAC=0;
  this->SetBranch();
  SetGayaStyle();
  gStyle->SetOptStat(0);


  int Precomp=0;
  for(int i=0;i<(DiffPrecomp_max-DiffPrecomp_min)/DiffPrecomp_step;i++){
    Precomp=DiffPrecomp_min+i*DiffPrecomp_step;
    hist[i]=new TH2F(Form("hist_%d_%d_%d",target_column,target_row,Precomp),Form("Threshold_Map;VTH1 [Register Value];VTH2 [Register Value]"),(DiffVth1_max-DiffVth1_min)/DiffVth1_step,DiffVth1_min,DiffVth1_max,(DiffVth2_max-DiffVth2_min)/DiffVth2_step,DiffVth2_min,DiffVth2_max);
    //    std::cout << DiffVth1_step << std::endl;
    hist[i]->SetMinimum(0);
    hist[i]->SetMaximum(4000);
    hist[i]->GetYaxis()->SetNdivisions(50505);
    hist[i]->GetZaxis()->SetTitle("Threshold [e]");
      //hist[i]->SetTitleOffset(1.0,"x");
      //hist[i]->SetTitleSize(0.045,"x");
      //hist[i]->SetTitleOffset(1.0,"y");
      //hist[i]->SetTitleSize(0.045,"y");
  }
}
void makeVthMap::SetBranch(){
  tree->SetBranchAddress("RunID",&RunID,&b_RunID);
  tree->SetBranchAddress("Column", &Column, &b_Column);
  tree->SetBranchAddress("Row", &Row, &b_Row);
  tree->SetBranchAddress("Threshold", &Threshold, &b_Threshold);
  tree->SetBranchAddress("Noise", &Noise, &b_Noise);
  tree->SetBranchAddress("DiffVth1", &DiffVth1, &b_DiffVth1);
  tree->SetBranchAddress("DiffVth2", &DiffVth2, &b_DiffVth2);
  tree->SetBranchAddress("DiffPrecomp", &DiffPrecomp, &b_DiffPrecomp);
  tree->SetBranchAddress("TDAC", &TDAC, &b_TDAC);

}
void makeVthMap::Loop(){
  int Precomp=0;
  for(int iEntry=0;iEntry<total_entry;iEntry++){
    tree->GetEntry(elist->GetEntry(iEntry));
    for(int i_prec=0;i_prec<(DiffPrecomp_max-DiffPrecomp_min)/DiffPrecomp_step;i_prec++){
      Precomp=DiffPrecomp_min+i_prec*DiffPrecomp_step;
      if(DiffPrecomp==Precomp){
	hist[i_prec]->Fill(DiffVth1,DiffVth2,Threshold);
      }
      if(DiffVth1-DiffVth2==Diff_VTH&&Column==Column_num&&Row>=Row_min&&Row<=Row_max){
	if(Threshold>0){
	  graph_x.push_back(DiffVth1);
	  graph_y.push_back(Threshold);
	}
      }
    }
  }
}
void makeVthMap::Draw(){
  TString pdf_file=target_dir+Form("/VTH_map_%d_%d.pdf",target_column,target_row);
  TString pdf_file_open=pdf_file+"[";
  TString pdf_file_close=pdf_file+"]";
  TString text=Form("(Column, Row)=(%d, %d)",target_column,target_row);

  TText* text_box=new TText(0.135,0.83,text);
  text_box->SetNDC(1);
  text_box->SetTextFont(62);

  TCanvas*canvas=new TCanvas("canvas","",700,500);
  canvas->Print(pdf_file_open);
  canvas->SetRightMargin(0.17);
  for(int i_prec=0;i_prec<(DiffPrecomp_max-DiffPrecomp_min)/DiffPrecomp_step;i_prec++){
    if (hist[i_prec]->GetEntries()>0){
      hist[i_prec]->Draw("colz");
      text_box->Draw();
      canvas->Print(pdf_file);
    }
  }
  canvas->Print(pdf_file_close);
}

TGraph* makeVthMap::GetTGraph(){
  int NEntry=graph_x.size();
  graph=new TGraph(NEntry);
  for(int i=0; i<NEntry; i++){
    graph->SetPoint(i,graph_x[i],graph_y[i]);
  }
  return graph;
}

int VthMap(std::string dir){
  int i_column=Column_num;
  int i_row=12;

  TString filename=dir+"/analysis.root";
  FileStat_t info;

  if (gSystem->GetPathInfo(filename.Data(), info)!=0) {
    printf("File '%s' does not exist.\n", filename.Data());
    return 1;
  }
  TFile*file=new TFile(filename);
  TChain*tree=(TChain*)file->Get("tree");

  int i = 0;
  makeVthMap* pixel[(Row_max-Row_min)/5];
  int i_pixel=0;
  bool Print;
  TText* text=new TText(0.15,0.82,Form("VTH1-VTH2=%d",Diff_VTH));
  text->SetNDC(1);
  TLegend *legend=new TLegend(0.15,0.50,0.45,0.80,"pixel coordinate (Column,Row)");
  
  std::vector <TGraph*> graph_vec;
  std::vector <TString> text_for_legend;
  //for(int i_row=Row_min;i_row<Row_max;i_row++){
  for(int i_row=0;i_row<(Row_max-Row_min)/5;i_row++){
    
    pixel[i]=new makeVthMap(dir,tree,i_column,i_row*5+3);
    pixel[i]->Loop();

    pixel[i]->Draw();
    graph_vec.push_back(pixel[i]->GetTGraph());
    text_for_legend.push_back(Form("(%d, %d)",i_column,i_row*5+3));
    i++;
  }
  TString option="";
  gPad->SetRightMargin(0.12);
  for(unsigned i_g=0;i_g<graph_vec.size();i_g++){
    if(i_g==0) option="APL";
    else option="PL";
    graph_vec[i_g]->SetTitle("Threshold_vs_VTH1;VTH1 [Register Value];Threshold [e]");
    graph_vec[i_g]->SetMinimum(10);
    graph_vec[i_g]->SetMaximum(4000);
    graph_vec[i_g]->SetLineColor(i_g+1);
    graph_vec[i_g]->SetMarkerColor(i_g+1);
    graph_vec[i_g]->GetXaxis()->SetLimits(100,600);
    graph_vec[i_g]->Draw(option);

    legend->AddEntry(graph_vec[i_g],text_for_legend[i_g],"lp");
  }
  double legend_tsize=0.04;
  gStyle->SetLegendTextSize(legend_tsize);

  legend->SetBorderSize(0);
  legend->SetNColumns(2);
  legend->Draw("same");
  text->Draw();

  TString graph_out=dir+"/Threshold_vs_VTH1_dVTh.pdf";
  gPad->Print(graph_out);
  return 0;
}
