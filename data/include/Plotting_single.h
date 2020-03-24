#ifndef PLOTING_SINGLE_H
#define PLOTING_SINGLE_H

#include <TH1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLegend.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TText.h>
#include <fstream>
#include <cstdlib>

#define core_col 8
#define core_row 8

//#include "GayaGraph.h"

class Plotting_single{
 public:
  Plotting_single();
  virtual void init(int target);
  virtual void additional_init(){};
  virtual void valueset(){};
  virtual void sort_value(int a, int b, double c, double d, double e, double f, double g, double h, double i, double j){};
  virtual void add_histogram(){};
  virtual void Draw();
  virtual void additional_draw(){};
  virtual void resultDraw(){};

  virtual void value_set(int col, int row, double value);
  virtual void pdf_open();
  virtual void pdf_close();
  virtual void SetOutputDir(std::string str);

 protected:
  virtual void declare_hist();
  virtual void text_position(double& x,double& y);
  virtual void DrawOption();
  virtual void ColorOption();
  virtual void Fitting(){};
  TH1F* hist;
  TH2F* histMap;
  THStack* hs; 

  TString dist_title;
  TString map_title;

  TLegend* legend;
  std::vector <TH1F*> hs_stock;
  
  TCanvas* canvas;

  double based_bin;
  double based_min;
  double based_max;
  TString dist_type;
  TString x_axis_title;

  double hist_minimum;
  double hist_maximum;
  double now_target_threshold;

  TString picture_dir;
  TText* text;
};

class ThresholdDist : public Plotting_single{
 public:
  ThresholdDist();
  //void init(int target);
  void additional_init();
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
  void Draw();
  void resultDraw();
  double get_threshold_mean();
  double get_threshold_sigma();

 private:
  void ColorOption();
  void Fitting();
  double threshold_mean;
  double threshold_sigma;
  double current_vth1;

  std::vector <double> mean_vec;
  std::vector <double> sigma_vec;
  std::vector <double> mean_er_vec;
  std::vector <double> sigma_er_vec;

  std::vector <double> target_thre_vec;
  std::vector <double> diff_mean_target_vec;
  std::vector <double> diff_vth1_vec;

  std::vector <TGraphErrors*> graph_list;
};

class Threshold2Dist : public Plotting_single{
 public:
  Threshold2Dist();
  //void additional_init();
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
  void Draw();
  //void resultDraw(){};

 private:
  void ColorOption();
};

class TDACDist : public Plotting_single{
 public:
  TDACDist();
  //void init(int target);
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
  //void additional_draw();
 private:
  void DrawOption();
};

class TDAC2Dist : public Plotting_single{
 public:
  TDAC2Dist();
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
  void additional_draw();
 private:
  void DrawOption();
  void ColorOption();
};

class NoiseDist : public Plotting_single{
 public:
  NoiseDist();
  //void additional_init();
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
  //void additional_draw();
  //void pdf_open();
  //void pdf_close();
  //void text_position(double& x,double& y){ x=0.4; y=0.6;};

  //private:
  //TH1F*core_noisehist[core_col][core_row];
  
};

class Threshold_viaDist : public Plotting_single{
 public:
  Threshold_viaDist();
  //void init(int target);
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
 private:
  void ColorOption();

};
class Noise_viaDist : public Plotting_single{
 public:
  Noise_viaDist();
  //void init(int target);
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
};
class Threshold_via2Dist : public Plotting_single{
 public:
  Threshold_via2Dist();
  //void init(int target);
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
 private:
  void ColorOption();

};

class LowerLimitDist : public Plotting_single{
 public:
  LowerLimitDist();
  LowerLimitDist(int num);
  void init(int target);
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
  void Draw();
  void resultDraw();
  void value_set(int col, int row, double value);

  void pdf_open();
  void pdf_close();
  void SetTitle(TString str);
  void SetNumber(unsigned num);
  std::ofstream ofs;
 private:
  void FirstInit();
  void DrawOption();
  void ColorOption();
  void declare_hist();
  double totalentry;  
  double typical_target;
  unsigned number;

  std::vector <TH2F*> hist2D_vec;
};

class TDAC_viaDist : public Plotting_single{
 public:
  TDAC_viaDist();
  //void init(int target);
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
 private:
  void DrawOption();
};
class DiffTDACDist : public Plotting_single{
 public:
  DiffTDACDist();
  //void init(int target);
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
 private:
  void ColorOption();
  void DrawOption();
};
class DiffThresholdDist : public Plotting_single{
 public:
  DiffThresholdDist();
  //void init(int target);
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
 private:
  void DrawOption();
};
class DiffNoiseDist : public Plotting_single{
 public:
  DiffNoiseDist();
  //void init(int target);
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
 private:
  void DrawOption();
};
class DiffNoise2Dist : public Plotting_single{
 public:
  DiffNoise2Dist();
  //void init(int target);
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
 private:
  void DrawOption();
};
class Noise_via2Dist : public Plotting_single{
 public:
  Noise_via2Dist();
  //void init(int target);
  void sort_value(int col, int row, double mean, double threshold, double Noise, double TDAC, double Enable, double threshold_via, double Noise_via, double TDAC_via);
  void add_histogram();
};
#endif
