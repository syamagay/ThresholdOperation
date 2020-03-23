#ifndef PLOTTING_SINGLE_CXX
#define PLOTTING_SINGLE_CXX
#include "Plotting_single.h"
#include "GayaStyle.h"
#define gaus_sigma 40
#define diff_col 264
#define column_max 400
#define row_max 192

#define plot_1 1700
#define plot_2 1200
#define plot_3 700
#define plot_4 200

bool makeSinglePlot(int target){
  if(target==plot_1){
    return true;
  }
  else if(target==plot_2){
    return true;
  }
  else if(target==plot_3){
    return true;
  }
  else if(target==plot_4){
    return true;
  }
  else{
    return false;
  }
}

Plotting_single::Plotting_single(){
  SetGayaStyle();

  picture_dir="picture/";
}
void Plotting_single::value_set(int Col, int Row, double value){
  hist->Fill(value);
  histMap->Fill(Col,Row,value);
}
void Plotting_single::pdf_open(){
  TString open =picture_dir+dist_title+"[";
  canvas->Print(open);
  open =picture_dir+map_title+"[";
  //canvas->Print(open);
}
void Plotting_single::pdf_close(){
  TString close =picture_dir+dist_title+"]";
  canvas->Print(close);
  close =picture_dir+map_title+"]";
  //canvas->Print(close);
}
void Plotting_single::DrawOption(){
  canvas->SetLogy();
  gStyle->SetOptStat("eMinoru");
}
void Plotting_single::ColorOption(){
  if(hs_stock.size()>0){
    for(unsigned it=0;it<hs_stock.size();it++){
      hs_stock[it]->SetFillColor(it+2);
    }
  }
  //default Color setting
}
void Plotting_single::declare_hist(){
  hist=new TH1F(dist_type+"_1d",dist_type+" distribution;"+x_axis_title+";# pixels",based_bin,based_min,based_max);
  histMap=new TH2F(dist_type+"_2d","",column_max-diff_col,diff_col,column_max,row_max,0,row_max);
  /*
  hist->SetTitleSize(0.045,"x");
  hist->SetTitleSize(0.045,"y");
  hist->SetTitleOffset(0.85);
  hist->GetXaxis()->SetTitleOffset(0.85);
  hist->GetYaxis()->SetTitleOffset(0.85);
  */
  hist->SetMinimum(hist_minimum);
  hist->SetMaximum(hist_maximum);
  canvas=new TCanvas(dist_type+"_c","");
}
void Plotting_single::init(int target_threshold){
  hist->Reset();
  histMap->Reset();
  for(unsigned it=0;it<hs_stock.size();it++){
    hs_stock[it]->Reset();
  }

  now_target_threshold=target_threshold;
  double text_x=0;
  double text_y=0;
  this->text_position(text_x,text_y);

  text=new TText(text_x,text_y,Form("Target Threshold= %d e",target_threshold));
  text->SetTextFont(62); //Helvetica bold
  hs=new THStack("hs",dist_type+Form("Dist target:%d;",target_threshold)+dist_type+";# pixels");
  //hist->SetTitle(dist_type+Form("Dist target:%d;",target_threshold)+dist_type+";# pixels");

  this->additional_init();
}

void Plotting_single::Draw(){
  this->DrawOption();
  this->ColorOption();

  if(hs_stock.size()>0){
    for(unsigned it=0;it<hs_stock.size();it++){
      hs->Add(hs_stock[it]);
    }
  }
  canvas->cd();
  if(hs_stock.size()>0){
    hist->Draw("hist");
    hs->Draw("HIST,same");
    canvas->RedrawAxis();
    legend->SetFillColor(0);
    legend->Draw("same");
    text->SetNDC(1);
    text->Draw();
  }
  else{
    hist->Draw();
  }
  canvas->Update();
  canvas->Print(picture_dir+dist_title);
  this->additional_draw();
}
void Plotting_single::SetOutputDir(std::string str){
  picture_dir=str;
}
void Plotting_single::text_position(double& x, double& y){
  x=0.15;
  y=0.82;
};

ThresholdDist::ThresholdDist(){
  dist_type="Threshold";
  x_axis_title="Threshold [e]";
  dist_title="tune_"+dist_type+"Dist.pdf";
  map_title="tune_"+dist_type+"Map.pdf";
  based_bin=251;
  based_min=-5;
  based_max=2505;

  hist_minimum=0.9;
  hist_maximum=20000;
  //hist_maximum=10000;

  this->declare_hist();
}

