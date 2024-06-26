#ifndef TriplePhotonCoincidenceHelper_h
#define TriplePhotonCoincidenceHelper_h

// Header file for the classes stored in the TTree if any.
#include "TGriffin.h"
#include "TGriffinBgo.h"
#include "TZeroDegree.h"
#include "TGRSIHelper.h"

#include "csv.h"

struct Addback_t
{
   std::vector<Double_t> energyCTCorrected;
   std::vector<Double_t> energy;
   std::vector<Long_t> time;
   std::vector<Long_t> timestamp;
   std::vector<Int_t> clover;
};

// function to calculate angles (from LeanCorrelations), implemented at the end of this file
std::vector<std::pair<double, int>> AngleCombinations(double distance = 145., bool folding = false,
                                                      bool addback = false);

std::vector<float> fAngularBinVec;

class TriplePhotonCoincidenceHelper : public TGRSIHelper,
                                      public ROOT::Detail::RDF::RActionImpl<TriplePhotonCoincidenceHelper>
{
private:
   std::vector<std::pair<double, int>> fAngleCombinations;
   std::vector<std::pair<double, int>> fAngleCombinationsAddback; // with addback
   std::map<double, int> fAngleMap;
   std::map<double, int> fAngleMapAddback; // with addback

   TGriffin fLastGrif;

   double ApplySplitCalibration(TGriffinHit *grifHit);
   bool DefaultGriffinAddback(TGriffinHit *one, TGriffinHit *two);
   Addback_t GetAddback(TGriffin &grif, TGriffinBgo &grifBgo);

public:
   TriplePhotonCoincidenceHelper(TList *list) : TGRSIHelper(list)
   {
      Prefix("run");
      // calculate angle combinations
      fAngleCombinations = AngleCombinations(145., false, false);
      fAngleCombinationsAddback = AngleCombinations(145., false, true); // with addback
      for (int i = 0; i < static_cast<int>(fAngleCombinations.size()); ++i)
      {
         fAngleMap.insert(std::make_pair(fAngleCombinations[i].first, i));
      }
      for (int i = 0; i < static_cast<int>(fAngleCombinationsAddback.size()); ++i)
      {
         fAngleMapAddback.insert(std::make_pair(fAngleCombinationsAddback[i].first, i));
      }

      // Setup calls CreateHistograms, which uses the stored angle combinations, so we need those set before
      Setup();
   }
   ROOT::RDF::RResultPtr<std::map<std::string, TList>> Book(ROOT::RDataFrame *d) override
   {
      return d->Book<TGriffin, TGriffinBgo, TZeroDegree>(std::move(*this), {"TGriffin", "TGriffinBgo", "TZeroDegree"});
   }
   void CreateHistograms(unsigned int slot);
   void Exec(unsigned int slot, TGriffin &grif, TGriffinBgo &grifBgo, TZeroDegree &zds);
};
#endif

// These are needed functions used by TDataFrameLibrary to create and destroy the instance of this TimingInvestigationHelper
extern "C" TriplePhotonCoincidenceHelper *CreateHelper(TList *list) { return new TriplePhotonCoincidenceHelper(list); }

extern "C" void DestroyHelper(TGRSIHelper *helper) { delete helper; }

std::vector<std::pair<double, int>> AngleCombinations(double distance, bool folding, bool addback)
{
   std::vector<std::pair<double, int>> result;
   std::vector<std::pair<double, int>> grouped_result;
   std::vector<double> angle;
   for (int firstDet = 1; firstDet <= 16; ++firstDet)
   {
      for (int firstCry = 0; firstCry < 4; ++firstCry)
      {
         for (int secondDet = 1; secondDet <= 16; ++secondDet)
         {
            for (int secondCry = 0; secondCry < 4; ++secondCry)
            {
               if (firstDet == secondDet && firstCry == secondCry)
               {
                  continue;
               }
               if (!addback)
               {
                  angle.push_back(TGriffin::GetPosition(firstDet, firstCry, distance)
                                      .Angle(TGriffin::GetPosition(secondDet, secondCry, distance)) *
                                  180. / TMath::Pi());
               }
               if (addback)
               {
                  if (((TGriffin::GetPosition(firstDet, firstCry, distance)
                                .Angle(TGriffin::GetPosition(secondDet, secondCry, distance)) *
                            180. / TMath::Pi() >
                        18.786) &&
                       (TGriffin::GetPosition(firstDet, firstCry, distance)
                                .Angle(TGriffin::GetPosition(secondDet, secondCry, distance)) *
                            180. / TMath::Pi() <
                        18.788)) ||
                      ((TGriffin::GetPosition(firstDet, firstCry, distance)
                                .Angle(TGriffin::GetPosition(secondDet, secondCry, distance)) *
                            180. / TMath::Pi() >
                        26.6800) &&
                       (TGriffin::GetPosition(firstDet, firstCry, distance)
                                .Angle(TGriffin::GetPosition(secondDet, secondCry, distance)) *
                            180. / TMath::Pi() <
                        26.6915)))
                  {
                     angle.push_back(18.7868);
                  }
                  else
                  {
                     angle.push_back(TGriffin::GetPosition(firstDet, firstCry, distance)
                                         .Angle(TGriffin::GetPosition(secondDet, secondCry, distance)) *
                                     180. / TMath::Pi());
                  }
               }
               if (folding && angle.back() > 90.)
               {
                  angle.back() = 180. - angle.back();
               }
            }
         }
      }
   }

   std::sort(angle.begin(), angle.end());
   size_t r;
   for (size_t a = 0; a < angle.size(); ++a)
   {
      for (r = 0; r < result.size(); ++r)
      {
         if (angle[a] >= result[r].first - 0.001 && angle[a] <= result[r].first + 0.001)
         {
            (result[r].second)++;
            break;
         }
      }
      if (result.size() == 0 || r == result.size())
      {
         result.push_back(std::make_pair(angle[a], 1));
      }
   }

   if (folding)
   { // if we fold we also want to group
      std::vector<std::pair<double, int>> groupedResult;
      for (size_t i = 0; i < result.size(); ++i)
      {
         switch (i)
         {
         case 0:
         case 1:
            groupedResult.push_back(result[i]);
            break;
         case 2:
         case 4:
         case 6:
            if (i + 1 >= result.size())
            {
               std::cerr << "Error!" << std::endl;
            }
            groupedResult.push_back(
                std::make_pair((result[i].first + result[i + 1].first) / 2., result[i].second + result[i + 1].second));
            ++i;
            break;
         default:
            groupedResult.push_back(std::make_pair((result[i].first + result[i + 1].first + result[i + 2].first) / 3.,
                                                   result[i].second + result[i + 1].second + result[i + 2].second));
            i += 2;
            break;
         }
      }
      return groupedResult;
   }

   return result;
}
