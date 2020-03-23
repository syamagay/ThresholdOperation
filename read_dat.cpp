#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "read_dat.h"
#include "picojson.h"
// a[column][row]

void ReadDatFile::init(){
  RD53a_column=400;
  RD53a_row=192;
  for(int i=0;i<3;i++){
    xrange[i]=0;
    yrange[i]=0;
  }
  over_flow=0;
  under_flow=0;
  value_map=std::vector<std::vector<double>>(RD53a_column, std::vector<double>(RD53a_row));
  parameter4=std::vector<std::vector<int>>(RD53a_column, std::vector<int>(RD53a_row));
  parameter5=std::vector<std::vector<int>>(RD53a_column, std::vector<int>(RD53a_row));
  parameter6=std::vector<std::vector<int>>(RD53a_column, std::vector<int>(RD53a_row));

  for(int cl=0;cl<RD53a_column;cl++){
    for(int ro=0;ro<RD53a_row;ro++){
      value_map[cl][ro]=0;
    }
  }
  std::cout << "ReadDatFile is initialized!" << std::endl;
}

void ReadDatFile::add_datfile(std::string new_dat_file){
  dat_file=new_dat_file;
  value_map2=value_map;
  std::cout << value_map2[264][50] << std::endl;
}

int ReadDatFile::read_datfile(){
  ifstream fin(dat_file);
  if(fin.fail()){
    std::cout << "akaaaaaann" << std::endl;
    return 1;
  }
  if(getline(fin,plotType)){
    //    std::cout << plotType << std::endl;
    if(plotType.find("Histo2d") != std::string::npos){
      getline(fin,plotTitle);
      getline(fin,x_axis);
      getline(fin,y_axis);
      getline(fin,z_axis);
      fin >> xrange[0] >> xrange[1] >> xrange[2];
      fin >> yrange[0] >> yrange[1] >> yrange[2];
      fin >> over_flow >> under_flow;
    }
    else if(plotType.find("Histo1d") != std::string::npos){
      std::cout << plotType << std::endl;
      getline(fin,plotTitle);
      getline(fin,x_axis);
      getline(fin,y_axis);
      getline(fin,z_axis);
      fin >> xrange[0] >> xrange[1] >> xrange[2];
      fin >> over_flow >> under_flow;
    }

    
    if(plotType.find("Histo2d") != std::string::npos){
      for(int j=0;j<RD53a_row;j++){
	for(int i=0;i<RD53a_column;i++){    
	  fin >> value_map[i][j];
	}
      }
    }
  }
  return 0;
}
void ReadDatFile::set_parameter(std::string name1,int par1,std::string name2,int par2,std::string name3,int par3,std::string name4,double par4[400][192],std::string name5,double par5[400][192],std::string name6,double par6[400][192]){
  parName1=name1;
  parameter1=par1;
  parName2=name2;
  parameter2=par2;
  parName3=name3;
  parameter3=par3;
  parName4=name4;
  parName5=name5;
  parName6=name6;
  for(int i=0;i<400;i++){
    for(int j=0;j<192;j++){
      parameter4[i][j]=par4[i][j];
      parameter5[i][j]=par5[i][j];
      parameter6[i][j]=par6[i][j];
    }
  }
}
void ReadDatFile::set_scanpar(std::string name1,int par1,std::string name2,int par2,std::string name3,int par3,std::string name4,double par4){
  scanparName1=name1;
  scanpar1=par1;
  scanparName2=name2;
  scanpar2=par2;
  scanparName3=name3;
  scanpar3=par3;
  scanparName4=name4;
  scanpar4=par4;
}