void ThresholdDist::additional_init(){
  threshold_mean=0;
  threshold_sigma=0;
}
void ThresholdDist::add_histogram(){
  //legend=new TLegend(0.65,0.38,0.88,0.58);
  legend=new TLegend(0.60,0.73,0.85,0.88);
  hs_stock.push_back(new TH1F("hist_thre_o3sg_15","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_thre_o3sg_m15","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_thre_o3sg_not_saturated","",based_bin,based_min,based_max));

  //legend->AddEntry(hs_stock[0],"TDAC==15");
  //legend->AddEntry(hs_stock[1],"TDAC==-15");
  legend->AddEntry(hs_stock[0],"Pixel Reg=15","lf");
  legend->AddEntry(hs_stock[1],"Pixel Reg=-15","lf");
  legend->AddEntry(hs_stock[2],"Pixel Reg : Other","lf");
}
void ThresholdDist::ColorOption(){
  hs_stock[0]->SetFillColor(4);
  hs_stock[1]->SetFillColor(6);
  hs_stock[2]->SetFillColor(5);
}
void ThresholdDist::sort_value(int Column,int Row, double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double DiffVth1){
  this->value_set(Column,Row,Threshold);
  current_vth1=DiffVth1;

  //double sigma=20;
  double three_sigma=0;
  three_sigma=gaus_sigma*3;
  if(TDAC==15&&(Threshold>=mean+three_sigma||Threshold<=mean-three_sigma)){
    hs_stock[0]->Fill(Threshold);
  }
  else if(TDAC==-15&&(Threshold>=mean+three_sigma||Threshold<=mean-three_sigma)){
    hs_stock[1]->Fill(Threshold);
  }
  else if(Threshold>=mean+three_sigma||Threshold<=mean-three_sigma){
    hs_stock[2]->Fill(Threshold);
  }
}
void ThresholdDist::Draw(){
  this->DrawOption();
  this->ColorOption();
  TF1*f=new TF1("f","gaus",0,3000);

  if(hs_stock.size()>0){
    for(unsigned it=0;it<hs_stock.size();it++){
      hs->Add(hs_stock[it]);
    }
  }
  canvas->cd();

  if(hs_stock.size()>0){
    //    std::cout << "fitting" << std::endl;
    canvas->SetLogy(0);
    hist->Draw("hist");
    double pseud_mean=hist->GetMean();
    double pseud_disp=hist->GetRMS();
    double fit_start=pseud_mean-pseud_disp;
    double fit_end=pseud_mean+pseud_disp;
    if(fit_start<0){
      fit_start=1;
    }
    hist->Fit("f","Q","",fit_start,fit_end);
    pseud_mean=f->GetParameter(1);
    pseud_disp=f->GetParameter(2);

    f->SetParameter(1,pseud_mean);
    f->SetParameter(2,pseud_disp);

    fit_start=pseud_mean-pseud_disp*2-10;
    fit_end=pseud_mean+pseud_disp*2+10;

    if(fit_start<0){
      fit_start=1;
    }

    hist->Fit("f","Q","",fit_start,fit_end);
    double threshold_er_mean=0;
    double threshold_er_sigma=0;
    threshold_mean=f->GetParameter(1);
    threshold_er_mean=f->GetParError(1);
    threshold_sigma=f->GetParameter(2);
    threshold_er_sigma=f->GetParError(2);
    f->Draw("same");
    //hist->Draw("hist");
    canvas->SetLogy(1);
    
    hs->Draw("HIST,same");
    canvas->RedrawAxis();
    legend->SetFillColor(0);
    legend->Draw("same");
    text->SetNDC(1);
    text->Draw();
    
    mean_vec.push_back(threshold_mean);
    sigma_vec.push_back(threshold_sigma);
    mean_er_vec.push_back(threshold_er_mean);
    sigma_er_vec.push_back(threshold_er_sigma);

    target_thre_vec.push_back(now_target_threshold);
    diff_mean_target_vec.push_back(threshold_mean-now_target_threshold);
    diff_vth1_vec.push_back(current_vth1);
  }
  else{
    hist->Draw();
  }
  canvas->Print(picture_dir+dist_title);

  if(makeSinglePlot((int)now_target_threshold)){
    hist->SetFillColor(3);
    hist->SetFillStyle(3003);
    hist->Draw();
    f->Draw("same");
    text->Draw();
    canvas->Print(picture_dir+Form("/Tuning%d.pdf",(int)now_target_threshold));
    hist->SetFillStyle(0);
  }

}
void ThresholdDist::resultDraw(){
  gStyle->SetMarkerStyle(9);

  unsigned entry_num=target_thre_vec.size();
  unsigned graph_num=4;
  for(unsigned i_graph=0;i_graph<graph_num;i_graph++){
    graph_list.push_back(new TGraphErrors(entry_num));
  }
  for(unsigned i_entry=0;i_entry<entry_num;i_entry++){
    if(i_entry==0){
      graph_list[0]->SetTitle("Threshold_mean;target_threshold [e];Threshold mean [e]");
      graph_list[0]->SetMinimum(-100);
      graph_list[0]->SetMaximum(2100);

      graph_list[1]->SetTitle("Threshold dispersion;target_threshold [e];Threshold dispersion [e]");
      graph_list[1]->SetMinimum(-100);
      graph_list[1]->SetMaximum(100);

      graph_list[2]->SetTitle("Difference between Mean and Target;target_threshold [e];mean-target threshold [e]");

      graph_list[3]->SetTitle("VTH1_vs_target;target_threshold;VTH1[Regitster Value]");
      graph_list[3]->SetMinimum(0);
      graph_list[3]->SetMaximum(500);


    }
    graph_list[0]->SetPoint(i_entry,target_thre_vec[i_entry],mean_vec[i_entry]);
    graph_list[1]->SetPoint(i_entry,target_thre_vec[i_entry],sigma_vec[i_entry]);
    graph_list[2]->SetPoint(i_entry,target_thre_vec[i_entry],diff_mean_target_vec[i_entry]);
    graph_list[3]->SetPoint(i_entry,target_thre_vec[i_entry],diff_vth1_vec[i_entry]);

    graph_list[0]->SetPointError(i_entry,0,mean_er_vec[i_entry]);
    graph_list[1]->SetPointError(i_entry,0,sigma_er_vec[i_entry]);
    graph_list[2]->SetPointError(i_entry,0,mean_er_vec[i_entry]);
    //    std::cout << mean_er_vec[i_entry] << " , " << sigma_er_vec[i_entry] << std::endl;

  }
  
  graph_list[2]->GetXaxis()->SetLimits(0,2000);
  graph_list[2]->SetMinimum(-20);
  graph_list[2]->SetMaximum(40);


  TCanvas*g_canvas=new TCanvas("g_canvas","");
  g_canvas->cd();
  
  for(unsigned i_graph=0;i_graph<graph_num;i_graph++){
    //graph_list[i_graph]->GetXaxis()->SetTitleSize(0.045);
    //graph_list[i_graph]->GetYaxis()->SetTitleSize(0.045);
    //graph_list[i_graph]->GetXaxis()->SetTitleOffset(0.85);
    //graph_list[i_graph]->GetYaxis()->SetTitleOffset(0.95);

    graph_list[i_graph]->GetXaxis()->SetNdivisions(50505);
    graph_list[i_graph]->Draw("APL");
    g_canvas->Print(picture_dir+dist_title);
  }
  delete g_canvas;
}

void ThresholdDist::Fitting(){
  TF1*f=new TF1("f","gaus",0,3000);
  hist->Draw();
  hist->Fit("f","Q","",100,3000);
  threshold_mean=f->GetParameter(1);
  threshold_sigma=f->GetParameter(2);
}

double ThresholdDist::get_threshold_mean(){
  if(threshold_mean==0){
    this->Fitting();
  }
  return threshold_mean;
}
double ThresholdDist::get_threshold_sigma(){
  if(threshold_sigma==0){
    this->Fitting();
  }
  return threshold_sigma;
}

Threshold2Dist::Threshold2Dist(){
  dist_type="Threshold2";
  x_axis_title="Threshold [e]";
  dist_title="tune_"+dist_type+"Dist.pdf";
  map_title="tune_"+dist_type+"Map.pdf";
  //based_bin=151;
  //based_min=-5;
  //based_max=1505;

  based_bin=126;
  based_min=-10;
  based_max=2510;

  hist_minimum=0.5;
  hist_maximum=10000;

  this->declare_hist();
}

