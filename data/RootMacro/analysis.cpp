#include <stdio.h>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TChain.h>
#include <TEventList.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TCut.h>
#include <TProfile.h>
#include <TStyle.h>
#include <vector>
#include <TGraphErrors.h>
#include <TString.h>
#include <TSystem.h>
//#include "GayaClass.h"

void make_threshold_vs_par(TChain*tree,std::string target_DIR,std::string parName,int bin,int min,int max,TCut cut ,double* p0, double* p1,double* p0E, double* p1E){
  TH2D*hist=new TH2D("hist",Form("Threshold_vs_%s_%s;%s;Threshold",parName.c_str(),cut.GetTitle(),parName.c_str()),bin,min,max,50,-100,2400);
  TCanvas*canvas=new TCanvas("canvas","");
  tree->Draw(Form("Threshold:%s>>hist",parName.c_str()),cut,"colz");
  canvas->Print(Form("%s/Threshold_vs_%s.pdf",target_DIR.c_str(),parName.c_str()));

  TH2D*hist_p=new TH2D("hist_p",Form("Threshold_vs_%s_%s;%s;Threshold",parName.c_str(),cut.GetTitle(),parName.c_str()),bin,min,max,50,1,2401);

  tree->Draw(Form("Threshold:%s>>hist_p",parName.c_str()),cut,"colz");

  TProfile*hp=hist_p->ProfileX();
  hp->SetMinimum(0);
  hp->SetMaximum(2000);
  hp->Draw();

  TF1*f=new TF1("f","pol1",0,300);
  int fitrange[2];
  fitrange[0]=0;
  fitrange[1]=0;
  if(parName=="TDAC"){
    fitrange[0]=-5;
    fitrange[1]=8;
  }
  else{
    fitrange[0]=0;
    fitrange[1]=300;
  }

  hp->Fit("f","","",fitrange[0],fitrange[1]);
  std::cout << "gaya" << std::endl;

  *p0=f->GetParameter(0);
  *p1=f->GetParameter(1);

  std::cout << *p0 << "   " << *p1 << std::endl;

  canvas->Print(Form("%s/Threshold_vs_%s_profile.pdf",target_DIR.c_str(),parName.c_str()));

  delete f;
  delete hist;
  delete hist_p;
  delete hp;
  delete canvas;
}

void make_thresholdMap(TChain*tree,int target_RunID,const char *pdf,TH2D*hist_th,TH2D*hist_no,TH1D*hist1d_th,TH1D*hist1d_no){
      
  tree->Draw(">>elist",Form("RunID==%d",target_RunID));
  TEventList *elist=(TEventList*)gROOT->FindObject("elist");
  int N=elist->GetN();

  int RunID=0;
  int DiffVth1=0;
  int DiffVth2=0;
  int Column=0;
  int Row=0;
  double Threshold=0;
  double Noise=0;
  tree->SetBranchAddress("RunID",&RunID);
  tree->SetBranchAddress("DiffVth1",&DiffVth1);
  tree->SetBranchAddress("DiffVth2",&DiffVth2);
  tree->SetBranchAddress("Column",&Column);
  tree->SetBranchAddress("Row",&Row);
  tree->SetBranchAddress("Threshold",&Threshold);
  tree->SetBranchAddress("Noise",&Noise);

  for(int iEntry=0;iEntry<N;iEntry++){
    tree->GetEntry(elist->GetEntry(iEntry));
    //    std::cout << "Threshold = " << Threshold << std::endl;
    hist_th->Fill(Column,Row,Threshold);
    hist_no->Fill(Column,Row,Noise);
    if (Column>263){
      hist1d_th->Fill(Threshold);
      hist1d_no->Fill(Noise);
    }
  }
  delete elist;
}


