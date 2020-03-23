#ifndef READ_DAT_H
#define READ_DAT_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <TTree.h>
#include <TFile.h>
#include <vector>

class ReadDatFile {
 public:
  ReadDatFile(std::string s){
    dat_file=s;
    init();
  };
  void init();
  void add_datfile(std::string s);
  int read_datfile();

  void set_parameter(std::string name1, int par1, std::string name2, int par2,std::string name3, int par3,std::string name4,double par4[400][192],std::string name5,double par5[400][192],std::string name6,double par6[400][192]);
  void set_scanpar(std::string name1, int par1, std::string name2, int par2,std::string name3, int par3,std::string name4,double par4);
  void make_TTree(std::string s,int c,int tar_thre,int time);

  std::string get_plotType(){return plotType;};
  std::string get_plotTitle(){return plotTitle;};
  std::string get_x_axis(){return x_axis;};
  std::string get_y_axis(){return y_axis;};
  std::string get_z_axis(){return z_axis;};
  void get_xrange(double x[3]){
    for(int i=0;i<3;i++){ 
      x[i]=xrange[i];
    }
  };
  void get_yrange(double y[3]){
    for(int i=0;i<3;i++){ 
      y[i]=yrange[i];
    }
  };
  int get_over_flow(){return over_flow;};
  int get_under_flow(){return under_flow;};
  
  TTree*tree;
  TFile*file;
 private:
  std::string dat_file;

  int RD53a_column;
  int RD53a_row;

  std::string plotType;
  std::string plotTitle;
  std::string x_axis;
  std::string y_axis;
  std::string z_axis;
  std::string parName1;
  int parameter1;
  std::string parName2;
  int parameter2;
  std::string parName3;
  int parameter3;
  std::string parName4;
  std::vector <std::vector<int> > parameter4;
  std::string parName5;
  std::vector <std::vector<int> > parameter5;
  std::string parName6;
  std::vector <std::vector<int> > parameter6;

  std::string scanparName1;
  int scanpar1;
  std::string scanparName2;
  int scanpar2;
  std::string scanparName3;
  int scanpar3;
  std::string scanparName4;
  int scanpar4;

  
  double xrange[3];
  double yrange[3];
  int over_flow;
  int under_flow;

  //  double value_map[400][400];
  //double value_Dist[100];
  std::vector <std::vector<double> > value_map;
  std::vector <std::vector<double> > value_map2;
};
/*
class MakeROOT {
 public:
  MakeROOT(ReadDatFile*rd){
    plotType=rd->get_plotType();
    rd->get_xrange(xrange);
    value_map=rd->get_value_map();
    std::cout << value_map[300][300] << std::endl; 
  };
  void make_TTree();
 private:
  std::string plotType;
  std::string plotTitle;
  std::string x_axis;
  std::string y_axis;
  std::string z_axis;
  double xrange[3];
  double yrange[3];
  int over_flow=0;
  int under_flow=0;

  double value_map[400][400];

  TTree*tree;
  TFile*file;
  };*/
#endif