void Threshold2Dist::add_histogram(){
  legend=new TLegend(0.55,0.73,0.85,0.88);
  hs_stock.push_back(new TH1F("hist_thre_0","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_thre_o3sg","",based_bin,based_min,based_max));

  legend->SetBorderSize(0);
  legend->AddEntry(hs_stock[0],"Thr^{meas}=0","f");
  legend->AddEntry(hs_stock[1],"#left|Thr^{meas}-Thr^{tar}#right|>120e","f");
}
void Threshold2Dist::ColorOption(){
  hs_stock[0]->SetFillColor(2);
  //hs_stock[0]->SetFillStyle(3344);
  hs_stock[1]->SetFillColor(4);
}
void Threshold2Dist::sort_value(int Column,int Row, double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double DiffVth1){
  this->value_set(Column,Row,Threshold);

  //double sigma=20;
  double three_sigma=0;
  three_sigma=gaus_sigma*3;
  if(Threshold==0){
    hs_stock[0]->Fill(Threshold);
  }
  else if(Threshold>=mean+three_sigma||Threshold<=mean-three_sigma){
    hs_stock[1]->Fill(Threshold);
  }
}
void Threshold2Dist::Draw(){
  this->DrawOption();
  this->ColorOption();

  canvas->SetLogy();
  gStyle->SetOptStat(0);
  legend->SetTextSize(0.04);
  hist->GetXaxis()->SetNdivisions(510);

  TF1*f=new TF1("f","gaus",0,3000);

  if(hs_stock.size()>0){
    for(unsigned it=0;it<hs_stock.size();it++){
      hs->Add(hs_stock[it]);
    }
  }
  canvas->cd();

  if(hs_stock.size()>0){
    //    std::cout << "fitting" << std::endl;
    canvas->SetLogy(0);
    hist->Draw("hist");
    double pseud_mean=hist->GetMean();
    double pseud_disp=hist->GetRMS();
    double fit_start=pseud_mean-pseud_disp;
    double fit_end=pseud_mean+pseud_disp;
    if(fit_start<0){
      fit_start=1;
    }
    hist->Fit("f","Q","",fit_start,fit_end);
    pseud_mean=f->GetParameter(1);
    pseud_disp=f->GetParameter(2);

    f->SetParameter(1,pseud_mean);
    f->SetParameter(2,pseud_disp);

    fit_start=pseud_mean-pseud_disp*2-10;
    fit_end=pseud_mean+pseud_disp*2+10;

    if(fit_start<0){
      fit_start=1;
    }

    hist->Fit("f","Q","",fit_start,fit_end);
    f->Draw("same");
    canvas->SetLogy(1);
    
    hs->Draw("HIST,same");
    canvas->RedrawAxis();
    legend->SetFillColor(0);
    legend->Draw("same");
    text->SetNDC(1);
    text->Draw();
    
  }
  else{
    hist->Draw();
  }
  canvas->Print(picture_dir+dist_title);

}
TDACDist::TDACDist(){
  dist_type="TDAC_";
  x_axis_title="TDAC [Register Value]";
  dist_title="tune_TDACDist.pdf";
  map_title="tune_TDACMap.pdf";

  based_bin=31;
  based_min=-15.5;
  based_max=15.5;

  hist_minimum=0.9;
  hist_maximum=40000;

  this->declare_hist();
}
void TDACDist::add_histogram(){
  legend=new TLegend(0.55,0.68,0.85,0.88);
  hs_stock.push_back(new TH1F("hist_TDAC_scan_failed","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_TDAC_no_response","",based_bin,based_min,based_max));
  //hs_stock.push_back(new TH1F("hist_TDAC_o3sg_saturated","",based_bin,based_min,based_max));
  //hs_stock.push_back(new TH1F("hist_TDAC_o3sg_not_saturated","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_TDAC_far_from_mean","",based_bin,based_min,based_max));

  legend->SetBorderSize(0);
  legend->AddEntry(hs_stock[0],"Fitting Failed","f");
  legend->AddEntry(hs_stock[1],"Non Responsive","f");
  legend->AddEntry(hs_stock[2],"#left|Thr^{meas}-Thr^{tar}#right|>120e","f");
  //legend->AddEntry(hs_stock[2],"out_saturated");
  //legend->AddEntry(hs_stock[2],"Pixel Reg=15");
  //legend->AddEntry(hs_stock[3],"out_notsaturated");
}
void TDACDist::DrawOption(){
  canvas->SetLogy();
  gStyle->SetOptStat(0);
  legend->SetTextSize(0.04);
  hist->GetXaxis()->SetNdivisions(510);  
}
void TDACDist::sort_value(int Column,int Row,double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double TDAC_via){
  this->value_set(Column,Row,TDAC);
  //double sigma=40;
  double three_sigma=0;
  three_sigma=gaus_sigma*3;
  if(Threshold==0){
    if(Enable==0){
      hs_stock[1]->Fill(TDAC);
    }
    else{
      hs_stock[0]->Fill(TDAC);
    }
  }
  else if(Threshold>=mean+three_sigma||Threshold<=mean-three_sigma){
    if(TDAC==15||TDAC==-15){
      //hs_stock[2]->Fill(TDAC);
      hs_stock[2]->Fill(TDAC);
    }
    else{
      //hs_stock[3]->Fill(TDAC);
      hs_stock[2]->Fill(TDAC);
    }
  }
}

/*void TDACDist::additional_draw(){
  if(makeSinglePlot((int)now_target_threshold)){
    canvas->Print(picture_dir+Form("/TDAC1d%d.pdf",(int)now_target_threshold));
  }
  }*/

TDAC2Dist::TDAC2Dist(){
  dist_type="TDAC";
  x_axis_title="TDAC [Register Value]";
  dist_title="tune_TDAC2Dist.pdf";
  map_title="tune_"+dist_type+"Map.pdf";

  based_bin=31;
  based_min=-15.5;
  based_max=15.5;

  hist_minimum=0.5;
  hist_maximum=40000;

  this->declare_hist();
}
void TDAC2Dist::add_histogram(){
  legend=new TLegend(0.55,0.72,0.88,0.87);
  hs_stock.push_back(new TH1F("hist_TDAC_Thre_0","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_TDAC_far_from_mean","",based_bin,based_min,based_max));

  legend->SetBorderSize(0);
  legend->AddEntry(hs_stock[0],"Thr^{meas}=0","f");
  legend->AddEntry(hs_stock[1],"#left|Thr^{meas}-Thr^{tar}#right|>120e","f");
}
void TDAC2Dist::DrawOption(){
  canvas->SetLogy();
  gStyle->SetOptStat(0);
  legend->SetTextSize(0.04);
  hist->GetXaxis()->SetNdivisions(510);
}
void TDAC2Dist::ColorOption(){
  hs_stock[0]->SetFillColor(1);
  hs_stock[0]->SetFillStyle(3344);
  hs_stock[1]->SetFillColor(4);
}
void TDAC2Dist::sort_value(int Column,int Row,double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double TDAC_via){
  this->value_set(Column,Row,TDAC);
  //double sigma=40;
  double three_sigma=0;
  three_sigma=gaus_sigma*3;
  if(Threshold==0){
    hs_stock[0]->Fill(TDAC);
  }
  else if(Threshold>=mean+three_sigma||Threshold<=mean-three_sigma){
    hs_stock[1]->Fill(TDAC);
  }
}

void TDAC2Dist::additional_draw(){
  if(makeSinglePlot((int)now_target_threshold)){
    canvas->Print(picture_dir+Form("/TDAC1d%d.pdf",(int)now_target_threshold));
  }
}