int analysis(std::string DIR_name, int file_num){
  gStyle->SetOptStat("eMinoru");
  gStyle->SetStatX(1.0);
  gStyle->SetStatY(0.9);
  gStyle->SetStatH(0.12);
  gStyle->SetStatW(0.14);


  //std::string TMP_DIR="/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/";
  std::string TMP_DIR="./";

  //  std::string target_DIR=TMP_DIR+"changing_VTH1_0603";
  std::string target_DIR=TMP_DIR+DIR_name;
  std::string analysis_file=target_DIR+"/analysis.root";
  std::string Mappdf=target_DIR+"/Mapplot.pdf";
  std::string Distpdf=target_DIR+"/Distplot.pdf";

  TFile*file=new TFile(analysis_file.c_str(),"recreate");
  TChain*tree=new TChain("tree","");
  TString ifn;
  FileStat_t info;
  
  int i=0;
  int f_count=0;
  while(i<file_num){
    ifn = Form("%s/gaya%d.root",target_DIR.c_str(),f_count);
    //    std::cout << gSystem->GetPathInfo(ifn.Data(), info) << std::endl;
    if (gSystem->GetPathInfo(ifn.Data(), info)!=0) {
      f_count++;
      continue;
    }
    else{
    //    std::cout << Form("%sgaya%d.root",target_DIR.c_str(),i) << std::endl;
      tree->Add(Form("%s/gaya%d.root",target_DIR.c_str(),f_count));
      f_count++;
      i++;
    }
    std::cout << i << std::endl;
  }
  tree->Write();

  /*    
  TCanvas*c=new TCanvas("c","");
  c->Print(Form("%s[",Mappdf.c_str()));
  c->Print(Form("%s[",Distpdf.c_str()));
  for(int i=0;i<file_num;i++){
    TH2D*hist_th=new TH2D("hist_th",Form("Threshold_RunID=%d;Column;Row",i),400,0,400,192,0,192);
    TH2D*hist_no=new TH2D("hist_no",Form("Noise_RunID=%d;Column;Row",i),400,0,400,192,0,192);

    TH1D*hist1d_th=new TH1D("hist1d_th",Form("Threshold_RunID=%d;Threshold; #pixel",i),100,0,3000);
    TH1D*hist1d_no=new TH1D("hist1d_no",Form("Noise_RunID=%d;Noise; #pixel",i),100,0,500);
    
    make_thresholdMap(tree,i,Mappdf.c_str(),hist_th,hist_no,hist1d_th,hist1d_no);
    
    hist_th->Draw("colz");
    c->Print(Mappdf.c_str());
    hist_no->Draw("colz");
    c->Print(Mappdf.c_str());

    hist1d_th->Draw("colz");
    c->Print(Distpdf.c_str());
    hist1d_no->Draw("colz");
    c->Print(Distpdf.c_str());

    delete hist_th;
    delete hist_no;
    delete hist1d_th;
    delete hist1d_no;
  }
  c->Print(Form("%s]",Mappdf.c_str()));
  c->Print(Form("%s]",Distpdf.c_str()));
  
  std::cout << "gaya" << std::endl;
  */
  
  /* ----------create DiffVth1 Graph-------------------------------------------------
  TCanvas*c1=new TCanvas("c1","");
  TCut DiffFE = "Column>263";
  //  int par=1000;
  std::string parName="DiffVth1";
  int par_bin=0;
  int par_min=0;
  int par_max=0;

  c1->Print(Form("%s/Threshold_vs_%s.pdf[",target_DIR.c_str(),parName.c_str()));
  c1->Print(Form("%s/Threshold_vs_%s_profile.pdf[",target_DIR.c_str(),parName.c_str()));

  double p0=0;
  double p1=0;
  double p0E=0;
  double p1E=0;

  std::cout << "gaya" << std::endl;

  std::vector<double> p0_vec;
  std::vector<double> p1_vec;
  std::vector<double> p0E_vec;
  std::vector<double> p1E_vec;
  std::vector<int> comp_vec;

  if(parName=="TDAC"){
    par_bin=40;
    par_min=-20;
    par_max=20;
  }
  else if(parName=="DiffVth1"){
    par_bin=50;
    par_min=25;
    par_max=275;
  }

  for(int i_precomp=0;i_precomp<1024;i_precomp=i_precomp+50){
    if(tree->GetEntries(Form("DiffPrecomp==%d",i_precomp))>1000){
      TCut cut_Precomp=Form("DiffPrecomp==%d",i_precomp);
      TCut cut = DiffFE&&cut_Precomp;
      make_threshold_vs_par(tree,target_DIR.c_str(),parName.c_str(),par_bin,par_min,par_max,cut,&p0,&p1,&p0E,&p1E);
      std::cout << p0 << " , " << p1 << std::endl;
      if(p0==0&&p1==0){
	p0_vec.push_back(p0);
	p1_vec.push_back(p1);
	p0E_vec.push_back(p0E);
	p1E_vec.push_back(p1E);
	comp_vec.push_back(i_precomp);
      }
    }
    else{
      std::cout << "DiffPrecomp = " << i_precomp << std::endl;
      continue;
    }
  }

  TGraphErrors*g_p0=new TGraphErrors(p0_vec.size());
  TGraphErrors*g_p1=new TGraphErrors(p1_vec.size());

  g_p0->SetTitle(Form("Precomp_effect_for_baseline;DiffPrecomp;d(threshold)/d(%s)",parName.c_str()));
  g_p1->SetTitle(Form("Precomp_effect_for_slope;DiffPrecomp;d(threshold)/d(%s)",parName.c_str()));
  g_p0->SetMarkerStyle(8);
  g_p0->SetMarkerSize(1);
  g_p1->SetMarkerStyle(8);
  g_p1->SetMarkerSize(1);

  for(unsigned int i=0;i<p0_vec.size();i++){
    g_p0->SetPoint(i,comp_vec[i],p0_vec[i]);
    g_p1->SetPoint(i,comp_vec[i],p1_vec[i]);
    g_p0->SetPointError(i,0,p0E_vec[i]);
    g_p1->SetPointError(i,0,p1E_vec[i]);
  }
  g_p0->Draw("AP");
  c1->Print(Form("%s/Precomp_effect.pdf(",target_DIR.c_str()));
  g_p1->Draw("AP");
  c1->Print(Form("%s/Precomp_effect.pdf)",target_DIR.c_str()));

  c1->Print(Form("%s/Threshold_vs_%s.pdf]",target_DIR.c_str(),parName.c_str()));
  c1->Print(Form("%s/Threshold_vs_%s_profile.pdf]",target_DIR.c_str(),parName.c_str()));
  //--------------------------------------------------------------------------------

  
  GayaClass*g=new GayaClass(tree);
  g->Loop(0);
  g->hist_th->Draw();
  c->Print(Form("%s",Mappdf.c_str()));
  g->hist_no->Draw();
  c->Print(Form("%s",Mappdf.c_str()));

  */

  return 0;
}
