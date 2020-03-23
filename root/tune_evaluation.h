//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Aug 23 11:50:30 2019 by ROOT version 6.16/00
// from TChain tree/
//////////////////////////////////////////////////////////

#ifndef tune_evaluation_h
#define tune_evaluation_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class tune_evaluation {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           RunID;
   Int_t           Column;
   Int_t           Row;
   Int_t           target_threshold;
   Double_t        Threshold;
   Double_t        Noise;
   Int_t           DiffVth1;
   Int_t           DiffVth2;
   Int_t           DiffPrecomp;
   Int_t           TDAC;
   Int_t           Enable;

   // List of branches
   TBranch        *b_RunID;   //!
   TBranch        *b_Column;   //!
   TBranch        *b_Row;   //!
   TBranch        *b_target_threshold;   //!
   TBranch        *b_Threshold;   //!
   TBranch        *b_Noise;   //!
   TBranch        *b_DiffVth1;   //!
   TBranch        *b_DiffVth2;   //!
   TBranch        *b_DiffPrecomp;   //!
   TBranch        *b_TDAC;   //!
   TBranch        *b_Enable;   //!

   tune_evaluation(TTree *tree=0);
   virtual ~tune_evaluation();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef tune_evaluation_cxx
tune_evaluation::tune_evaluation(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya50.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya50.root");
      }
      f->GetObject("tree",tree);

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("tree","");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya0.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya1.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya2.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya3.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya4.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya5.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya6.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya7.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya8.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya9.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya10.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya11.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya12.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya13.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya14.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya15.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya16.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya17.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya18.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya19.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya20.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya21.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya22.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya23.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya24.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya25.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya26.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya27.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya28.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya29.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya30.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya31.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya32.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya33.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya34.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya35.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya36.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya37.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya38.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya39.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya40.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya41.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya42.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya43.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya44.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya45.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya46.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya47.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya48.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya49.root/tree");
      chain->Add("/home/yamagaya/Desktop/Yarr-sw/Low-threshold/root/0822_aftertune_2000_to_0//gaya50.root/tree");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

tune_evaluation::~tune_evaluation()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t tune_evaluation::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t tune_evaluation::LoadTree(Long64_t entry)
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

void tune_evaluation::Init(TTree *tree)
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
   fChain->SetBranchAddress("Threshold", &Threshold, &b_Threshold);
   fChain->SetBranchAddress("Noise", &Noise, &b_Noise);
   fChain->SetBranchAddress("DiffVth1", &DiffVth1, &b_DiffVth1);
   fChain->SetBranchAddress("DiffVth2", &DiffVth2, &b_DiffVth2);
   fChain->SetBranchAddress("DiffPrecomp", &DiffPrecomp, &b_DiffPrecomp);
    fChain->SetBranchAddress("TDAC", &TDAC, &b_TDAC);
    fChain->SetBranchAddress("Enable", &Enable, &b_Enable);
   Notify();
}

Bool_t tune_evaluation::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void tune_evaluation::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t tune_evaluation::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef tune_evaluation_cxx