void ReadDatFile::make_TTree(std::string output_dir,int count,int target_threshold,int scan_time){
  int Column=0;
  int Row=0;
  double Threshold=0;
  double Noise=0;
  int RunID=count;
  int TDAC=0;
  int Enable=0;
  int InjEn=0;
  file=new TFile(Form("%s/gaya%d.root",output_dir.c_str(),count),"recreate");
  tree=new TTree("tree","");

  //  char* x=x_axis.c_str();
  //char* y=y_axis.c_str();
  //char* z=z_axis.c_str();
  tree->Branch("RunID",&RunID,"RunID/I");
  tree->Branch(x_axis.c_str(),&Column,Form("%s/I",x_axis.c_str()));
  tree->Branch(y_axis.c_str(),&Row,Form("%s/I",y_axis.c_str()));
  tree->Branch("target_threshold",&target_threshold,"target_threshold/I");
  tree->Branch("scan_time",&scan_time,"scan_time/I");
  tree->Branch("Threshold",&Threshold,"Threshold/D");
  tree->Branch("Noise",&Noise,"Noise/D");
  tree->Branch(parName1.c_str(),&parameter1,Form("%s/I",parName1.c_str()));
  tree->Branch(parName2.c_str(),&parameter2,Form("%s/I",parName2.c_str()));
  tree->Branch(parName3.c_str(),&parameter3,Form("%s/I",parName3.c_str()));
  tree->Branch(parName4.c_str(),&TDAC,Form("%s/I",parName4.c_str()));
  tree->Branch(parName5.c_str(),&Enable,Form("%s/I",parName5.c_str()));
  tree->Branch(parName6.c_str(),&InjEn,Form("%s/I",parName6.c_str()));
  tree->Branch(scanparName1.c_str(),&scanpar1,Form("%s/I",scanparName1.c_str()));
  tree->Branch(scanparName2.c_str(),&scanpar2,Form("%s/I",scanparName2.c_str()));
  tree->Branch(scanparName3.c_str(),&scanpar3,Form("%s/I",scanparName3.c_str()));
  tree->Branch(scanparName4.c_str(),&scanpar4,Form("%s/I",scanparName4.c_str()));

  for(Column=0;Column<RD53a_column;Column++){
    for(Row=0;Row<RD53a_row;Row++){
      Threshold=value_map2[Column][Row];
      Noise=value_map[Column][Row];
      TDAC=parameter4[Column][Row];
      Enable=parameter5[Column][Row];
      InjEn=parameter6[Column][Row];
      //      std::cout << Threshold << std::endl;
      tree->Fill();
    }
  }
  tree->Write();
}
/*
int make_root(std::string dat_file, std::string dat_file2, std::string output_dir, std::string parname1, double parameter1,std::string parname2, double parameter2 ,std::string parname3, double parameter3 ,std::string parname4,double parameter4[400][192],std::string parname5,double parameter5[400][192],std::string parname6,double parameter6[400][192],int count, int target_threshold){
  ReadDatFile*rd=new ReadDatFile(dat_file);
  rd->set_parameter(parname1,parameter1,parname2,parameter2,parname3,parameter3,parname4,parameter4,parname5,parameter5,parname6,parameter6);
  int read_dat=rd->read_datfile();
  rd->add_datfile(dat_file2);
  int read_dat2=rd->read_datfile();
  if(read_dat==0&&read_dat==0){
    std::cout << "Reading dat file is succeed!" << std::endl;
    rd->make_TTree(output_dir,count,target_threshold);
    
    std::cout << rd->tree->GetEntries() << std::endl;
  }
  else{
    return 1;
  }
  return 0;
  }*/
