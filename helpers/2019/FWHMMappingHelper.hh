#ifndef FWHMMappingHelper_h
#define FWHMMappingHelper_h

// Header file for the classes stored in the TTree if any.
#include "TGriffin.h"
#include "TGriffinBgo.h"
#include "TZeroDegree.h"
#include "TGRSIHelper.h"

#include "csv.h"

struct comptonData
{
   double angle;
   int angleIndex;
   std::vector<int> energy;
   std::vector<double> comptonLimit0;
   std::vector<double> comptonLimit1;
   std::vector<double> comptonLimit2;
   std::vector<double> comptonLimit3;
};

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
// Compton rejection stuff
void ReadInComptonLimitsFile(std::string dataFile);
std::vector<comptonData> BuildComptonBands(double energy_max = 4000);

std::vector<float> fAngularBinVec;
std::vector<float> fComptonLimits0;
std::vector<float> fComptonLimits1;

class FWHMMappingHelper : public TGRSIHelper,
                          public ROOT::Detail::RDF::RActionImpl<FWHMMappingHelper>
{
private:
   std::vector<std::pair<double, int>> fAngleCombinations;
   std::vector<std::pair<double, int>> fAngleCombinationsAddback; // with addback
   std::map<double, int> fAngleMap;
   std::map<double, int> fAngleMapAddback; // with addback

   TGriffin fLastGrif;

   std::vector<comptonData> fComptonMapping;
   bool GetComptonTag(double energy1, double energy2, int index);
   void GetComptonLimits(double energy, int index, std::vector<double> &vec);
   double ApplySplitCalibration(TGriffinHit *grifHit);
   bool DefaultGriffinAddback(TGriffinHit *one, TGriffinHit *two);
   Addback_t GetAddback(TGriffin &grif, TGriffinBgo &grifBgo);
   bool HasDuplicate(const std::vector<int> &vec);

public:
   FWHMMappingHelper(TList *list) : TGRSIHelper(list)
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

      // build Compton scatter limits
      std::string comptonLimitsDataFile = "/data_fast/cnatzke/two-photon/72Ge/data/compton_limits_145mm.csv";
      ReadInComptonLimitsFile(comptonLimitsDataFile);
      fComptonMapping = BuildComptonBands(5000);

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
extern "C" FWHMMappingHelper *CreateHelper(TList *list) { return new FWHMMappingHelper(list); }

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

/****************************************************************
 * Reads in csv file with Compton scattering bands
 *
 * @param filepath Filepath to csv file
 ***************************************************************/
void ReadInComptonLimitsFile(std::string dataFile)
{

   std::fstream compton_limit_file;

   compton_limit_file.open(dataFile, std::ios_base::in);
   // if bg file doesn't exist, throw error
   if (!compton_limit_file)
   {
      compton_limit_file.close();
      std::cout << "Could not open " << dataFile << ", exiting." << std::endl;
      exit(EXIT_FAILURE);
   }
   else
   {
      // std::cout << "Found accepted Compton scatter angles file: " << dataFile << "\n"
      //  << std::endl;
      io::CSVReader<5> in(dataFile);
      in.read_header(io::ignore_extra_column, "angular_bin", "compton_limit_ftb", "compton_limit_btf", "angle_diff_ftb", "angle_diff_btf");
      float angular_bin;
      float compton_limit_ftb;
      float compton_limit_btf;
      float angle_diff_ftb;
      float angle_diff_btf;
      while (in.read_row(angular_bin, compton_limit_ftb, compton_limit_btf, angle_diff_ftb, angle_diff_btf))
      {
         fAngularBinVec.push_back(angular_bin);
         fComptonLimits0.push_back(compton_limit_ftb);
         fComptonLimits1.push_back(compton_limit_btf);
      }
      compton_limit_file.close();
      // remove first bin of filled vectors sinces it's the zero angle
      fAngularBinVec.erase(fAngularBinVec.begin());
      fComptonLimits0.erase(fComptonLimits0.begin());
      fComptonLimits1.erase(fComptonLimits1.begin());
   }

} // end ReadInComptonLimits

/****************************************************************
 * Builds allowed Compton bands
 *
 ***************************************************************/
std::vector<comptonData> BuildComptonBands(double energy_max)
{

   std::vector<int> x;
   std::vector<comptonData> result(static_cast<int>(fAngularBinVec.size()));
   std::vector<double> evaluated_function0(static_cast<int>(energy_max));
   std::vector<double> evaluated_function1(static_cast<int>(energy_max));
   std::vector<double> evaluated_function2(static_cast<int>(energy_max));
   std::vector<double> evaluated_function3(static_cast<int>(energy_max));

   double electron_rest_mass_energy = 511.; // keV

   // Compton-Scatter functions
   TF1 *f0 = new TF1("f0", "x / (1 + (x / [0]) * (1 - TMath::Cos([1])))", 0, energy_max);
   TF1 *f1 = new TF1("f1", "x / (1 + (x / [0]) * (1 - TMath::Cos([1])))", 0, energy_max);
   TF1 *f2 = new TF1("f2", "x - x / (1 + (x / [0]) * (1 - TMath::Cos([1])))", 0, energy_max);
   TF1 *f3 = new TF1("f3", "x - x / (1 + (x / [0]) * (1 - TMath::Cos([1])))", 0, energy_max);

   // first build evaluation points
   for (int eval_point = 0; eval_point < energy_max; ++eval_point)
   {
      x.push_back(eval_point);
   }

   // now build the vector we care about
   f0->SetParameter(0, electron_rest_mass_energy);
   f1->SetParameter(0, electron_rest_mass_energy);
   f2->SetParameter(0, electron_rest_mass_energy);
   f3->SetParameter(0, electron_rest_mass_energy);

   for (int index = 0; index < static_cast<int>(fAngularBinVec.size()); ++index)
   {
      f0->SetParameter(1, fComptonLimits0.at(index));
      f1->SetParameter(1, fComptonLimits1.at(index));
      f2->SetParameter(1, fComptonLimits0.at(index));
      f3->SetParameter(1, fComptonLimits1.at(index));

      for (const int &i : x)
      {
         double val0 = f0->Eval(i);
         double val1 = f1->Eval(i);
         double val2 = f2->Eval(i);
         double val3 = f3->Eval(i);

         // if (index == 2 && i == 1500)
         // {
         //    std::cout << "---> Test" << std::endl;
         //    std::cout << i << std::endl;
         //    std::cout << fComptonLimits0.at(2) << ", " << f0->Eval(i) << std::endl;
         //    std::cout << fComptonLimits1.at(2) << ", " << f1->Eval(i) << std::endl;
         //    std::cout << f2->Eval(i) << std::endl;
         //    std::cout << f3->Eval(i) << std::endl;
         //    std::cout << std::endl;
         // }

         evaluated_function0.at(i) = val0;
         evaluated_function1.at(i) = val1;
         evaluated_function2.at(i) = val2;
         evaluated_function3.at(i) = val3;
      }

      result.at(index).angle = fAngularBinVec.at(index);
      result.at(index).angleIndex = index;
      result.at(index).energy = x;
      result.at(index).comptonLimit0 = evaluated_function1;
      result.at(index).comptonLimit1 = evaluated_function0;
      result.at(index).comptonLimit2 = evaluated_function2;
      result.at(index).comptonLimit3 = evaluated_function3;
   }

   delete f0;
   delete f1;
   delete f2;
   delete f3;

   return result;

} // end BuildComptonBands