NoiseDist::NoiseDist(){
  dist_type="Noise";
  x_axis_title="Noise [e]";
  dist_title="tune_"+dist_type+"Dist.pdf";
  map_title="tune_"+dist_type+"Map.pdf";

  based_bin=100;
  based_min=0;
  based_max=500;

  hist_minimum=0.9;
  hist_maximum=20000;

  this->declare_hist();
}/*
void NoiseDist::pdf_open(){
  TString open =picture_dir+dist_title+"[";
  canvas->Print(open);
  open =picture_dir+map_title+"[";
  //canvas->Print(open);
  open=picture_dir+"Core_Noise2DHist.pdf[";
  canvas->Print(open);
}
void NoiseDist::pdf_close(){
  TString close =picture_dir+dist_title+"]";
  canvas->Print(close);
  close =picture_dir+map_title+"]";
  //canvas->Print(close);

  close=picture_dir+"Core_Noise2DHist.pdf]";
  canvas->Print(close);

}
void NoiseDist::additional_init(){
   for(int col8=0;col8<8;col8++){
    for(int row8=0;row8<8;row8++){
      core_noisehist[col8][row8]->Reset();
    }
  }
  }*/
void NoiseDist::add_histogram(){
  //legend=new TLegend(0.32,0.65,0.70,0.88);
  legend=new TLegend(0.55,0.50,0.75,0.87);

  legend->SetBorderSize(0);
  legend->SetTextSize(0.035);

  hs_stock.push_back(new TH1F("hist_Noise_scan_failed","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_Noise_no_response","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_Noise_o3sg_saturated","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_Noise_o3sg_not_saturated","",based_bin,based_min,based_max));

  legend->AddEntry((TObject*)0,"#kern[-0.5]{Thr^{meas}=0}","");
  legend->AddEntry(hs_stock[0],"Fitting failed","f");
  legend->AddEntry(hs_stock[1],"Non Responsive","f");

  legend->AddEntry((TObject*)0,"#kern[-0.2]{#left|Thr^{meas}-Thr^{tar}#right| >120e}","");
  legend->AddEntry(hs_stock[2],"TDAC=15 or -15","f");
  legend->AddEntry(hs_stock[3],"TDAC:other","f");
  //  legend->AddEntry(hs_stock[0],"scan failed");
  //legend->AddEntry(hs_stock[1],"no response");
  //legend->AddEntry(hs_stock[2],"out_saturated");
  //legend->AddEntry(hs_stock[3],"out_notsaturated");
  /*
  for(int col8=0;col8<8;col8++){
    for(int row8=0;row8<8;row8++){
      core_noisehist[col8][row8]=new TH1F(Form("NoiseDist_%d_%d",col8,row8),"Noise;Noise [e];# pixels",100,0,200);
    }
    }*/
}
void NoiseDist::sort_value(int Column,int Row,double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double TDAC_via){
  this->value_set(Column,Row,Noise);
  //double sigma=40;
  double three_sigma=0;
  three_sigma=gaus_sigma*3;

  //core_noisehist[Column%8][Row%8]->Fill(Noise);

  if(Threshold==0){
    if(Enable==0){
      hs_stock[1]->Fill(Noise);
    }
    else{
      hs_stock[0]->Fill(Noise);
    }
  }
  else if(Threshold>=mean+three_sigma||Threshold<=mean-three_sigma){
    if(TDAC==15||TDAC==-15){
      hs_stock[2]->Fill(Noise);
    }
    else{
      hs_stock[3]->Fill(Noise);
    }
  }
}/*
void NoiseDist::additional_draw(){
  TCanvas*NoiseCoreCanvas=new TCanvas("NosieCoreCanvas","");
  TH2F*NoiseCore2d=new TH2F("NoiseCore2d",Form("Core_Noise2Dhist_%d;Column;Row",(int)now_target_threshold),8,0,8,8,0,8);
  //double NoiseMean[core_col][core_row]={{0}};
  for(int col8=0;col8<8;col8++){
    for(int row8=0;row8<8;row8++){
      //NoiseMean[col8][row8]=core_noisehist[col8][row8]->GetMean();
      NoiseCore2d->Fill(col8,row8,core_noisehist[col8][row8]->GetMean());
    }
  }
  NoiseCore2d->Draw("text45 colz");
  TString pdf=picture_dir+"Core_Noise2DHist.pdf";
  NoiseCoreCanvas->Print(pdf);

  }*/

Threshold_viaDist::Threshold_viaDist(){
  dist_type="Threshold_via";
  x_axis_title="Threshold [e]";
  dist_title="tune_"+dist_type+"Dist.pdf";
  map_title="tune_"+dist_type+"Map.pdf";

  based_bin=200;
  based_min=0;
  based_max=4000;

  hist_minimum=0.9;
  hist_maximum=15000;

  this->declare_hist();
}
void Threshold_viaDist::add_histogram(){
  legend=new TLegend(0.73,0.38,0.98,0.58);
  hs_stock.push_back(new TH1F("hist_threvia_o3sg_15","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_threvia_o3sg_m15","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_threvia_o3sg_not_saturated","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_threvia_no_response","",based_bin,based_min,based_max));

  legend->AddEntry(hs_stock[0],"TDAC==15 (after)");
  legend->AddEntry(hs_stock[1],"TDAC==-15(after)");
  legend->AddEntry(hs_stock[2],"not saturated(after)");
  legend->AddEntry(hs_stock[3],"no response(after)");

}
void Threshold_viaDist::ColorOption(){
  hs_stock[0]->SetFillColor(6);
  hs_stock[1]->SetFillColor(4);
  hs_stock[2]->SetFillColor(5);
  hs_stock[3]->SetFillColor(3);
}
void Threshold_viaDist::sort_value(int Column,int Row,double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double TDAC_via){
  this->value_set(Column,Row,Threshold_via);
  //double sigma=40;
  double three_sigma=0;
  three_sigma=gaus_sigma*3;
  if(Enable==0){
    hs_stock[3]->Fill(Threshold_via);
  }
  else if(TDAC==15){
    hs_stock[0]->Fill(Threshold_via);
  }
  else if(TDAC==-15){
    hs_stock[1]->Fill(Threshold_via);
  }
  else if(Threshold>=mean+three_sigma||Threshold<=mean-three_sigma){
    hs_stock[2]->Fill(Threshold_via);
  }
}
Noise_viaDist::Noise_viaDist(){
  dist_type="Noise_via";
  dist_title="tune_"+dist_type+"Dist.pdf";
  map_title="tune_"+dist_type+"Map.pdf";

  based_bin=100;
  based_min=0;
  based_max=500;

  hist_minimum=0.9;
  hist_maximum=15000;

  this->declare_hist();
}
void Noise_viaDist::add_histogram(){
  legend=new TLegend(0.73,0.38,0.98,0.58);
  hs_stock.push_back(new TH1F("hist_Noise_via_scan_failed","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_Noise_via_no_response","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_Noise_via_o3sg_saturated","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_Noise_via_o3sg_not_saturated","",based_bin,based_min,based_max));

  legend->AddEntry(hs_stock[0],"scan failed");
  legend->AddEntry(hs_stock[1],"no response");
  legend->AddEntry(hs_stock[2],"out_saturated");
  legend->AddEntry(hs_stock[3],"out_notsaturated");

}
void Noise_viaDist::sort_value(int Column,int Row,double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double TDAC_via){
  this->value_set(Column,Row,Noise_via);
  //double sigma=40;
  double three_sigma=0;
  three_sigma=gaus_sigma*3;
  if(Threshold==0){
    if(Enable==0){
      hs_stock[1]->Fill(Noise_via);
    }
    else{
      hs_stock[0]->Fill(Noise_via);
    }
  }
  else if(Threshold>=mean+three_sigma||Threshold<=mean-three_sigma){
    if(TDAC==15||TDAC==-15){
      hs_stock[2]->Fill(Noise_via);
    }
    else{
      hs_stock[3]->Fill(Noise_via);
    }
  }
}

