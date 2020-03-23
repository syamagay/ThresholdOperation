#include <math.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TChain.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TEventList.h>
#include <TSystem.h>

#include <iostream>
#include <stdio.h>

#include "pixelGraph.h"
#include "GayaStyle.C"

#define Column_num 264
#define Row_min 10
#define Row_max 16

PixelGraph::PixelGraph(TChain*tr, int col, int row, int color){
  color_number=color;
  tree=tr;
  target_column=col;
  target_row=row;

  tree->Draw(">>elist",Form("Column==%d&&Row==%d",target_column,target_row));
  elist=(TEventList*)gROOT->FindObject("elist");
  total_entry=elist->GetN();
}
void PixelGraph::init(){
  Threshold=0;
  Noise=0;
  Column=0;
  Row=0;
  DiffPrecomp=0;
  DiffVth1=0;
  DiffVth2=0;
  TDAC=0;

  int id=0;
  for(int i=DiffVth1_min;i<=DiffVth1_max;i=i+DiffVth1_step){
    sg_prec_cDiffVth1[id]=new SupportTGraph();
    sg_prec_cDiffVth1[id]->SetTitle(Form("Threshold_vs_PrecompVbn(VTH1==%d);PrecompVbn;Threshold[e]",i));
    sg_prec_cDiffVth1[id]->SetXrange(DiffPrecomp_min,DiffPrecomp_max+150);
    sg_prec_cDiffVth1[id]->Set_const_DiffVth1(i);
    sg_TDAC_cDiffVth1[id]=new SupportTGraph();
    sg_TDAC_cDiffVth1[id]->SetTitle(Form("Threshold_vs_TDAC(VTH1==%d);TDAC;Threshold[e]",i));
    sg_TDAC_cDiffVth1[id]->SetXrange(-17,20);
    sg_TDAC_cDiffVth1[id]->Set_const_DiffVth1(i);

    id++;
  }
  graph_num_cVth1=id;
  
  id=0;
  for(int j=DiffPrecomp_min;j<=DiffPrecomp_max;j=j+DiffPrecomp_step){
    sg_vth1_cDiffPrecomp[id]=new SupportTGraph();
    sg_vth1_cDiffPrecomp[id]->SetTitle(Form("Threshold_vs_VTH1(PrecompVbn==%d;VTH1;Threshold[e]",j));
    sg_vth1_cDiffPrecomp[id]->SetXrange(DiffVth1_min,DiffVth1_max);
    sg_vth1_cDiffPrecomp[id]->Set_const_DiffPrecomp(j);
    sg_TDAC_cDiffPrecomp[id]=new SupportTGraph();
    sg_TDAC_cDiffPrecomp[id]->SetTitle(Form("Threshold_vs_TDAC(PrecompVbn)==%d;TDAC;Threshold[e]",j));
    sg_TDAC_cDiffPrecomp[id]->SetXrange(-17,20);
    sg_TDAC_cDiffPrecomp[id]->Set_const_DiffPrecomp(j);
    id++;
  }
  graph_num_cPrec=id;
  id=0;
  for(int k=target_min;k<=target_max;k=k+target_step){
    sg_TDAC_ctarget[id]=new SupportTGraph();
    sg_TDAC_ctarget[id]->SetTitle(Form("Threshold_vs_TDAC(target==%d;TDAC;Threshold[e]",k));
    sg_TDAC_ctarget[id]->SetXrange(-17,20);
    sg_TDAC_ctarget[id]->Set_const_target_threshold(k);
    id++;
  }
  graph_num_ctarget=id;
  this->SetBranch();
}
void PixelGraph::SetBranch(){
  tree->SetBranchAddress("RunID",&RunID,&b_RunID);
  tree->SetBranchAddress("Column", &Column, &b_Column);
  tree->SetBranchAddress("Row", &Row, &b_Row);
  tree->SetBranchAddress("Threshold", &Threshold, &b_Threshold);
  tree->SetBranchAddress("Noise", &Noise, &b_Noise);
  tree->SetBranchAddress("DiffVth1", &DiffVth1, &b_DiffVth1);
  tree->SetBranchAddress("DiffVth2", &DiffVth2, &b_DiffVth2);
  tree->SetBranchAddress("DiffPrecomp", &DiffPrecomp, &b_DiffPrecomp);
  tree->SetBranchAddress("TDAC", &TDAC, &b_TDAC);
  tree->SetBranchAddress("target_threshold", &target_threshold, &b_target_threshold);
}
void PixelGraph::Loop(){
  int id=0;
  for(int iEntry=0;iEntry<total_entry;iEntry++){
    tree->GetEntry(elist->GetEntry(iEntry));
    id=0;
    for(int i_DiffVth1=DiffVth1_min;i_DiffVth1<DiffVth1_max;i_DiffVth1=i_DiffVth1+DiffVth1_step){
      if(DiffVth1==i_DiffVth1){
	sg_prec_cDiffVth1[id]->SetPoint(DiffPrecomp,Threshold);
	sg_TDAC_cDiffVth1[id]->SetPoint(TDAC,Threshold);
      }
      id++;
    }
    id=0;
    for(int i_DiffPrecomp=DiffPrecomp_min;i_DiffPrecomp<=DiffPrecomp_max;i_DiffPrecomp=i_DiffPrecomp+DiffPrecomp_step){
      if(DiffPrecomp==i_DiffPrecomp && Threshold > 0 ){
	sg_vth1_cDiffPrecomp[id]->SetPoint(DiffVth1,Threshold);
	sg_TDAC_cDiffPrecomp[id]->SetPoint(TDAC,Threshold);
      }
      id++;
    }
    id=0;
    for(int i_target_threshold=target_min;i_target_threshold<=target_max;i_target_threshold=i_target_threshold+target_step){
      if(target_threshold==i_target_threshold){
	//std::cout << target_threshold << std::endl;
	sg_TDAC_ctarget[id]->SetPoint(TDAC,Threshold);
	sg_TDAC_ctarget[id]->Set_const_DiffVth1(DiffVth1);
      }
      id++;
    }
  }

  int entry=0;
  for(id=0;id<graph_num_cVth1;id++){
    entry=sg_prec_cDiffVth1[id]->MakeTGraph(color_number);
    entry=sg_TDAC_cDiffVth1[id]->MakeTGraph(color_number);
  }
  for(id=0;id<graph_num_cPrec;id++){
    entry=sg_vth1_cDiffPrecomp[id]->MakeTGraph(color_number);
    entry=sg_TDAC_cDiffPrecomp[id]->MakeTGraph(color_number);
  }
  for(id=0;id<graph_num_ctarget;id++){
    entry=sg_TDAC_ctarget[id]->MakeTGraph(color_number);
  }
}
SupportTGraph* PixelGraph::Get_sg_prec_cDiffVth1(int num){
  if (num<graph_num_cVth1)
    return sg_prec_cDiffVth1[num];
  else
    return nullptr;
}
SupportTGraph* PixelGraph::Get_sg_TDAC_cDiffVth1(int num){
  if (num<graph_num_cVth1)
    return sg_TDAC_cDiffVth1[num];
  else
    return nullptr;
}

