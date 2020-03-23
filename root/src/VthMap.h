#ifndef VTHMAP_HXX
#define VTHMAP_HXX

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
#include <TPaletteAxis.h>

#include <iostream>
#include <stdio.h>
#include <vector>

#define graph_num 6
#define DiffVth1_min 0
#define DiffVth1_max 500
#define DiffVth1_step 25
#define DiffVth2_min 0
#define DiffVth2_max 500
#define DiffVth2_step 25
#define DiffPrecomp_min 0
#define DiffPrecomp_max 1100
#define DiffPrecomp_step 50

class makeVthMap {
 public:
  makeVthMap(std::string dir,TChain* tr, int col, int row);
  ~makeVthMap();
  void init();
  TGraph* GetTGraph();
  void SetBranch();
  void Loop();
  void Draw();

 private:
  TH2F *hist[(DiffPrecomp_max-DiffPrecomp_min)/DiffPrecomp_step];
  TGraph* graph;
  std::vector<double> graph_x;
  std::vector<double> graph_y;

  TString target_dir;
  int RunID;
  double Threshold;
  double Noise;
  int Column;
  int Row;
  int DiffPrecomp;
  int DiffVth1;
  int DiffVth2;
  int TDAC;

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
};

#endif