LowerLimitDist::LowerLimitDist(){
  number=0;
  this->FirstInit();
}
LowerLimitDist::LowerLimitDist(int num){
  number=num;
  this->FirstInit();
}
void LowerLimitDist::init(int target){
  TString mk_dat="mkdir -p "+picture_dir+"/dat";
  std::system(mk_dat);
  now_target_threshold=target;
  char fname[256];
  sprintf(fname,picture_dir+"/dat/outof3Sigma_%d.dat",target);
  ofs.open(fname);
}
void LowerLimitDist::FirstInit(){
  dist_type=Form("LowerLimit%d",number);
  //TH1F_dist_title="The ratio of the untuned pixel";
  
  //dist_title="tune_"+dist_type+"Dist.pdf";
  dist_title="tune_"+dist_type+"Dist.pdf";
  map_title="tune_"+dist_type+"Map.pdf";

  based_bin=101;
  based_min=-10;
  based_max=2010;

  hist_minimum=0.9;
  hist_maximum=15000;

  totalentry=0;
  typical_target=-1;

}
void LowerLimitDist::declare_hist(){
  hist=new TH1F(dist_type+Form("_1d_%d",number),"",based_bin,based_min,based_max);

  hist->SetMinimum(hist_minimum);
  canvas=new TCanvas(dist_type+Form("_c_%d",number),"");
  hs=new THStack("hs",dist_type+";target_threshold [e];The ratio of untuned pixels");

  hist->SetTitle("The ratio of the untuned pixel;target_threshold [e];The ratio of untuned pixels");
  /*
  hist->SetTitleSize(0.045,"x");
  hist->SetTitleSize(0.045,"y");
  hist->SetTitleOffset(0.7);
  hist->GetXaxis()->SetTitleOffset(0.85);
  hist->GetYaxis()->SetTitleOffset(0.85);
  */
}
void LowerLimitDist::pdf_open(){
  TString open =picture_dir+dist_title+"[";
  canvas->Print(open);
  open =picture_dir+map_title+"[";

  if(number==0){
    for (unsigned it=0; it<hist2D_vec.size(); it++){
      canvas->Print(picture_dir+Form("LowerLimit_2DHist%d_%d.pdf[",number,it));
    }
  }
}
void LowerLimitDist::pdf_close(){
  TString close =picture_dir+dist_title+"]";
  canvas->Print(close);
  close =picture_dir+map_title+"]";
  //canvas->Print(close);
  if(number==0){
    for (unsigned it=0; it<hist2D_vec.size(); it++){
      canvas->Print(picture_dir+Form("LowerLimit_2DHist%d_%d.pdf]",number,it));
    }
  }

}
void LowerLimitDist::value_set(int col, int row, double value){
  hist->Fill(value);
}