SupportTGraph* PixelGraph::Get_sg_TDAC_cDiffPrecomp(int num){
  if (num<graph_num_cPrec)
    return sg_TDAC_cDiffPrecomp[num];
  else
    return nullptr;
}
SupportTGraph* PixelGraph::Get_sg_vth1_cDiffPrecomp(int num){
  if (num<graph_num_cPrec)
    return sg_vth1_cDiffPrecomp[num];
  else
    return nullptr;
}
SupportTGraph* PixelGraph::Get_sg_TDAC_ctarget(int num){
  if (num<graph_num_ctarget){
    std::cout << num << std::endl;
    return sg_TDAC_ctarget[num];
  }
  else
    return nullptr;
}

SupportTGraph::SupportTGraph(){
  const_DiffVth1=0;
  const_DiffPrecomp=0;
  entry_num=0;
  x_min=0;
  x_max=0;
  gStyle->SetMarkerStyle(20);
}
void SupportTGraph::SetPoint(int point_x, int point_y){
  x.push_back(point_x);
  y.push_back(point_y);
  entry_num++;
}
void SupportTGraph::Set_const_DiffVth1(int par){
  const_DiffVth1=par;
}
void SupportTGraph::Set_const_DiffPrecomp(int par){
  const_DiffPrecomp=par;
}
void SupportTGraph::Set_const_target_threshold(int par){
  const_target_threshold=par;
}

