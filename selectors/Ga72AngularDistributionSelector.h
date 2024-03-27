//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct 25 13:18:27 2016 by ROOT version 5.34/24
// from TTree FragmentTree/FragmentTree
// found on file: fragment07844_000.root
//////////////////////////////////////////////////////////

#ifndef Ga72AngularDistributionSelector_H
#define Ga72AngularDistributionSelector_H

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

// function to calculate angles (from LeanCorrelations), implemented at the end of this file
std::vector<std::pair<double, int>> AngleCombinations(double distance = 110., bool folding = false, bool addback = false);

class Ga72AngularDistributionSelector : public TGRSISelector
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
        std::vector<std::pair<double, int>> fAngleCombinations;
        std::vector<std::pair<double, int>> fAngleCombinationsAddback; // with addbacks
        std::map<double, int> fAngleMap;
        std::map<double, int> fAngleMapAddback; // with addback

        Ga72AngularDistributionSelector(TTree * /*tree*/ = 0) : TGRSISelector(), fGrif(nullptr), fGriffinBgo(NULL), fScep(nullptr), fZDS(nullptr), fLaBr(nullptr), fLaBrBgo(NULL), fTAC(nullptr), fPaces(nullptr)
        {
                SetOutputPrefix("run");
                fAngleCombinations = AngleCombinations(110., false, false);
                // fAngleCombinationsAddback = AngleCombinations(110., false, true); // with addback
                for (int i = 0; i < static_cast<int>(fAngleCombinations.size()); ++i)
                {
                        fAngleMap.insert(std::make_pair(fAngleCombinations[i].first, i));
                }
                // for (int i = 0; i < static_cast<int>(fAngleCombinationsAddback.size()); ++i)
                // {
                //         fAngleMapAddback.insert(std::make_pair(fAngleCombinationsAddback[i].first, i));
                // }
        }
        virtual ~Ga72AngularDistributionSelector() {}
        virtual Int_t Version() const { return 2; }
        void CreateHistograms();
        void FillHistograms();
        void InitializeBranches(TTree *tree);
};
#endif

#ifdef Ga72AngularDistributionSelector_cxx

void Ga72AngularDistributionSelector::InitializeBranches(TTree *tree)
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

#endif // #ifdef Ga72AngularDistributionSelector_cxx