void LowerLimitDist::add_histogram(){
  this->declare_hist();
  double x_left=0;
  double y_left=0;
  double x_right=0;
  double y_right=0;
  double text_size=0;
  if(number==0){
    x_left=0.35;
    y_left=0.65;
    x_right=0.87;
    y_right=0.87;
    text_size=0.04;
  }
  else{
    x_left=0.50;
    y_left=0.70;
    x_right=0.85;
    y_right=0.87;
    text_size=0.045;
  }
  
  legend=new TLegend(x_left,y_left,x_right,y_right);
  legend->SetBorderSize(0);
  legend->SetTextSize(text_size);

  //legend=new TLegend(0.40,0.55,0.85,0.87);
  if (number==0){
    hs_stock.push_back(new TH1F(Form("hist_non_responsive_%d",number),"no response;target threshold; The ratio of untuned pixels",based_bin,based_min,based_max));
    hs_stock.push_back(new TH1F(Form("hist_scan_failed_%d",number),"scan_failed;target threshold; The ratio of untuned pixels",based_bin,based_min,based_max));
    hs_stock.push_back(new TH1F(Form("hist_o3sg_TDAC15_%d",number),"out of 3sigma with TDAC=15;target threshold; The ratio of untuned pixels",based_bin,based_min,based_max));
    hs_stock.push_back(new TH1F(Form("hist_o3sg_TDACm15_%d",number),"out of 3sigma with TDAC=-15;target threshold; The ratio of untuned pixels",based_bin,based_min,based_max));
    hs_stock.push_back(new TH1F(Form("hist_o3sg_not_saturated_%d",number),"out of 3sigma with not saturated TDAC;target threshold; The ratio of untuned pixels",based_bin,based_min,based_max));
        
    legend->SetNColumns(2);
    legend->AddEntry((TObject*)0,"#kern[-0.5]{Thr^{meas}=0}","");
    legend->AddEntry((TObject*)0,"#kern[-0.2]{#left|Thr^{meas}-Thr^{tar}#right| >120e}","");
    
    legend->AddEntry(hs_stock[0],"Non-Responsive","f");
    legend->AddEntry(hs_stock[2],"TDAC=15","f");
    
    legend->AddEntry(hs_stock[1],"Fitting Failed","f");
    legend->AddEntry(hs_stock[3],"TDAC=-15","f");
    
    legend->AddEntry((TObject*)0,"","");
    legend->AddEntry(hs_stock[4],"TDAC:other","f");
  }
  else {
    hs_stock.push_back(new TH1F(Form("hist_Thre_0_%d",number),"Thr^{meas}=0e;target threshold; The ratio of untuned pixels",based_bin,based_min,based_max));
    hs_stock.push_back(new TH1F(Form("hist_Thre_120_%d",number),"#left|Thr^{meas}-Thr^{tar}#right|>120e;target threshold; The ratio of untuned pixels",based_bin,based_min,based_max));
        
    legend->AddEntry(hs_stock[0],"Thr^{meas}=0","f");
    legend->AddEntry(hs_stock[1],"#left|Thr^{meas}-Thr^{tar}#right| >120e","f");
        
  }
  //
  /*
  legend->AddEntry((TObject*)0,"#kern[-0.5]{untuned pixel}","");
  legend->AddEntry(hs_stock[0],"Non-Responsive (Thr^{meas}=0)","f");
  legend->AddEntry(hs_stock[1],"Fitting Failed (Thr^{meas}=0)","f");
  legend->AddEntry(hs_stock[2],"#left|Thr^{meas}-Thr^{tar}#right| >120e","f");
  */
  
  int diff_start_col=264;
  int diff_end_col=450;
  int diff_bin=diff_end_col-diff_start_col;
  
  hist2D_vec.push_back(new TH2F(Form("hist2d_non_responsive_%d",number),"non_responsive;Column;Row",diff_bin,diff_start_col,diff_end_col,192,0,192));
  hist2D_vec.push_back(new TH2F(Form("hist2d_scan_failed_%d",number),"scan_failed;Column;Row",diff_bin,diff_start_col,diff_end_col,192,0,192));
  hist2D_vec.push_back(new TH2F(Form("hist2d_TDAC15_%d",number),"TDAC15;Column;Row",diff_bin,diff_start_col,diff_end_col,192,0,192));
  hist2D_vec.push_back(new TH2F(Form("hist2d_TDACm15_%d",number),"TDACm15;Column;Row",diff_bin,diff_start_col,diff_end_col,192,0,192));
  hist2D_vec.push_back(new TH2F(Form("hist2d_not_saturated_%d",number),"not_saturated;Column;Row",diff_bin,diff_start_col,diff_end_col,192,0,192));
  hist2D_vec.push_back(new TH2F(Form("hist2d_total_%d",number),"total;Column;Row",diff_bin,diff_start_col,diff_end_col,192,0,192));
  
  hist2D_vec.push_back(new TH2F(Form("hist2d_non_responsive_%d_Core",number),"non_responsive;Column;Row",10,0,10,8,0,8));
  hist2D_vec.push_back(new TH2F(Form("hist2d_scan_failed_%d_Core",number),"scan_failed;Column;Row",10,0,10,8,0,8));
  hist2D_vec.push_back(new TH2F(Form("hist2d_TDAC15_%d_Core",number),"TDAC15;Column;Row",10,0,10,8,0,8));
  hist2D_vec.push_back(new TH2F(Form("hist2d_TDACm15_%d_Core",number),"TDACm15;Column;Row",10,0,10,8,0,8));
  hist2D_vec.push_back(new TH2F(Form("hist2d_not_saturated_%d_Core",number),"not_saturated;Column;Row",10,0,10,8,0,8));

  
}
void LowerLimitDist::DrawOption(){
  gStyle->SetOptStat(0);
}
void LowerLimitDist::ColorOption(){
  if(number==0){
    hs_stock[0]->SetFillColor(3);
    hs_stock[1]->SetFillColor(2);
    hs_stock[2]->SetFillColor(4);
    hs_stock[3]->SetFillColor(6);
    hs_stock[4]->SetFillColor(5);
  }
  else{
    hs_stock[0]->SetFillColor(2);
    hs_stock[1]->SetFillColor(4);    
  }
  // to make a plot simple
  //hs_stock[3]->SetFillColor(4);
  //hs_stock[4]->SetFillColor(4);
}
void LowerLimitDist::sort_value(int Column,int Row,double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double TDAC_via){
  if(typical_target==-1){
    typical_target=now_target_threshold;
  }
  
  if(typical_target!=-1&&typical_target==now_target_threshold){
    totalentry++;
  }

  int hs_num_nonRes=0;
  int hs_num_fitfail=0;
  int hs_num_TDAC_15=0;
  int hs_num_TDAC_m15=0;
  int hs_num_TDAC_other=0;
  if(number==0){
    hs_num_nonRes=0;
    hs_num_fitfail=1;
    hs_num_TDAC_15=2;
    hs_num_TDAC_m15=3;
    hs_num_TDAC_other=4;
  }
  else{
    hs_num_nonRes=0;
    hs_num_fitfail=0;
    hs_num_TDAC_15=1;
    hs_num_TDAC_m15=1;
    hs_num_TDAC_other=1;
  }

  //double sigma=40;
  double three_sigma=0;
  double target_threshold=now_target_threshold;
  three_sigma=gaus_sigma*3;

  if(Threshold!=0&&Threshold<=mean+three_sigma&&Threshold>=mean-three_sigma){}
  else{
    hist2D_vec[5]->Fill(Column,Row,1);
    this->value_set(Column,Row,target_threshold);
    if(Threshold==0){
      if(Enable==0){
	hs_stock[hs_num_nonRes]->Fill(target_threshold);
	hist2D_vec[0]->Fill(Column,Row,1);
	hist2D_vec[6]->Fill(Column%8,Row%8,1);
      }
      else{
	hs_stock[hs_num_fitfail]->Fill(target_threshold);
	hist2D_vec[1]->Fill(Column,Row,1);
	hist2D_vec[7]->Fill(Column%8,Row%8,1);
      }
    }
    else{
      ofs << "Column = " << Column << " , Row = " << Row <<"  \t, TDAC = " << TDAC << "  \t, Threshold = " << Threshold << "\n";
      if(TDAC==15){
	hs_stock[hs_num_TDAC_15]->Fill(target_threshold);
	hist2D_vec[2]->Fill(Column,Row,1);
	hist2D_vec[8]->Fill(Column%8,Row%8,1);
      }
      else if(TDAC==-15){
	hs_stock[hs_num_TDAC_m15]->Fill(target_threshold);
	//hs_stock[2]->Fill(target_threshold);
	hist2D_vec[3]->Fill(Column,Row,1);
	hist2D_vec[9]->Fill(Column%8,Row%8,1);
      }
      else{
	hs_stock[hs_num_TDAC_other]->Fill(target_threshold);
	//hs_stock[2]->Fill(target_threshold);
	hist2D_vec[4]->Fill(Column,Row,1);
	hist2D_vec[10]->Fill(Column%8,Row%8,1);
      }
    }
  }
}
void LowerLimitDist::Draw(){
  canvas->cd();
  canvas->SetLogy(0);
  if(number==0){
    //std::cout << number << std::endl;
    gStyle->SetOptStat("eioun");
    for (unsigned it=0; it<hist2D_vec.size(); it++){
      if (it<6){
	hist2D_vec[it]->SetMaximum(2);
      }
      hist2D_vec[it]->SetName(Form("Map_%f_%d",now_target_threshold,it));
      hist2D_vec[it]->Draw("colz");
      canvas->Print(picture_dir+Form("LowerLimit_2DHist%d_%d.pdf",number,it));
      hist2D_vec[it]->Reset();
    }
    gStyle->SetOptStat("eMinoru");
  }
  ofs.close();
}
void LowerLimitDist::resultDraw(){
  this->DrawOption();
  this->ColorOption();
  canvas->cd();
  canvas->SetLogy();
  //  hist->SetMaximum(totalentry*2);
  hist->Scale(1.0/totalentry);

  hist->SetMaximum(10);
  //  hist->SetMinimum(0.00001);
  hist->SetMinimum(0.5/totalentry);
  hist->GetXaxis()->SetNdivisions(50505);

  if(hs_stock.size()>0){
    for(unsigned it=0;it<hs_stock.size();it++){
      hs_stock[it]->Scale(1.0/totalentry);
      hs_stock[it]->GetXaxis()->SetNdivisions(50505);
      hs->Add(hs_stock[it]);
    }
  }
  canvas->cd();
  if(hs_stock.size()>0){
    std::cout << picture_dir+dist_title << std::endl;
    hist->Draw("hist");
    hs->Draw("HIST,same");
    legend->Draw("same");
  }
  else{
    hist->Draw();
  }
  canvas->RedrawAxis();
  canvas->Print(picture_dir+dist_title);

  if(hs_stock.size()>0){
    for(unsigned it=0;it<hs_stock.size();it++){
      hs_stock[it]->SetMaximum(2);
      hs_stock[it]->SetMinimum(0.5/totalentry);

      hs_stock[it]->Draw("hist");
      canvas->Print(picture_dir+dist_title);
    }
  }

}
void LowerLimitDist::SetTitle(TString str){
  hist->SetTitle(str);
}
void LowerLimitDist::SetNumber(unsigned num){
  dist_type=dist_type+Form("%d",num);
  number=num;
}