int read_dat(std::string target_dir,std::string output_dir, int count, int target_threshold=0, int scantime=0){
  std::string dat="chipId0_ThresholdMap-0.dat";
  std::string dat2="chipId0_NoiseMap-0.dat";
  std::string dat_file=target_dir+dat;
  std::string dat_file2=target_dir+dat2;
  
  std::string json="chipId0.json.after";
  std::string json_file=target_dir+json;

  std::string scanjson="diff_thresholdscan.json";
  std::string scanjson_file=target_dir+scanjson;

  ifstream jf(json_file);
  if(jf.fail()){
    std::cout << json_file << std::endl;
    std::cout << "json ga akaaaaaann" << std::endl;
    return 1;
  }  
  ifstream sjf(scanjson_file);
  if(sjf.fail()){
    std::cout << scanjson_file << std::endl;
    std::cout << "scan json ga akaaaaaann" << std::endl;
    return 1;
  }  

  picojson::value val;
  jf >> val;

  std::string parName1="DiffVth1";
  std::string parName2="DiffVth2";
  std::string parName3="DiffPrecomp";
  std::string parName4="TDAC";
  std::string parName5="Enable";
  std::string parName6="InjEn";
  double par1=0;
  double par2=0;
  double par3=0;
  double par4[400][192];
  double par5[400][192];
  double par6[400][192];

  int i=0;
  int j=0;
    
  par1=val.get<picojson::object>()
    ["RD53A"].get<picojson::object>()
    ["GlobalConfig"].get<picojson::object>()
    [parName1].get<double>();
  par2=val.get<picojson::object>()
    ["RD53A"].get<picojson::object>()
    ["GlobalConfig"].get<picojson::object>()
    [parName2].get<double>();
  par3=val.get<picojson::object>()
    ["RD53A"].get<picojson::object>()
    ["GlobalConfig"].get<picojson::object>()
    [parName3].get<double>();
  
  picojson::array& array=val.get<picojson::object>()
    ["RD53A"].get<picojson::object>()
    ["PixelConfig"].get<picojson::array>();
  for (picojson::array::iterator it = array.begin(); it != array.end(); it++) {
    picojson::array& TDAC_List=it->get<picojson::object>()
      ["TDAC"].get<picojson::array>();
    j=0;
    for (picojson::array::iterator it2 = TDAC_List.begin(); it2 != TDAC_List.end(); it2++){
      par4[i][j]=it2->get<double>();
      j++;
    }
    picojson::array& Enable_List=it->get<picojson::object>()
      ["Enable"].get<picojson::array>();
    j=0;
    for (picojson::array::iterator it2 = Enable_List.begin(); it2 != Enable_List.end(); it2++){
      par5[i][j]=it2->get<double>();
      j++;
    }
    picojson::array& InjEn_List=it->get<picojson::object>()
      ["InjEn"].get<picojson::array>();
    j=0;
    for (picojson::array::iterator it2 = InjEn_List.begin(); it2 != InjEn_List.end(); it2++){
      par6[i][j]=it2->get<double>();
      j++;
    }

    i++;
  }

  picojson::value val2;
  sjf >> val2;
  std::string scanparName1="min";
  std::string scanparName2="max";
  std::string scanparName3="step";
  std::string scanparName4="trigCount";
  double scanpar1=0;
  double scanpar2=0;
  double scanpar3=0;
  double scanpar4=0;

  picojson::array& array2=val2.get<picojson::object>()
    ["scan"].get<picojson::object>()
    ["loops"].get<picojson::array>();
  picojson::array::iterator it2 = array2.begin();
  it2++;
  scanpar1 =it2->get<picojson::object>()["config"].get<picojson::object>()["min"].get<double>();
  scanpar2 =it2->get<picojson::object>()["config"].get<picojson::object>()["max"].get<double>();
  scanpar3 =it2->get<picojson::object>()["config"].get<picojson::object>()["step"].get<double>();

  it2++;
  it2++;

  scanpar4=it2->get<picojson::object>()["config"].get<picojson::object>()["count"].get<double>();

  
  //  int success=make_root(dat_file,dat_file2,output_dir,parName1,par1,parName2,par2,parName3,par3,parName4,par4,parName5,par5,parName6,par6,count,target_threshold);

  ReadDatFile*rd=new ReadDatFile(dat_file);
  rd->set_parameter(parName1,par1,parName2,par2,parName3,par3,parName4,par4,parName5,par5,parName6,par6);
  rd->set_scanpar(scanparName1,scanpar1,scanparName2,scanpar2,scanparName3,scanpar3,scanparName4,scanpar4);

  int read_dat=rd->read_datfile();
  rd->add_datfile(dat_file2);
  int read_dat2=rd->read_datfile();
  if(read_dat==0&&read_dat==0){
    std::cout << "Reading dat file is succeed!" << std::endl;
    rd->make_TTree(output_dir,count,target_threshold,scantime);
    
    std::cout << rd->tree->GetEntries() << std::endl;
  }
  else{
    return 1;
  }
  return 0;

  //return success;
}
