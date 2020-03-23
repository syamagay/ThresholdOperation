#define tune_evaluation_cxx
#include "tune_evaluation.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <vector>
#include <unistd.h>

#define graph_upper 2000

void tune_evaluation::Loop()
{
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

   const int hist2d_num=3;
   // Threshold, TDAC, Threshold_vs_d_Vth,
   TH2F*hist2d[hist2d_num];
   TCanvas*h2d_c[hist2d_num];
   TString hist2d_title[hist2d_num]={"Threshold_vs_target;target_threshold[e];Threshold[e]",
				     "TDAC_vs_target;target_threshold[e];TDAC",
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

   for(Int_t i_hist2d;i_hist2d<hist2d_num;i_hist2d++){
     //X-axis : target_Threshold Status
     if(hist2d_min[i_hist2d]==0){
       hist2d_min[i_hist2d]=0;
     }
     if(hist2d_max[i_hist2d]==0){
       hist2d_max[i_hist2d]=1560;
     }

     if(hist2d_bin[i_hist2d]==0){
       hist2d_bin[i_hist2d]=51;
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
     h2d_c[i_hist2d]=new TCanvas(Form("2d_c_%d",i_hist2d),"");
   }
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   std::vector<double> target_threshold_vec;
   std::vector<double> delta_Vth;
   std::vector<double> threshold_mean;
   std::vector<double> threshold_sigma;
   std::vector<double> threshold_mean_e;
   std::vector<double> threshold_sigma_e;

   std::vector<double> noise_mean;
   std::vector<double> noise_sigma;
   std::vector<double> fit_mis_pixels;
   std::vector<double> Enable_0_pixels;
   std::vector<double> out_of_3sigma;
   std::vector<double> out_of_3sigma_except_0;

   int totalEntry=0;
   int tunable_pixels=0;
   int start_bin=0;
   int end_bin=0;
   int tmp_zero_pixels=0;
   int tmp_mask_pixels=0;
   double tmp_thre_mean=0;
   double tmp_thre_sigma=0;
   double tmp_thre_mean_e=0;
   double tmp_thre_sigma_e=0;

   double tmp_noise_mean=0;
   double tmp_noise_sigma=0;

   TF1*f=new TF1("f","gaus",0,3000);
   int gentry=0;

   TH1F* tmp_thre;
   TH1F* tmp_noise;
   TH1F* tmp_TDAC;
   TH1F* tmp_TDAC_scanmiss;
   TH1F* tmp_TDAC_masked;
   TH1F* tmp_TDAC_out_of_range;
   TH1F* tmp_noise_scanmiss;
   TH1F* tmp_noise_masked;
   TH1F* tmp_noise_out_of_range;

   TCanvas* tmp_c=new TCanvas("tmp_c","");

   TH1F* tmp_thre_TDAC_15;
   TH1F* tmp_thre_TDAC_m15;

   double threshold_value[400][192]={{0}};
   double TDAC_value[400][192]={{0}};
   double noise_value[400][192]={{0}};
   double Enable_value[400][192]={{0}};

   tmp_thre=new TH1F("tmp_thre","",100,0,3000);
   tmp_thre_TDAC_15=new TH1F("tmp_thre_TDAC_15","",100,0,3000);
   tmp_thre_TDAC_m15=new TH1F("tmp_thre_TDAC_m15","",100,0,3000);

   tmp_thre_TDAC_15->SetFillColor(2);
   tmp_thre_TDAC_m15->SetFillColor(3);

   TLegend* tmp_legend=new TLegend(0.75,0.38,0.95,0.58);
   tmp_legend->AddEntry(tmp_thre_TDAC_15,"TDAC==15");
   tmp_legend->AddEntry(tmp_thre_TDAC_m15,"TDAC==-15");
   
   tmp_noise=new TH1F("tmp_noise","",50,0,500);
   tmp_noise_scanmiss=new TH1F("tmp_noise_scanmiss","",50,0,500);
   tmp_noise_masked=new TH1F("tmp_noise_masked","",50,0,500);
   tmp_noise_out_of_range=new TH1F("tmp_noise_out_of_range","",50,0,500);
   tmp_noise_scanmiss->SetFillColor(2);
   tmp_noise_masked->SetFillColor(3);
   tmp_noise_out_of_range->SetFillColor(4);

   TLegend* noise_legend=new TLegend(0.15,0.58,0.45,0.88);
   noise_legend->AddEntry(tmp_noise_scanmiss,"scan failed","f");
   noise_legend->AddEntry(tmp_noise_masked,"no responce","f");
   noise_legend->AddEntry(tmp_noise_out_of_range,"out_of_3sigma","f");
   noise_legend->AddEntry(tmp_noise,"total","l");

   tmp_TDAC=new TH1F("tmp_TDAC","",31,-15,16);
   tmp_TDAC->SetMinimum(0.9);
   tmp_TDAC_scanmiss=new TH1F("tmp_TDAC_scanmiss","",31,-15,16);
   tmp_TDAC_masked=new TH1F("tmp_TDAC_masked","",31,-15,16);
   tmp_TDAC_out_of_range=new TH1F("tmp_TDAC_out_of_range","",31,-15,16);
   tmp_TDAC_scanmiss->SetFillColor(2);
   tmp_TDAC_masked->SetFillColor(3);
   tmp_TDAC_out_of_range->SetFillColor(4);

   tmp_c->Print("tune_evaluation_ThresholdMap.pdf[");
   tmp_c->Print("tune_evaluation_NoiseMap.pdf[");
   tmp_c->Print("tune_evaluation_NoiseMap_detail.pdf[");
   tmp_c->Print("tune_evaluation_TDACMap.pdf[");

   TLegend* TDAC_legend=new TLegend(0.15,0.58,0.45,0.88);
   TDAC_legend->AddEntry(tmp_TDAC_scanmiss,"scan failed","f");
   TDAC_legend->AddEntry(tmp_TDAC_masked,"no responce","f");
   TDAC_legend->AddEntry(tmp_TDAC_out_of_range,"out_of_3sigma","f");
   TDAC_legend->AddEntry(tmp_TDAC,"total","l");

   double totalpixel=0;
   double sigma_value=0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if(Column==0&&Row==0){
	target_threshold_vec.push_back(target_threshold);
	delta_Vth.push_back(DiffVth1-DiffVth2);
	tmp_zero_pixels=0;
	tmp_mask_pixels=0;
	tmp_thre->SetTitle(Form("target_threshold : %d;threshold[e];# pixels",target_threshold));
	tmp_noise->SetTitle(Form("target_threshold : %d;Noise[e];# pixels",target_threshold));
	tmp_TDAC->SetTitle(Form("target_threshold : %d;TDAC;# pixels",target_threshold));
	totalpixel=0;

	gentry++;
      }      
      // pixel selection
      //if(Column>263 && ((Column%8>2&&Column%8<5) || (Column%8>0&&Column%8<3&&Row%8<6&&Row%8>7))){
      //if(Column>263 && (Column%8>2&&Column%8<5)){
      if(Column>263){
      //if(Column>263 && (Column%8==5&&Row%8<7)){
	totalpixel++;
	threshold_value[Column][Row]=Threshold;
	TDAC_value[Column][Row]=TDAC;
	noise_value[Column][Row]=Noise;
	Enable_value[Column][Row]=Enable;
	
	//std::cout << target_threshold << std::endl;
	hist2d[0]->Fill(target_threshold,Threshold,1);
	hist2d[1]->Fill(target_threshold,TDAC,1);
	hist2d[2]->Fill(DiffVth1-DiffVth2,Threshold,1);

	if(TDAC==15){
	  tmp_thre_TDAC_15->Fill(Threshold);
	}
	else if(TDAC==-15){
	  tmp_thre_TDAC_m15->Fill(Threshold);
	}
	tmp_thre->Fill(Threshold);
	tmp_noise->Fill(Noise);
	tmp_TDAC->Fill(TDAC);
	if(Threshold==0){
	  if(Enable==1){
	    tmp_zero_pixels++;
	  }
	  else if(Enable==0){
	    tmp_mask_pixels++;
	  }
	}
	
      }
      if(Column==399&&Row==191){
	tmp_c->cd();
	tmp_c->SetLogy();
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	THStack* hs_thre=new THStack("hs_thre","");
	hs_thre->Add(tmp_thre_TDAC_15);
	hs_thre->Add(tmp_thre_TDAC_m15);

	tmp_thre->Draw();
	totalEntry=tmp_thre->GetEntries();
	tmp_thre->Fit("f","Q","",100,3000);
	hs_thre->Draw("same");
	tmp_legend->Draw("same");
 	tmp_c->Print("tune_evaluation_ThresholdMap.pdf");

	delete hs_thre;

	tmp_noise->Draw();
 	tmp_c->Print("tune_evaluation_NoiseMap.pdf");

	tmp_thre_mean=f->GetParameter(1);
	tmp_thre_sigma=f->GetParameter(2);
	tmp_thre_mean_e=f->GetParError(1);
	tmp_thre_sigma_e=f->GetParError(2);

	//sigma_value=tmp_thre_sigma;
	sigma_value=40;
	//sigma_value=7;
	if(tmp_thre_mean-3*tmp_thre_sigma>0){
	  start_bin=tmp_thre->FindBin(tmp_thre_mean-3*sigma_value);
	  if (start_bin<=1){
	    start_bin=2;
	  }
	}
	else{
	  start_bin=2;
	}
	end_bin=tmp_thre->FindBin(tmp_thre_mean+3*sigma_value);

	tunable_pixels=tmp_thre->Integral(start_bin,end_bin);

	threshold_mean.push_back(tmp_thre_mean);
	threshold_sigma.push_back(tmp_thre_sigma);
 	fit_mis_pixels.push_back(tmp_zero_pixels);
 	Enable_0_pixels.push_back(tmp_mask_pixels);
	out_of_3sigma.push_back(totalEntry-tunable_pixels);
	out_of_3sigma_except_0.push_back(totalEntry-tunable_pixels-tmp_zero_pixels-tmp_mask_pixels);

	if(totalEntry-tunable_pixels-tmp_zero_pixels-tmp_mask_pixels<0){
	  std::cout << totalEntry-tunable_pixels-tmp_zero_pixels-tmp_mask_pixels << std::endl;
	  std::cout << "totalEntry :" << totalEntry << std::endl;
	  std::cout << "tunbale_pixels :" << tunable_pixels << std::endl;
	  std::cout << "tmp_zero_pixels:" << tmp_zero_pixels << std::endl;
	  std::cout << "tmp_mask_pixels:" << tmp_mask_pixels << std::endl;
	  std::cout << "start bin :" << start_bin << std::endl;
	  std::cout << "start :" << tmp_thre_mean-3*tmp_thre_sigma << start_bin << std::endl;
	}
	// TDAC distribution
	THStack* hs_TDAC=new THStack("hs_TDAC","TDAC_distribution");
	for(int i_col=264;i_col<400;i_col++){
	  for(int i_row=0;i_row<192;i_row++){
	    //    std::cout << threshold_value[i_col][i_row] << " , " << Enable_value[i_col][i_row] << std::endl;
	    if(threshold_value[i_col][i_row]==0){
	      if(Enable_value[i_col][i_row]==0){
		tmp_TDAC_masked->Fill(TDAC_value[i_col][i_row]);
	      }
	      else if(Enable_value[i_col][i_row]==1){
		tmp_TDAC_scanmiss->Fill(TDAC_value[i_col][i_row]);
	      }
	    }
	    else if(threshold_value[i_col][i_row]>tmp_thre_mean+3*sigma_value||threshold_value[i_col][i_row]<tmp_thre_mean-3*sigma_value){
	      tmp_TDAC_out_of_range->Fill(TDAC_value[i_col][i_row]);
	    }
	  }
	}
	hs_TDAC->Add(tmp_TDAC_scanmiss);
	hs_TDAC->Add(tmp_TDAC_masked);
	hs_TDAC->Add(tmp_TDAC_out_of_range);

	tmp_TDAC->Draw();
	hs_TDAC->Draw("same");
	TDAC_legend->Draw("same");
 	tmp_c->Print("tune_evaluation_TDACMap.pdf");
	delete hs_TDAC;

	//noise distribution
	THStack* hs_noise=new THStack("hs_noise","noise_distribution");
	for(int i_col=264;i_col<400;i_col++){
	  for(int i_row=0;i_row<192;i_row++){
	    //    std::cout << threshold_value[i_col][i_row] << " , " << Enable_value[i_col][i_row] << std::endl;
	    if(threshold_value[i_col][i_row]==0){
	      if(Enable_value[i_col][i_row]==0){
		tmp_noise_masked->Fill(noise_value[i_col][i_row]);
	      }
	      else if(Enable_value[i_col][i_row]==1){
		tmp_noise_scanmiss->Fill(noise_value[i_col][i_row]);
	      }
	    }
	    else if(threshold_value[i_col][i_row]>tmp_thre_mean+3*sigma_value||threshold_value[i_col][i_row]<tmp_thre_mean-3*sigma_value){
	      tmp_noise_out_of_range->Fill(noise_value[i_col][i_row]);
	    }
	  }
	}
	hs_noise->Add(tmp_noise_scanmiss);
	hs_noise->Add(tmp_noise_masked);
	hs_noise->Add(tmp_noise_out_of_range);
	
	tmp_noise->SetMinimum(0.9);
	tmp_noise->Draw();
	hs_noise->Draw("same");
	noise_legend->Draw("same");
 	tmp_c->Print("tune_evaluation_noiseMap_detail.pdf");
	delete hs_noise;
	
	tmp_thre->Reset();
	tmp_thre_TDAC_15->Reset();
	tmp_thre_TDAC_m15->Reset();
	tmp_noise->Reset();
	tmp_TDAC->Reset();
	tmp_TDAC_scanmiss->Reset();
	tmp_TDAC_masked->Reset();
	tmp_TDAC_out_of_range->Reset();
	tmp_noise->Reset();
	tmp_noise_scanmiss->Reset();
	tmp_noise_masked->Reset();
	tmp_noise_out_of_range->Reset();


	std::cout << RunID << std::endl;
      }
   }
   tmp_c->Print("tune_evaluation_ThresholdMap.pdf]");
   tmp_c->Print("tune_evaluation_NoiseMap.pdf]");
   tmp_c->Print("tune_evaluation_NoiseMap_detail.pdf]");
   tmp_c->Print("tune_evaluation_TDACMap.pdf]");
   delete tmp_c;
   delete tmp_thre;
   delete tmp_noise;
   delete tmp_TDAC;


   //auto min_it=target_threshold_vec.end();
   //auto max_it=target_threshold_vec.begin();

   double min=target_threshold_vec[gentry-1];
   //   double max=target_threshold_vec[0];
   double max=graph_upper+20;
   //auto bin=target_threshold_vec.size();
   //int min=0;
   //int max=2000;
   //int bin=100;
   //std::cout << gentry << std::endl;
   //std::cout << min << std::endl;
   //std::cout << max << std::endl;

   //double hist_bin=RunID+1;
   double hist_bin=(max-min)/20+1;

   TH1F* untuned_hist=new TH1F("untuned_hist","Tuning failed pixels;target_threshold[e];# pixels",hist_bin,min,max+(max-min)/hist_bin);
   untuned_hist->SetTitleOffset(1.0,"x");
   untuned_hist->SetTitleSize(0.045,"x");
   untuned_hist->SetTitleOffset(1.0,"y");
   untuned_hist->SetTitleSize(0.045,"y");

   TH1F* scanmiss_hist=new TH1F("scan_miss_hist","scan miss pixels;target_threshold;# pixels",hist_bin,min,max+(max-min)/hist_bin);
   TH1F* masked_hist=new TH1F("masked_hist","masked pixels;target_threshold;# pixels",hist_bin,min,max+(max-min)/hist_bin);
   TH1F* out_of_range_hist=new TH1F("out_og_range_hist","outofrange pixels;target_threshold;# pixels",hist_bin,min,max+(max-min)/hist_bin);
   TH1F* other_hist=new TH1F("other_hist","outofrange pixels;target_threshold;# pixels",hist_bin,min,max+(max-min)/hist_bin);

   scanmiss_hist->SetFillColor(2);
   masked_hist->SetFillColor(3);
   out_of_range_hist->SetFillColor(4);
   other_hist->SetFillColor(16);


   const int graph_num=9;
   TString graph_title[graph_num]={"#Delta_vs_target;target_threshold[e];#Delta",
				   "Threshold_mean_vs_target;target_threshold[e];threshold_mean[e]",
				   "Threshold_sigma_vs_target;target_threshold[e];threshold_sigma[e]",
				   "Threshold_vs#DeltaVTH;#DeltaVTH;Threshold[e]",
				   "fit-mis-pixel;target_threshold[e];# fit-mis-pixels",
				   "untuned_pixels;target_threshold[e];# untune_pixels",
				   "masked_pixels;target_threshold[e];# masked_pixels",
				   "out_of_range_pixels;target_threshold[e];# out_of_range_pixels",
				   "difference from target threshold;target_threshold[e];# target_mean[e]"};
   // DiffVth1-DiffVth2, Threshold_mean, Threshold_vs_d_Vth, Threshold0_pixels_num, pixels_out of sigma
   gStyle->SetMarkerStyle(8);
   TGraph* graph[graph_num];
   TCanvas* g_c[graph_num];
   //   std::cout << "gaya" << std::endl;

   for(int i_g=0;i_g<graph_num;i_g++){
     graph[i_g]=new TGraph(gentry);
     graph[i_g]->SetTitle(graph_title[i_g]);
     g_c[i_g]=new TCanvas(Form("g_c_%d",i_g),"");
   }
   //   double totalpixel=192*(400-263);
   graph[8]->SetMinimum(-60);
   graph[8]->SetMaximum(60);
   graph[2]->SetMaximum(100);
   graph[2]->SetMinimum(0);
   //std::cout << "gaya" << std::endl;
   for(int i_gentry=0;i_gentry<gentry;i_gentry++){
     graph[0]->SetPoint(i_gentry,target_threshold_vec[i_gentry],delta_Vth[i_gentry]);
     graph[1]->SetPoint(i_gentry,target_threshold_vec[i_gentry],threshold_mean[i_gentry]);
     graph[2]->SetPoint(i_gentry,target_threshold_vec[i_gentry],threshold_sigma[i_gentry]);
     graph[3]->SetPoint(i_gentry,delta_Vth[i_gentry],delta_Vth[i_gentry]);
     graph[4]->SetPoint(i_gentry,target_threshold_vec[i_gentry],fit_mis_pixels[i_gentry]);
     graph[5]->SetPoint(i_gentry,target_threshold_vec[i_gentry],out_of_3sigma[i_gentry]);
     graph[6]->SetPoint(i_gentry,target_threshold_vec[i_gentry],Enable_0_pixels[i_gentry]);
     graph[7]->SetPoint(i_gentry,target_threshold_vec[i_gentry],out_of_3sigma_except_0[i_gentry]);
     graph[8]->SetPoint(i_gentry,target_threshold_vec[i_gentry],threshold_mean[i_gentry]-target_threshold_vec[i_gentry]);

     if(out_of_3sigma_except_0[i_gentry]<0){
       std::cout << out_of_3sigma[i_gentry] << std::endl;
       std::cout << i_gentry << std::endl;
       std::cout << fit_mis_pixels[i_gentry] << std::endl;
       std::cout << Enable_0_pixels[i_gentry] << std::endl;
     }
     // std::cout << i_gentry << std::endl;

     untuned_hist->Fill(target_threshold_vec[i_gentry],out_of_3sigma[i_gentry]);
     
     scanmiss_hist->Fill(target_threshold_vec[i_gentry],fit_mis_pixels[i_gentry]);
     masked_hist->Fill(target_threshold_vec[i_gentry],Enable_0_pixels[i_gentry]);
     //std::cout << target_threshold_vec[i_gentry] << " , " << out_of_3sigma[i_gentry] << std::endl;
     out_of_range_hist->Fill(target_threshold_vec[i_gentry],out_of_3sigma_except_0[i_gentry]);
     other_hist->Fill(target_threshold_vec[i_gentry],out_of_3sigma[i_gentry]-out_of_3sigma_except_0[i_gentry]-fit_mis_pixels[i_gentry]-Enable_0_pixels[i_gentry]);
   }
   gStyle->SetOptFit(1111);
   graph[1]->Fit("pol1","Q","",0,2000);
   
   TLegend* legend=new TLegend(0.65,0.58,0.95,0.88);
   legend->AddEntry(scanmiss_hist,"scan failed","f");
   legend->AddEntry(masked_hist,"no responce","f");
   legend->AddEntry(out_of_range_hist,"out_of_3sigma","f");
   legend->AddEntry(other_hist,"other","f");
   legend->AddEntry(untuned_hist,"total","l");
   
   THStack* stack_hist=new THStack("stack_hist","tuning_failed_pixels; target_threshold;# pixels");

   untuned_hist->SetMinimum(0.001*totalpixel);
   scanmiss_hist->SetMinimum(0.001*totalpixel);
   masked_hist->SetMinimum(0.001*totalpixel);
   out_of_range_hist->SetMinimum(0.001*totalpixel);
   other_hist->SetMinimum(0.001*totalpixel);

   untuned_hist->Scale(1/totalpixel);
   scanmiss_hist->Scale(1/totalpixel);
   masked_hist->Scale(1/totalpixel);
   out_of_range_hist->Scale(1/totalpixel);
   other_hist->Scale(1/totalpixel);

   stack_hist->Add(scanmiss_hist);
   stack_hist->Add(masked_hist);
   stack_hist->Add(out_of_range_hist);
   stack_hist->Add(other_hist);
   

   gPad->Print("tune_evaluation.pdf[");
   for(int i_g=0;i_g<graph_num;i_g++){
     if (i_g != 3){
       graph[i_g]->GetXaxis()->SetLimits(0,graph_upper);
     }
     g_c[i_g]->cd();
     //g_c[i_g]->SetLogy();
     graph[i_g]->Draw("APL");
     g_c[i_g]->Print("tune_evaluation.pdf");
   }

   gStyle->SetOptStat(0);
   untuned_hist->Draw("hist");
   stack_hist->Draw("HIST same");

   legend->Draw("SAME");
   gPad->Print("tune_evaluation.pdf");
   gPad->SetLogy();
   gPad->Print("tune_evaluation.pdf");
   
   for(Int_t i_hist2d=0;i_hist2d<hist2d_num;i_hist2d++){
     h2d_c[i_hist2d]->cd();
     hist2d[i_hist2d]->SetMaximum(10000);
     hist2d[i_hist2d]->Draw("colz");
     //sleep(3);
     h2d_c[i_hist2d]->Print("tune_evaluation.pdf");
   }
   h2d_c[0]->cd();
   hist2d[0]->Draw("colz");
   h2d_c[0]->Print("tune_evaluation.pdf");
   gPad->Print("tune_evaluation.pdf]");
}

void run(TTree* tree){
  tune_evaluation * te = new tune_evaluation(tree);
  te->Loop();
}

void run(){
  tune_evaluation * te = new tune_evaluation(tree);
  te->Loop();
}
