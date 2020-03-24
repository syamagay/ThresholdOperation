//
// ATLAS Style, based on a style file from BaBar
//

#include <iostream>

#include "GayaStyle.h"

#include "TROOT.h"

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
  gStyle->SetPadRightMargin(0.10); //original 0.05
  gStyle->SetPadBottomMargin(0.12); //original 0.16
  gStyle->SetPadLeftMargin(0.13); //original 0.05

  gStyle->SetTitleXOffset(1.0);
  gStyle->SetTitleYOffset(1.2);
  gStyle->SetTitleOffset(0.8,"z");

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

  gStyle->SetNdivisions(510);

  //gStyle->SetStripDecimals(false);

  gStyle->SetOptStat("eMinoru");
  gStyle->SetStatX(0.98);
  gStyle->SetStatY(0.9);
  gStyle->SetStatH(0.23);
  gStyle->SetStatW(0.20);
  gStyle->SetStatFontSize(0.06);


  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  
}

TStyle* GayaStyle() 
{
  TStyle *atlasStyle = new TStyle("GAYA","Gaya style");

  // use plain black on white colors
  
  Int_t icol=0; // WHITE
  //atlasStyle = gStyle;
  
  //atlasStyle->SetFrameBorderMode(icol);
  atlasStyle->SetFrameFillColor(icol);
  //atlasStyle->SetCanvasBorderMode(icol);
  atlasStyle->SetCanvasColor(icol);
  //atlasStyle->SetPadBorderMode(icol);
  atlasStyle->SetPadColor(icol);
  atlasStyle->SetFuncColor(kRed);
  atlasStyle->SetFuncWidth(2);
  //atlasStyle->SetStatColor(icol);
  //atlasStyle->SetFillColor(icol); // don't use: white fill color for *all* objects

  // set the paper & margin sizes
  //atlasStyle->SetPaperSize(20,26);
  
  // set margin sizes
  atlasStyle->SetPadTopMargin(0.10); //original 0.16
  atlasStyle->SetPadRightMargin(0.16); //original 0.05
  atlasStyle->SetPadBottomMargin(0.12); //original 0.16
  atlasStyle->SetPadLeftMargin(0.14); //original 0.05

  // set title offsets (for axis label)
  atlasStyle->SetTitleXOffset(1.0);
  atlasStyle->SetTitleYOffset(1.2);
  atlasStyle->SetTitleOffset(1.3,"z");

  // use large fonts
  //Int_t font=72; // Helvetica italics
  Int_t font=42; // Helvetica
  Double_t tsize=0.05;
  atlasStyle->SetTextFont(font);

  atlasStyle->SetTextSize(tsize);
  atlasStyle->SetLabelFont(font,"x");
  atlasStyle->SetTitleFont(font,"x");
  atlasStyle->SetLabelFont(font,"y");
  atlasStyle->SetTitleFont(font,"y");
  atlasStyle->SetLabelFont(font,"z");
  atlasStyle->SetTitleFont(font,"z");
  
  atlasStyle->SetLabelSize(tsize,"x");
  atlasStyle->SetTitleSize(tsize,"x");
  atlasStyle->SetLabelSize(tsize,"y");
  atlasStyle->SetTitleSize(tsize,"y");
  atlasStyle->SetLabelSize(tsize,"z");
  atlasStyle->SetTitleSize(tsize,"z");

  // use bold lines and markers
  atlasStyle->SetMarkerStyle(20);
  atlasStyle->SetMarkerSize(1.2);
  atlasStyle->SetHistLineWidth(1.); //original:2. gaya changed(2020/01/20)
  atlasStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars (as recommended in ATLAS figure guidelines)
  atlasStyle->SetErrorX(0.0001);
  // get rid of error bar caps
  atlasStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  atlasStyle->SetOptTitle(1);
  //atlasStyle->SetOptStat(1111);
  atlasStyle->SetOptStat(0);
  //atlasStyle->SetOptFit(1111);
  atlasStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  //atlasStyle->SetPadTickX(1);
  //atlasStyle->SetPadTickY(1);
  
  return atlasStyle;
  //return gStyle;
}