Threshold_via2Dist::Threshold_via2Dist(){
  dist_type="Threshold_via_2";
  x_axis_title="Threshold [e]";
  dist_title="tune_"+dist_type+"Dist.pdf";
  map_title="tune_"+dist_type+"Map.pdf";

  based_bin=200;
  based_min=0;
  based_max=4000;

  hist_minimum=0.9;
  hist_maximum=15000;

  this->declare_hist();
}
void Threshold_via2Dist::add_histogram(){
  legend=new TLegend(0.73,0.38,0.98,0.58);
  hs_stock.push_back(new TH1F("hist_threvia2_o3sg_15","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_threvia2_o3sg_m15","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_threvia2_o3sg_not_saturated","",based_bin,based_min,based_max));

  legend->AddEntry(hs_stock[0],"TDAC==15 (init)");
  legend->AddEntry(hs_stock[1],"TDAC==-15(init)");
  legend->AddEntry(hs_stock[2],"not saturated(init)");

}
void Threshold_via2Dist::ColorOption(){
  hs_stock[0]->SetFillColor(6);
  hs_stock[1]->SetFillColor(4);
  hs_stock[2]->SetFillColor(5);
}
void Threshold_via2Dist::sort_value(int Column,int Row,double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double TDAC_via){
  this->value_set(Column,Row,Threshold_via);
  //double sigma=40;
  double three_sigma=0;

  mean=800;
  three_sigma=gaus_sigma*3;
  if(TDAC_via==15){
    hs_stock[0]->Fill(Threshold_via);
  }
  else if(TDAC_via==-15){
    hs_stock[1]->Fill(Threshold_via);
  }
  else if(Threshold_via>=mean+three_sigma||Threshold_via<=mean-three_sigma){
    hs_stock[2]->Fill(Threshold_via);
  }
}
TDAC_viaDist::TDAC_viaDist(){
  dist_type="TDAC_via";
  x_axis_title="TDAC [bit]";
  dist_title="tune_"+dist_type+"Dist.pdf";
  map_title="tune_"+dist_type+"Map.pdf";

  based_bin=35;
  based_min=-15;
  based_max=20;

  hist_minimum=0.9;
  hist_maximum=15000;

  this->declare_hist();
}
void TDAC_viaDist::add_histogram(){
  legend=new TLegend(0.73,0.68,0.98,0.98);
  hs_stock.push_back(new TH1F("hist_TDACvia_scan_failed","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_TDACvia_no_response","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_TDACvia_o3sg_saturated","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_TDACvia_o3sg_not_saturated","",based_bin,based_min,based_max));

  legend->AddEntry(hs_stock[0],"scan failed (after)");
  legend->AddEntry(hs_stock[1],"no response (after)");
  legend->AddEntry(hs_stock[2],"out_saturated (after)");
  legend->AddEntry(hs_stock[3],"out_notsaturated (after)");
}
void TDAC_viaDist::DrawOption(){
  canvas->SetLogy();
  gStyle->SetOptStat(0);
}
void TDAC_viaDist::sort_value(int Column,int Row,double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double TDAC_via){
  this->value_set(Column,Row,TDAC_via);
  //double sigma=40;
  double three_sigma=0;
  three_sigma=gaus_sigma*3;
  if(Threshold==0){
    if(Enable==0){
      hs_stock[1]->Fill(TDAC_via);
    }
    else{
      hs_stock[0]->Fill(TDAC_via);
    }
  }
  else if(Threshold>=mean+three_sigma||Threshold<=mean-three_sigma){
    if(TDAC==15||TDAC==-15){
      hs_stock[2]->Fill(TDAC_via);
    }
    else{
      hs_stock[3]->Fill(TDAC_via);
    }
  }
}

