//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Feb 12 17:28:20 2020 by ROOT version 6.16/00
// from TTree tree/
// found on file: 0119_lowerlimit_simple_all_OU078B/gaya0.root
//////////////////////////////////////////////////////////

#ifndef Core_NoiseDist_h
#define Core_NoiseDist_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <TGaxis.h>
#include <TSystem.h>

// Header file for the classes stored in the TTree if any.

class Core_NoiseDist {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   std::string     output_dir;

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           RunID;
   Int_t           Column;
   Int_t           Row;
   Int_t           target_threshold;
   Int_t           scan_time;
   Double_t        Threshold;
   Double_t        Noise;
   Double_t        Threshold_via;
   Double_t        Noise_via;
   Int_t           DiffVth1;
   Int_t           DiffVth2;
   Int_t           DiffPrecomp;
   Int_t           TDAC;
   Int_t           Enable;
   Int_t           InjEn;
   Int_t           TDAC_via;
   Int_t           min;
   Int_t           max;
   Int_t           step;
   Int_t           trigCount;

   // List of branches
   TBranch        *b_RunID;   //!
   TBranch        *b_Column;   //!
   TBranch        *b_Row;   //!
   TBranch        *b_target_threshold;   //!
   TBranch        *b_scan_time;   //!
   TBranch        *b_Threshold;   //!
   TBranch        *b_Noise;   //!
   TBranch        *b_Threshold_via;   //!
   TBranch        *b_Noise_via;   //!
   TBranch        *b_DiffVth1;   //!
   TBranch        *b_DiffVth2;   //!
   TBranch        *b_DiffPrecomp;   //!
   TBranch        *b_TDAC;   //!
   TBranch        *b_Enable;   //!
   TBranch        *b_InjEn;   //!
   TBranch        *b_TDAC_via;   //!
   TBranch        *b_min;   //!
   TBranch        *b_max;   //!
   TBranch        *b_step;   //!
   TBranch        *b_trigCount;   //!

   Core_NoiseDist(TTree *tree=0, std::string dir="");
   virtual ~Core_NoiseDist();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Core_NoiseDist_cxx
Core_NoiseDist::Core_NoiseDist(TTree *tree, std::string dir) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   output_dir=dir;
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("0119_lowerlimit_simple_all_OU078B/gaya0.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("0119_lowerlimit_simple_all_OU078B/gaya0.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

Core_NoiseDist::~Core_NoiseDist()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Core_NoiseDist::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Core_NoiseDist::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Core_NoiseDist::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("RunID", &RunID, &b_RunID);
   fChain->SetBranchAddress("Column", &Column, &b_Column);
   fChain->SetBranchAddress("Row", &Row, &b_Row);
   fChain->SetBranchAddress("target_threshold", &target_threshold, &b_target_threshold);
   fChain->SetBranchAddress("scan_time", &scan_time, &b_scan_time);
   fChain->SetBranchAddress("Threshold", &Threshold, &b_Threshold);
   fChain->SetBranchAddress("Noise", &Noise, &b_Noise);
   fChain->SetBranchAddress("Threshold_via", &Threshold_via, &b_Threshold_via);
   fChain->SetBranchAddress("Noise_via", &Noise_via, &b_Noise_via);
   fChain->SetBranchAddress("DiffVth1", &DiffVth1, &b_DiffVth1);
   fChain->SetBranchAddress("DiffVth2", &DiffVth2, &b_DiffVth2);
   fChain->SetBranchAddress("DiffPrecomp", &DiffPrecomp, &b_DiffPrecomp);
   fChain->SetBranchAddress("TDAC", &TDAC, &b_TDAC);
   fChain->SetBranchAddress("Enable", &Enable, &b_Enable);
   fChain->SetBranchAddress("InjEn", &InjEn, &b_InjEn);
   fChain->SetBranchAddress("TDAC_via", &TDAC_via, &b_TDAC_via);
   fChain->SetBranchAddress("min", &min, &b_min);
   fChain->SetBranchAddress("max", &max, &b_max);
   fChain->SetBranchAddress("step", &step, &b_step);
   fChain->SetBranchAddress("trigCount", &trigCount, &b_trigCount);
   Notify();
}

Bool_t Core_NoiseDist::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Core_NoiseDist::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Core_NoiseDist::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Core_NoiseDist_cxx
