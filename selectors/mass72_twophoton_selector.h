//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct 25 13:18:27 2016 by ROOT version 5.34/24
// from TTree FragmentTree/FragmentTree
// found on file: fragment07844_000.root
//////////////////////////////////////////////////////////

#ifndef MASS72_TWOPHOTON_SELECTOR_H
#define MASS72_TWOPHOTON_SELECTOR_H

#include <stdlib.h> // rand() function
#include <vector>

#include "TChain.h"
#include "TFile.h"

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TCutG.h"

// Header file for the classes stored in the TTree if any.
#include "TGriffin.h"
#include "TGriffinBgo.h"
#include "TSceptar.h"
#include "TZeroDegree.h"
#include "TLaBr.h"
#include "TLaBrBgo.h"
#include "TTAC.h"
#include "TPaces.h"
#include "TGRSISelector.h"

class mass72_twophoton_selector : public TGRSISelector
{
public:
        TGriffin *fGrif;
        TGriffinBgo *fGriffinBgo;
        TSceptar *fScep;
        TZeroDegree *fZDS;
        TLaBr *fLaBr;
        TLaBrBgo *fLaBrBgo;
        TTAC *fTAC;
        TPaces *fPaces;
        Long64_t fCycleLength;

        // singles vectors
        vector<double> singles_energy;
        vector<double> singles_charge;
        vector<int> singles_detector;
        vector<Int_t> singles_clover;
        vector<long> singles_timestamp;

        vector<float> add_en;   // vector which contains the addback energy values
        vector<float> add_CTen; // vector which contains the addback energy values corrected for CrossTalk
        vector<int> skip;
        vector<float> addSuppr_en;   // vector which contains the suppressed addback energy values
        vector<float> addSuppr_CTen; // vector which contains the suppressed addback energy values corrected for CrossTalk
        vector<long> addSuppr_time, addSuppr_timestamp;
        vector<int> addSuppr_detector; // vectors whcih contain the addback events time, timestamp and crystal
        vector<int> skipSuppr;

        mass72_twophoton_selector(TTree * /*tree*/ = 0) : TGRSISelector(), fGrif(nullptr), fGriffinBgo(NULL), fScep(nullptr), fZDS(nullptr), fLaBr(nullptr), fLaBrBgo(NULL), fTAC(nullptr), fPaces(nullptr)
        {
                SetOutputPrefix("run");
        }
        virtual ~mass72_twophoton_selector() {}
        virtual Int_t Version() const { return 2; }
        void CreateHistograms();
        void FillHistograms();
        void InitializeBranches(TTree *tree);

        TCutG *pE_ZDS_dt;
        TCutG *gE_ZDS_dt;

        ClassDef(mass72_twophoton_selector, 1); // 2); //Makes ROOT happier
};
#endif

#ifdef mass72_twophoton_selector_cxx
void mass72_twophoton_selector::InitializeBranches(TTree *tree)
{
        if (!tree)
                return;
        if (tree->SetBranchAddress("TGriffin", &fGrif) == TTree::kMissingBranch)
        {
                fGrif = new TGriffin;
        }
        if (tree->SetBranchAddress("TGriffinBgo", &fGriffinBgo) == TTree::kMissingBranch)
        {
                fGriffinBgo = new TGriffinBgo;
        }
        if (tree->SetBranchAddress("TLaBr", &fLaBr) == TTree::kMissingBranch)
        {
                fLaBr = new TLaBr;
        }
        if (tree->SetBranchAddress("TLaBrBgo", &fLaBrBgo) == TTree::kMissingBranch)
        {
                fLaBrBgo = new TLaBrBgo;
        }
        if (tree->SetBranchAddress("TZeroDegree", &fZDS) == TTree::kMissingBranch)
        {
                fZDS = new TZeroDegree;
        }
        if (tree->SetBranchAddress("TSceptar", &fScep) == TTree::kMissingBranch)
        {
                fScep = new TSceptar;
        }
        if (tree->SetBranchAddress("TPaces", &fPaces) == TTree::kMissingBranch)
        {
                fPaces = new TPaces;
        }
        if (tree->SetBranchAddress("TTAC", &fTAC) == TTree::kMissingBranch)
        {
                fTAC = new TTAC;
        }
}

#endif // #ifdef mass72_twophoton_selector_cxx