int SupportTGraph::Get_const_DiffVth1(){
  return const_DiffVth1;
}
int SupportTGraph::Get_const_DiffPrecomp(){
  return const_DiffPrecomp;
}
int SupportTGraph::Get_const_target_threshold(){
  return const_target_threshold;
}

void SupportTGraph::SetTitle(std::string title){
  graph_title=title;
}
void SupportTGraph::SetXrange(int min, int max){
  x_min=min;
  x_max=max;
}
void SupportTGraph::SetColorNum(int num){
  std::cout << "gaya" << std::endl;
  graph->SetMarkerColor(num);
  graph->SetLineColor(num);
}

int SupportTGraph::MakeTGraph(int color){
  graph=new TGraph(entry_num);
  if(x_min!=0||x_max!=0){
    graph->GetXaxis()->SetLimits(x_min,x_max);
  }

  for(int i=0;i<entry_num;i++){
    graph->SetPoint(i,x[i],y[i]);
  }
  graph->SetMarkerColor(color);
  graph->SetLineColor(color);
  graph->SetMinimum(0);
  graph->SetMaximum(4000);
  /*
  graph->GetXaxis()->SetLimits(x_min,x_max);
  graph->GetXaxis()->SetTitleOffset(1.0);
  graph->GetXaxis()->SetTitleSize(0.045);
  graph->GetYaxis()->SetTitleOffset(1.0);
  graph->GetYaxis()->SetTitleSize(0.045);
  */
  return Get_entry_num();
}
void SupportTGraph::Draw(std::string option){
  TString opt=option;
  SetGayaStyle();
  if(graph_title!=""){
    graph->SetTitle(graph_title);
  }
  
  graph->Draw(opt);
}
int SupportTGraph::Get_entry_num(){
  return entry_num;
}

TGraph* SupportTGraph::Get_TGraph_address(){
  return graph;
}