DiffTDACDist::DiffTDACDist(){
  dist_type="DiffTDAC";
  x_axis_title="#Delta TDAC [bit]";
  dist_title="tune_"+dist_type+"Dist.pdf";
  map_title="tune_"+dist_type+"Map.pdf";

  //based_bin=65;
  //based_min=-30;
  //based_max=35;

  based_bin=20;
  based_min=-10;
  based_max=10;

  hist_minimum=0.9;
  hist_maximum=50000;

  this->declare_hist();
}
void DiffTDACDist::add_histogram(){
  legend=new TLegend(0.73,0.68,0.98,0.98);
  hs_stock.push_back(new TH1F("hist_dTDAC_scan_failed","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_dTDAC_no_response","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_dTDAC_o3sg_TDAC15","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_dTDAC_o3sg_TDACm15","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_dTDAC_o3sg_not_saturated","",based_bin,based_min,based_max));

  legend->AddEntry(hs_stock[0],"scan failed (after)");
  legend->AddEntry(hs_stock[1],"no response (after)");
  legend->AddEntry(hs_stock[2],"TDAC==15 (after)");
  legend->AddEntry(hs_stock[3],"TDAC==-15 (after)");
  legend->AddEntry(hs_stock[4],"out_notsaturated (after)");
}
void DiffTDACDist::ColorOption(){
  hs_stock[0]->SetFillColor(2);
  hs_stock[1]->SetFillColor(3);
  hs_stock[2]->SetFillColor(6);
  hs_stock[3]->SetFillColor(4);
  hs_stock[4]->SetFillColor(5);

}
void DiffTDACDist::DrawOption(){
  canvas->SetLogy();
  gStyle->SetOptStat(0);
}
void DiffTDACDist::sort_value(int Column,int Row,double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double TDAC_via){
  this->value_set(Column,Row,TDAC-TDAC_via);
  //double sigma=40;
  double three_sigma=0;
  three_sigma=gaus_sigma*3;
  if(Threshold==0){
    if(Enable==0){
      hs_stock[1]->Fill(TDAC-TDAC_via);
    }
    else{
      hs_stock[0]->Fill(TDAC-TDAC_via);
    }
  }
  else if(Threshold>=mean+three_sigma||Threshold<=mean-three_sigma){
    if(TDAC==15){
      hs_stock[2]->Fill(TDAC-TDAC_via);
    }
    else if (TDAC==-15){
      hs_stock[3]->Fill(TDAC-TDAC_via);
    }
    else{
      hs_stock[4]->Fill(TDAC-TDAC_via);
    }
  }
}
DiffThresholdDist::DiffThresholdDist(){
  dist_type="DiffThreshold";
  x_axis_title="#Delta Threshold [e]";
  dist_title="tune_"+dist_type+"Dist.pdf";
  map_title="tune_"+dist_type+"Map.pdf";

  based_bin=150;
  based_min=-1500;
  based_max=3000;

  hist_minimum=0.9;
  hist_maximum=15000;

  this->declare_hist();
}
void DiffThresholdDist::add_histogram(){
  legend=new TLegend(0.73,0.68,0.98,0.98);
  hs_stock.push_back(new TH1F("hist_dthre_scan_failed","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_dthre_no_response","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_dthre_o3sg_saturated","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_dthre_o3sg_not_saturated","",based_bin,based_min,based_max));

  legend->AddEntry(hs_stock[0],"scan failed (after)");
  legend->AddEntry(hs_stock[1],"no response (after)");
  legend->AddEntry(hs_stock[2],"out_saturated (after)");
  legend->AddEntry(hs_stock[3],"out_notsaturated (after)");
}
void DiffThresholdDist::DrawOption(){
  canvas->SetLogy();
  gStyle->SetOptStat(0);
}
void DiffThresholdDist::sort_value(int Column,int Row,double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double TDAC_via){
  this->value_set(Column,Row,Threshold-Threshold_via);
  //double sigma=40;
  double three_sigma=0;
  three_sigma=gaus_sigma*3;
  if(Threshold==0){
    if(Enable==0){
      hs_stock[1]->Fill(Threshold-Threshold_via);
    }
    else{
      hs_stock[0]->Fill(Threshold-Threshold_via);
    }
  }
  else if(Threshold>=mean+three_sigma||Threshold<=mean-three_sigma){
    if(TDAC==15||TDAC==-15){
      hs_stock[2]->Fill(Threshold-Threshold_via);
    }
    else{
      hs_stock[3]->Fill(Threshold-Threshold_via);
    }
  }
}
DiffNoiseDist::DiffNoiseDist(){
  dist_type="DiffNoise";
  x_axis_title="#Delta Noise [e]";
  dist_title="tune_"+dist_type+"Dist.pdf";
  map_title="tune_"+dist_type+"Map.pdf";

  based_bin=100;
  based_min=-300;
  based_max=300;

  hist_minimum=0.9;
  hist_maximum=15000;

  this->declare_hist();
}
void DiffNoiseDist::add_histogram(){
  legend=new TLegend(0.73,0.68,0.98,0.98);
  hs_stock.push_back(new TH1F("hist_dnoise_scan_failed","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_dnoise_no_response","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_dnoise_o3sg_saturated","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_dnoise_o3sg_not_saturated","",based_bin,based_min,based_max));

  legend->AddEntry(hs_stock[0],"scan failed (after)");
  legend->AddEntry(hs_stock[1],"no response (after)");
  legend->AddEntry(hs_stock[2],"out_saturated (after)");
  legend->AddEntry(hs_stock[3],"out_notsaturated (after)");
}
void DiffNoiseDist::DrawOption(){
  canvas->SetLogy();
  gStyle->SetOptStat(0);
}
void DiffNoiseDist::sort_value(int Column,int Row,double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double TDAC_via){
  this->value_set(Column,Row,Noise-Noise_via);
  //double sigma=40;
  double three_sigma=0;
  three_sigma=gaus_sigma*3;
  if(Threshold==0){
    if(Enable==0){
      hs_stock[1]->Fill(Noise-Noise_via);
    }
    else{
      hs_stock[0]->Fill(Noise-Noise_via);
    }
  }
  else if(Threshold>=mean+three_sigma||Threshold<=mean-three_sigma){
    if(TDAC==15||TDAC==-15){
      hs_stock[2]->Fill(Noise-Noise_via);
    }
    else{
      hs_stock[3]->Fill(Noise-Noise_via);
    }
  }
}
DiffNoise2Dist::DiffNoise2Dist(){
  dist_type="DiffNoise2";
  dist_title="tune_"+dist_type+"Dist.pdf";
  map_title="tune_"+dist_type+"Map.pdf";

  based_bin=100;
  based_min=-300;
  based_max=300;

  hist_minimum=0.9;
  hist_maximum=15000;

  this->declare_hist();
}
void DiffNoise2Dist::add_histogram(){
  legend=new TLegend(0.73,0.68,0.98,0.98);
  hs_stock.push_back(new TH1F("hist_dnoise_over100","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_dnoise_over200","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_dnoise_over300","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_dnoise_over400","",based_bin,based_min,based_max));

  legend->AddEntry(hs_stock[0],"Noise(after) > 100");
  legend->AddEntry(hs_stock[1],"Noise(after) > 200");
  legend->AddEntry(hs_stock[2],"Noise(after) > 300");
  legend->AddEntry(hs_stock[3],"Noise(after) > 400");
}
void DiffNoise2Dist::DrawOption(){
  canvas->SetLogy();
  gStyle->SetOptStat(0);
}
void DiffNoise2Dist::sort_value(int Column,int Row,double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double TDAC_via){
  this->value_set(Column,Row,Noise-Noise_via);
  if(Noise>400){
    hs_stock[3]->Fill(Noise-Noise_via);
  }
  else if(Noise>300){
    hs_stock[2]->Fill(Noise-Noise_via);
  }
  else if(Noise>200){
    hs_stock[1]->Fill(Noise-Noise_via);
  }
  else if(Noise>100){
    hs_stock[0]->Fill(Noise-Noise_via);
  }  
}
Noise_via2Dist::Noise_via2Dist(){
  dist_type="Noise_via2";
  x_axis_title="Noise [e]";
  dist_title="tune_"+dist_type+"Dist.pdf";
  map_title="tune_"+dist_type+"Map.pdf";

  based_bin=100;
  based_min=0;
  based_max=500;

  hist_minimum=0.9;
  hist_maximum=15000;

  this->declare_hist();
}
void Noise_via2Dist::add_histogram(){
  legend=new TLegend(0.73,0.38,0.98,0.58);

  hs_stock.push_back(new TH1F("hist_Noise_via2_over100","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_Noise_via2_over200","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_Noise_via2_over300","",based_bin,based_min,based_max));
  hs_stock.push_back(new TH1F("hist_Noise_via2_over400","",based_bin,based_min,based_max));

  legend->AddEntry(hs_stock[0],"Noise(after) > 100");
  legend->AddEntry(hs_stock[1],"Noise(after) > 200");
  legend->AddEntry(hs_stock[2],"Noise(after) > 300");
  legend->AddEntry(hs_stock[3],"Noise(after) > 400");


}
void Noise_via2Dist::sort_value(int Column,int Row,double mean,double Threshold,double Noise, double TDAC, double Enable,double Threshold_via,double Noise_via, double TDAC_via){
  this->value_set(Column,Row,Noise_via);
  if(Noise>400){
    hs_stock[3]->Fill(Noise_via);
  }
  else if(Noise>300){
    hs_stock[2]->Fill(Noise_via);
  }
  else if(Noise>200){
    hs_stock[1]->Fill(Noise_via);
  }
  else if(Noise>100){
    hs_stock[0]->Fill(Noise_via);
  }  
}


#endif
