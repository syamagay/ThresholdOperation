#ifndef PIXELGRAPH_HXX
#define PIXELGRAPH_HXX

#include <math.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TChain.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TEntryList.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TLegend.h>

#include <iostream>
#include <stdio.h>
#include <vector>

#define graph_num 7
#define DiffVth1_min 50
#define DiffVth1_max 600
#define DiffVth1_step 25
#define DiffPrecomp_min 0
#define DiffPrecomp_max 1100
#define DiffPrecomp_step 50
#define DiffVth12_min 100
#define DiffVth12_max 1000
#define DiffVth12_setp 25

#define target_min 0
#define target_max 2000
#define target_step 20


class SupportTGraph {
 public:
  SupportTGraph();
  ~SupportTGraph();
  void SetPoint(int x, int y);
  void SetTitle(std::string str);
  void SetColorNum(int color=1);
  void SetXrange(int min,int max);
  void Set_const_DiffVth1(int par);
  void Set_const_DiffPrecomp(int par);
  void Set_const_target_threshold(int par);

  int Get_const_DiffVth1();
  int Get_const_DiffPrecomp();
  int Get_const_target_threshold();

  TGraph* Get_TGraph_address();

  int MakeTGraph(int color=1);
  void Draw(std::string opt="");
  int Get_entry_num();
  
 private:
  TGraph*graph;
  int color_num;
  int entry_num;
  TString graph_title;
  std::vector<double> x;
  std::vector<double> y;
  int x_min;
  int x_max;

  int const_DiffVth1;
  int const_DiffPrecomp;
  int const_target_threshold;
};


class PixelGraph {
 public:
  PixelGraph(TChain* tr, int col, int row, int color=1);
  ~PixelGraph();
  void init();
  void SetBranch();
  void Loop();
  SupportTGraph* Get_sg_prec_cDiffVth1(int num);
  SupportTGraph* Get_sg_TDAC_cDiffVth1(int num);
  SupportTGraph* Get_sg_TDAC_cDiffPrecomp(int num);
  SupportTGraph* Get_sg_vth1_cDiffPrecomp(int num);
  SupportTGraph* Get_sg_TDAC_ctarget(int num);

 private:

  SupportTGraph* sg_prec_cDiffVth1[100];
  SupportTGraph* sg_TDAC_cDiffVth1[100];
  SupportTGraph* sg_TDAC_ctarget[200];
  SupportTGraph* sg_vth1_cDiffPrecomp[100];
  SupportTGraph* sg_TDAC_cDiffPrecomp[100];

  int graph_num_cVth1;
  int graph_num_cPrec;
  int graph_num_ctarget;

  int RunID;
  int color_number;
  double Threshold;
  double Noise;
  int Column;
  int Row;
  int DiffPrecomp;
  int DiffVth1;
  int DiffVth2;
  int TDAC;
  int target_threshold;

  TEventList* elist;
  TChain* tree;
  int target_column;
  int target_row;
  int total_entry;

  TBranch *b_RunID;   //!                                                                                                                                 
  TBranch *b_Column;   //!                                                                                                                                
  TBranch *b_Row;   //!                                                                                                                                   
  TBranch *b_Threshold;   //!                                                                                                                             
  TBranch *b_Noise;   //!                                                                                                                                 
  TBranch *b_DiffVth1;   //!                                                                                                                              
  TBranch *b_DiffVth2;
  TBranch *b_DiffPrecomp;
  TBranch *b_TDAC;
  TBranch *b_target_threshold;
};

#endif