int analysis_pixels(std::string dir){
  //  int Column_min=264;
  //  int Column_max=265;
  //  int Row_min=41;
  //  int Row_max=46;

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
  TCanvas*canvas=new TCanvas("canvas","");

  int i = 0;
  PixelGraph* pixel[Row_max-Row_min];
  SupportTGraph* gaya;
  int i_pixel=0;
  bool Print;
  TLegend *legend;
  double legend_1=0.82;
  double legend_2=0.25;
  double legend_3=0.95;
  double legend_4=0.85;
  
  for(int i_row=Row_min;i_row<Row_max;i_row++){
    pixel[i]=new PixelGraph(tree,i_column,i_row,i+1);
    pixel[i]->init();
    pixel[i]->Loop();
    i++;
  }
  
  //  canvas->Print(Form("%s/Threshold_vs_TDAC_col%d_row%d.pdf[",dir.c_str(),i_column,i_row));
  
  canvas->Print(Form("%s/Threshold_vs_TDAC.pdf[",dir.c_str()));
  i=0;
  while(gaya || i==0){
    Print=false;
    legend = new TLegend( legend_1, legend_2, legend_3, legend_4);
    for(i_pixel=0;i_pixel<Row_max-Row_min;i_pixel++){
      gaya=pixel[i_pixel]->Get_sg_TDAC_cDiffPrecomp(i);
      if (gaya && gaya->Get_entry_num()>0){
	legend->AddEntry(gaya->Get_TGraph_address(), Form("%d:%d",i_column,Row_min+i_pixel),"lp");
	Print=true;
	if(i_pixel==0)
	  gaya->Draw("APL");
	else
	  gaya->Draw("PL");
      }
    }
    if (Print){
      legend->Draw();
      canvas->Print(Form("%s/Threshold_vs_TDAC.pdf",dir.c_str()));
    }
    i++;
  }
  //canvas->Print(Form("%s/Threshold_vs_TDAC_col%d_row%d.pdf]",dir.c_str(),i_column,i_row));
  canvas->Print(Form("%s/Threshold_vs_TDAC.pdf]",dir.c_str()));
  
  //-------------Threshold_vs_DiffVth1--------------------------------------------------------
  TCanvas*canvas2=new TCanvas("canvas2","");
  canvas2->Print(Form("%s/Threshold_vs_DiffVth1.pdf[",dir.c_str()));
  i=0;
  while(gaya || i==0){
    Print=false;
    legend = new TLegend( legend_1, legend_2, legend_3, legend_4);
    for(i_pixel=0;i_pixel<Row_max-Row_min;i_pixel++){
      gaya=pixel[i_pixel]->Get_sg_vth1_cDiffPrecomp(i);
      if (gaya && gaya->Get_entry_num()>0){
	legend->AddEntry(gaya->Get_TGraph_address(), Form("%d:%d",i_column,Row_min+i_pixel),"lp");
	Print=true;
	if(i_pixel==0){
	  gaya->Draw("APL");
	}
	else{
	  gaya->Draw("PL");
	}
      }
    }
    if (Print){
      legend->Draw();
      canvas2->Print(Form("%s/Threshold_vs_DiffVth1.pdf",dir.c_str()));
    }
    i++;
  }
  canvas2->Print(Form("%s/Threshold_vs_DiffVth1.pdf]",dir.c_str()));

  //-------------Threshold_vs_DiffPrecomp--------------------------------------------------------
  TCanvas*canvas3=new TCanvas("canvas3","");
  canvas3->Print(Form("%s/Threshold_vs_DiffPrecomp.pdf[",dir.c_str()));
  i=0;
  while(gaya || i==0){
    Print=false;
    legend = new TLegend( legend_1, legend_2, legend_3, legend_4);
    for(i_pixel=0;i_pixel<Row_max-Row_min;i_pixel++){
      gaya=pixel[i_pixel]->Get_sg_prec_cDiffVth1(i);
      if (gaya && gaya->Get_entry_num()>0){
	legend->AddEntry(gaya->Get_TGraph_address(), Form("%d:%d",i_column,Row_min+i_pixel),"lp");
	Print=true;
	if(i_pixel==0){
	  gaya->Draw("APL");
	}
	else{
	  gaya->Draw("PL");
	}
      }
    }
    if (Print){
      legend->Draw();
      canvas3->Print(Form("%s/Threshold_vs_DiffPrecomp.pdf",dir.c_str()));
    }
    i++;
  }
  canvas3->Print(Form("%s/Threshold_vs_DiffPrecomp.pdf]",dir.c_str()));

  
  //------------------graph in one pixel------------------------------------
  i_pixel=0;
  TCanvas*canvas4=new TCanvas("canvas4","");
  canvas4->Print(Form("%s/one_pixel.pdf[",dir.c_str()));
  bool First_Print;
  int const_DiffPrecomp=0;
  int const_DiffVth1=0;
  int const_target_threshold=0;
  int color_num=1;
  
  for(i_pixel=0;i_pixel<Row_max-Row_min;i_pixel++){
    First_Print=true;
    legend = new TLegend( legend_1, legend_2, legend_3, legend_4);
    i=0; color_num=1;
    while(gaya || i==0){
      Print=false;
      gaya=pixel[i_pixel]->Get_sg_TDAC_cDiffPrecomp(i);
      if (gaya && gaya->Get_entry_num()>0){
	Print=true;
	if (color_num==10){color_num++;}
	gaya->SetColorNum(color_num);
	const_DiffPrecomp=gaya->Get_const_DiffPrecomp();
	legend->AddEntry(gaya->Get_TGraph_address(), Form("%d",const_DiffPrecomp),"lp");
	if(First_Print){
	  gaya->SetTitle(Form("Threshold_vs_TDAC_Col_%d_row_%d;TDAC;Threshold[e]",i_column,i_pixel+Row_min));
	  gaya->Draw("APL");
	  First_Print=false;
	}
	else{
	  gaya->Draw("PL");
	}
	color_num++;
      }
      i++;
    }
    legend->Draw();
    canvas4->Print(Form("%s/one_pixel.pdf",dir.c_str()));
  }
  canvas4->Print(Form("%s/one_pixel.pdf]",dir.c_str()));
  

  TCanvas*canvas5=new TCanvas("canvas5","");
  canvas5->Print(Form("%s/Thre_vs_vth1_one_pixel.pdf[",dir.c_str()));
  for(i_pixel=0;i_pixel<Row_max-Row_min;i_pixel++){
    First_Print=true;
    legend = new TLegend( legend_1, legend_2, legend_3, legend_4);
    i=0; color_num=1;
    while(gaya || i==0){
      Print=false;
      gaya=pixel[i_pixel]->Get_sg_vth1_cDiffPrecomp(i);
      if (gaya && gaya->Get_entry_num()>0){
	Print=true;
	if (color_num==10){color_num++;}
	gaya->SetColorNum(color_num);
	const_DiffPrecomp=gaya->Get_const_DiffPrecomp();
	legend->AddEntry(gaya->Get_TGraph_address(), Form("%d",const_DiffPrecomp),"lp");
	if(First_Print){
	  gaya->SetTitle(Form("Threshold_vs_VTH1_Col_%d_row_%d;VTH1;Threshold[e]",i_column,i_pixel+Row_min));
	  gaya->Draw("APL");
	  First_Print=false;
	}
	else{
	  gaya->Draw("PL");
	}
	color_num++;
      }
      i++;
    }
    legend->Draw();
    canvas5->Print(Form("%s/Thre_vs_vth1_one_pixel.pdf",dir.c_str()));
  }
  canvas5->Print(Form("%s/Thre_vs_vth1_one_pixel.pdf]",dir.c_str()));
  

  TCanvas*canvas6=new TCanvas("canvas6","");
  canvas6->Print(Form("%s/Thre_vs_TDAC_one_pixel.pdf[",dir.c_str()));
  for(i_pixel=0;i_pixel<Row_max-Row_min;i_pixel++){
    First_Print=true;
    legend = new TLegend( legend_1, legend_2, legend_3, legend_4);
    i=0; color_num=1;
    while(gaya || i==0){
      Print=false;
      gaya=pixel[i_pixel]->Get_sg_TDAC_cDiffVth1(i);
      if (gaya && gaya->Get_entry_num()>0){
	Print=true;
	if (color_num==10){color_num++;}
	gaya->SetColorNum(color_num);
	const_DiffVth1=gaya->Get_const_DiffVth1();
	legend->AddEntry(gaya->Get_TGraph_address(), Form("%d",const_DiffVth1),"lp");
	if(First_Print){
	  gaya->SetTitle(Form("Threshold_vs_TDAC_Col_%d_row_%d;TDAC;Threshold[e]",i_column,i_pixel+Row_min));
	  gaya->Draw("APL");
	  First_Print=false;
	}
	else{
	  gaya->Draw("PL");
	}
	color_num++;
      }
      i++;
    }
    legend->Draw();
    canvas6->Print(Form("%s/Thre_vs_TDAC_one_pixel.pdf",dir.c_str()));
  }
  canvas6->Print(Form("%s/Thre_vs_TDAC_one_pixel.pdf]",dir.c_str()));

  TCanvas*canvas7=new TCanvas("canvas7","");
  canvas7->Print(Form("%s/Thre_vs_TDAC_ctarget.pdf[",dir.c_str()));
  for(i_pixel=0;i_pixel<Row_max-Row_min;i_pixel++){
    First_Print=true;
    legend = new TLegend( legend_1, legend_2, legend_3, legend_4);
    i=0; color_num=1;
    while(gaya || i==0){
      Print=false;
      gaya=pixel[i_pixel]->Get_sg_TDAC_ctarget(i);
      std::cout << gaya <<std::endl;
      if (gaya && gaya->Get_entry_num()>0){
	Print=true;
	if (color_num==10){color_num++;}
	gaya->SetColorNum(color_num);
	const_target_threshold=gaya->Get_const_target_threshold();
	const_DiffVth1=gaya->Get_const_DiffVth1();
        legend->AddEntry(gaya->Get_TGraph_address(), Form("%d",const_target_threshold),"lp");
	//        legend->AddEntry(gaya->Get_TGraph_address(), Form("%d",const_DiffVth1),"lp");
	if(First_Print){
	  gaya->SetTitle(Form("Threshold_vs_TDAC_Col_%d_row_%d;TDAC ;Threshold[e]",i_column,i_pixel+Row_min));
	  gaya->Draw("APL");
	  First_Print=false;
	}
	else{
	  gaya->Draw("PL");
	}
	color_num++;
      }
      i++;
    }
    legend->Draw();
    canvas7->Print(Form("%s/Thre_vs_TDAC_ctarget.pdf",dir.c_str()));
  }
  canvas7->Print(Form("%s/Thre_vs_TDAC_ctarget.pdf]",dir.c_str()));


  return 0;
}
