#include "QuadPhotonCoincidenceHelper.hh"
#include <stdlib.h> // rand() function
#include <unordered_set>

Double_t betaGammaTimingOffset = 277.01;

// -- time random window
Int_t timeRandomGateStart = 1000;
Int_t timeRandomGateEnd = 2000;

double gammaEnergyMin = 0.;
double gammaEnergyMax = 4000.;

// -- minimum values conditions
double gammaMinimumEnergy = 10;  // keV
double minimumScatterAngle = 25; // deg

// --- energy gates, PoC
double expectedTwoPhotonEnergy = 1862;
double gateEnergy1 = 629;
double gateEnergy2 = 834;
double nuclearLevelEnergy = 3325;
double qValue = 3998;
// --- prompt cascade, proof of concept (PoC)
Int_t gateGammaDelayWindowStart = 0;   // ns
Int_t gateGammaDelayWindowWidth = 400; // ns

// -- energy gates, two-photon
// double expectedTwoPhotonEnergy = 691;
// double gateEnergy1 = 633;
// double gateEnergy2 = 1710;
// double nuclearLevelEnergy = 3036;
// double qValue = 3998;
// // --- delayed coincidence timing
// Int_t gateGammaDelayWindowStart = 90;  // ns
// Int_t gateGammaDelayWindowWidth = 970; // ns

// -- global options
double gateEnergyHalfWidth = 3;
float nuclearLevelEnergyBuffer = 20;
bool useSecondGate = true;
bool setTotalEnergyFloor = false;

// -- timings
Int_t twoPhotonCoincidenceTime = 90; // ns, max time diff for two-photon events
Int_t gateGammaDelayWindowEnd = gateGammaDelayWindowStart + gateGammaDelayWindowWidth;

// --- diagnostic count, for printing
int count = 0;

bool CoincidentInTimeAbs(Double_t time1, Double_t time2, Double_t time_max)
{
   return TMath::Abs(time1 - time2) < time_max;
}

// =========== ADDBACK BUILDING taken from GRSISort/libraries/TGRSIAnalysis/TGrfiffin/TGriffin.cxx ============ //
// bool DefaultGriffinAddback(const TGRSIDetectorHit& one, const TGRSIDetectorHit& two)
bool DefaultGriffinAddback(TGriffinHit *one, TGriffinHit *two)
{
   return ((one->GetDetector() == two->GetDetector()) &&
           (std::fabs(one->GetTime() - two->GetTime()) < 300.));
   // TGRSIOptions::AnalysisOptions()->AddbackWindow())); // change this to 300 (ns) if it complains
}

void QuadPhotonCoincidenceHelper::CreateHistograms(unsigned int slot)
{
   //* ------- Energy -------
   fH1[slot]["gEnergyTotal"] = new TH1D("gEnergyTotal", "Total decay energy", gammaEnergyMax, gammaEnergyMin, gammaEnergyMax);
   fH2[slot]["gEChannel"] = new TH2D("gEChannel", "HPGe crystal number vs #gamma energy", 65, 0., 65., gammaEnergyMax, gammaEnergyMin, gammaEnergyMax);

   fH2[slot]["ggggSumEnergy"] = new TH2D("ggggSumEnergy", Form("#Deltat_{#gamma#gamma} < %i ns, #Deltat_{gate}#in(%i,%i) ns, Gate: %i keV;E_{#gamma+#gamma} [keV];E_{#gamma} [keV]", twoPhotonCoincidenceTime, gateGammaDelayWindowStart, gateGammaDelayWindowEnd, (int)gateEnergy1), gammaEnergyMax, gammaEnergyMin, gammaEnergyMax, gammaEnergyMax, gammaEnergyMin, gammaEnergyMax);
   fH2[slot]["ggggSumEnergy2Gates"] = new TH2D("ggggSumEnergy2Gates", Form("#Deltat_{#gamma#gamma} < %i ns, #Deltat_{gate}#in(%i,%i) ns, Gate: %i,%i keV;E_{#gamma+#gamma} [keV];E_{#gamma} [keV]", twoPhotonCoincidenceTime, gateGammaDelayWindowStart, gateGammaDelayWindowEnd, (int)gateEnergy1, (int)gateEnergy2), gammaEnergyMax, gammaEnergyMin, gammaEnergyMax, gammaEnergyMax, gammaEnergyMin, gammaEnergyMax);

   //* ------- Timing -------
   fH2[slot]["ggTiming"] = new TH2D("ggTiming", "#Deltat #gamma#gamma", 1200, -100, 1100, gammaEnergyMax, gammaEnergyMin, gammaEnergyMax);
   fH2[slot]["ggggTiming"] = new TH2D("ggggTiming", "#Deltat #gamma#gamma-#gamma-#gamma", 2500, -500, 2000, gammaEnergyMax, gammaEnergyMin, gammaEnergyMax);

   //* ------- Angles -------
   fH1[slot]["angleDiagnostic"] = new TH1D("angleDiagnostic", "Angle between any two #gamma", 185, 0, 185);

   //* ------- Multiplicity -------

   //* ------- Hit Pattern -------
   // fH2[slot]["ggggHitPattern"] = new TH2D("ggggHitPattern", "Hit pattern of #gamma#gamma#gamma#gamma events", 65, 0., 65., 65, 0., 65);
}

void QuadPhotonCoincidenceHelper::Exec(unsigned int slot, TGriffin &grif, TGriffinBgo &grifBgo, TZeroDegree &zds)
{

   // --- First gamma
   for (auto g1 = 0; g1 < grif.GetSuppressedMultiplicity(&grifBgo); ++g1)
   {
      auto grif1 = grif.GetSuppressedHit(g1);

      double calibratedEnergyGrif1 = ApplySplitCalibration(grif1);

      // --- sets energy floor at minimum value;
      if (calibratedEnergyGrif1 < gammaMinimumEnergy)
      {
         continue;
      }

      fH2[slot].at("gEChannel")->Fill(grif1->GetArrayNumber(), calibratedEnergyGrif1);

      // --- Second gamma
      for (auto g2 = g1 + 1; g2 < grif.GetSuppressedMultiplicity(&grifBgo); ++g2)
      {
         if (g1 >= g2)
         {
            continue;
         }

         auto grif2 = grif.GetSuppressedHit(g2);

         double calibratedEnergyGrif2 = ApplySplitCalibration(grif2);
         // --- sets energy floor at minimum value;
         if (calibratedEnergyGrif2 < gammaMinimumEnergy)
         {
            continue;
         }

         double g1g2Angle = grif1->GetPosition(145.).Angle(grif2->GetPosition(145.)) * 180. / TMath::Pi();

         // --- remove intraclover scatters
         if (g1g2Angle < minimumScatterAngle)
         {
            continue;
         }

         fH1[slot].at("angleDiagnostic")->Fill(g1g2Angle);

         // --- Third gamma
         for (auto g3 = g2 + 1; g3 < grif.GetSuppressedMultiplicity(&grifBgo); ++g3)
         {

            auto grif3 = grif.GetSuppressedHit(g3);

            double calibratedEnergyGrif3 = ApplySplitCalibration(grif3);

            // --- sets energy floor at minimum value;
            if (calibratedEnergyGrif3 < gammaMinimumEnergy)
            {
               continue;
            }

            double g1g3Angle = grif1->GetPosition(145.).Angle(grif3->GetPosition(145.)) * 180. / TMath::Pi();
            double g2g3Angle = grif2->GetPosition(145.).Angle(grif3->GetPosition(145.)) * 180. / TMath::Pi();

            // --- remove intraclover scatters
            if (g2g3Angle < minimumScatterAngle || g1g3Angle < minimumScatterAngle)
            {
               continue;
            }

            fH1[slot].at("angleDiagnostic")->Fill(g1g3Angle);
            fH1[slot].at("angleDiagnostic")->Fill(g2g3Angle);

            // --- Fourth gamma
            for (auto g4 = g3 + 1; g4 < grif.GetSuppressedMultiplicity(&grifBgo); ++g4)
            {

               auto grif4 = grif.GetSuppressedHit(g4);

               double calibratedEnergyGrif4 = ApplySplitCalibration(grif4);

               // --- sets energy floor at minimum value;
               if (calibratedEnergyGrif4 < gammaMinimumEnergy)
               {
                  continue;
               }

               double g1g4Angle = grif1->GetPosition(145.).Angle(grif4->GetPosition(145.)) * 180. / TMath::Pi();
               double g2g4Angle = grif2->GetPosition(145.).Angle(grif4->GetPosition(145.)) * 180. / TMath::Pi();
               double g3g4Angle = grif3->GetPosition(145.).Angle(grif4->GetPosition(145.)) * 180. / TMath::Pi();

               // --- remove intraclover scatters
               if (g1g4Angle < minimumScatterAngle || g2g4Angle < minimumScatterAngle || g3g4Angle < minimumScatterAngle)
               {
                  continue;
               }

               fH1[slot].at("angleDiagnostic")->Fill(g1g4Angle);
               fH1[slot].at("angleDiagnostic")->Fill(g2g4Angle);
               fH1[slot].at("angleDiagnostic")->Fill(g3g4Angle);

               double totalEnergy = grif1->GetEnergy() + grif2->GetEnergy() + grif3->GetEnergy() + grif4->GetEnergy();

               double totalEnergyMin = nuclearLevelEnergy - nuclearLevelEnergyBuffer;
               double totalEnergyMax = qValue;

               if (setTotalEnergyFloor)
               {
                  totalEnergyMin -= expectedTwoPhotonEnergy;
               }

               if (totalEnergy > totalEnergyMin && totalEnergy < totalEnergyMax)
               {
                  fH1[slot].at("gEnergyTotal")->Fill(totalEnergy);

                  // -- There may be a better way to do this but I can't think of one
                  std::vector<TGriffinHit *> hits = {grif1, grif2, grif3, grif4};
                  double twoPhotonTime = -1;
                  double timingHit, gateTime1, gateTime2;

                  // -- first check for two photon concidence
                  for (size_t i = 0; i < hits.size(); ++i)
                  {
                     double hit1Time = hits[i]->GetTime();
                     for (size_t j = i + 1; j < hits.size(); ++j)
                     {
                        double hit2Time = hits[j]->GetTime();
                        // -- timing is not symmetric so Abs() is not reasonable
                        if (hit1Time > hit2Time)
                        {
                           timingHit = hit1Time;
                           twoPhotonTime = hit1Time - hit2Time;
                        }
                        else
                        {
                           timingHit = hit2Time;
                           twoPhotonTime = hit2Time - hit1Time;
                        }

                        if (twoPhotonTime < twoPhotonCoincidenceTime)
                        {
                           double sumEnergy = hits[i]->GetEnergy() + hits[j]->GetEnergy();
                           double gamma1Energy = hits[i]->GetEnergy();
                           fH2[slot].at("ggTiming")->Fill(twoPhotonTime, sumEnergy);

                           // -- next check for delayed gate photons
                           // -- photon 3
                           for (size_t k = 0; k < hits.size(); ++k)
                           {
                              if (k != i && k != j)
                              {
                                 double hit3Time = hits[k]->GetTime();
                                 double hit3Energy = hits[k]->GetEnergy();

                                 if (hit3Time > timingHit)
                                 {
                                    gateTime1 = hit3Time - timingHit;
                                 }
                                 else
                                 {
                                    gateTime1 = timingHit - hit3Time;
                                 }

                                 // -- photon 4
                                 for (size_t l = k + 1; l < hits.size(); ++l)
                                 {
                                    if (l != i && l != j)
                                    {
                                       double hit4Time = hits[l]->GetTime();
                                       double hit4Energy = hits[l]->GetEnergy();

                                       if (hit4Time > timingHit)
                                       {
                                          gateTime2 = hit4Time - timingHit;
                                       }
                                       else
                                       {
                                          gateTime2 = timingHit - hit4Time;
                                       }

                                       if (gateTime1 > gateGammaDelayWindowStart && gateTime1 < gateGammaDelayWindowEnd && gateTime2 > gateGammaDelayWindowStart && gateTime2 < gateGammaDelayWindowEnd)
                                       {
                                          fH2[slot].at("ggggTiming")->Fill(gateTime1, sumEnergy);
                                          fH2[slot].at("ggggTiming")->Fill(gateTime2, sumEnergy);

                                          // -- select correct cascade
                                          // --- gate 3 first transition, gate 4 second transition
                                          if (hit3Energy > (gateEnergy2 - gateEnergyHalfWidth) && hit3Energy < (gateEnergy2 + gateEnergyHalfWidth))
                                          {
                                             fH2[slot].at("ggggSumEnergy")->Fill(sumEnergy, gamma1Energy);

                                             if (useSecondGate && hit4Energy > (gateEnergy1 - gateEnergyHalfWidth) && hit4Energy < (gateEnergy1 + gateEnergyHalfWidth))
                                             {
                                                fH2[slot].at("ggggSumEnergy2Gates")->Fill(sumEnergy, gamma1Energy);
                                             }
                                          } // end hit3 first transition, hit 4 second transition

                                          // --- gate 4 first transition, gate 3 second transition
                                          if (hit4Energy > (gateEnergy2 - gateEnergyHalfWidth) && hit4Energy < (gateEnergy2 + gateEnergyHalfWidth))
                                          {
                                             fH2[slot].at("ggggSumEnergy")->Fill(sumEnergy, gamma1Energy);

                                             if (useSecondGate && hit3Energy > (gateEnergy1 - gateEnergyHalfWidth) && hit3Energy < (gateEnergy1 + gateEnergyHalfWidth))
                                             {
                                                fH2[slot].at("ggggSumEnergy2Gates")->Fill(sumEnergy, gamma1Energy);
                                             }
                                          } // end hit3 first transition, hit 4 second transition

                                       } // end gateGammaDelayWindow
                                    }    // end photon 4
                                 }
                              } // end photon 3
                           }
                        } // end twoPhotonCoincidenceTime
                     }    // end photon 2
                  }       // end photon 1
               }          // end totalEnergy

            } // end fourth gamma
         }    // end third gamma
      }       // end second gamma
   }          // end first gamma
}

double QuadPhotonCoincidenceHelper::ApplySplitCalibration(TGriffinHit *grifHit)
{
   double charge = grifHit->GetCharge();
   int detector = grifHit->GetArrayNumber();
   double calibratedEnergy = 0;
   double check;

   //===== CALIBRATION PARAMETERS at low energy (up to 1.5 MeV) =====//
   float non_lin_lowEn[64] = {-4.68291e-07, -1.57739e-07, -1.08496e-06, -1.31358e-06, -1.80958e-06, 1.14446e-06, 2.57513e-06, -1.17114e-06, 6.40568e-07, 1.12764e-06, 1.03848e-06, -9.54139e-07, -3.06841e-06, 1.41572e-06, -1.01885e-06, -2.32572e-06, -2.48592e-06, -2.51741e-06, 3.02532e-06, -2.77584e-06, -8.07434e-07, 2.36235e-06, 1.32074e-06, 4.67746e-07, -2.29941e-06, 2.62618e-07, -3.71342e-07, -2.44712e-06, -3.79734e-06, -2.19376e-06, -2.15188e-06, 1.54592e-06, -1.15353e-06, -1.10749e-06, -1.32878e-06, 6.95103e-07, -5.5924e-06, 9.73702e-07, 2.04325e-06, -1.00954e-06, 1.22901e-06, -8.92667e-07, 9.69105e-08, 0, -8.19234e-07, 3.06149e-06, -5.17136e-07, -6.88177e-07, 0, 0, 0, 0, -1.94666e-06, 1.41071e-06, -2.24866e-07, 1.79207e-07, 6.40774e-07, -4.84088e-07, -1.4193e-06, 2.01336e-06, 0, 9.04935e-07, 0, 6.17711e-07};
   float gain_lowEn[64] = {1.24238, 1.1877, 1.10483, 1.19776, 1.37344, 1.31449, 1.36987, 1.33894, 1.34556, 1.39863, 1.37127, 1.34308, 1.26494, 1.27275, 1.27896, 1.2549, 1.27085, 1.3495, 1.31796, 1.26194, 1.24582, 1.2873, 1.24139, 1.18823, 1.2445, 1.25976, 1.28235, 1.27126, 1.3905, 1.38788, 1.39808, 1.39012, 1.31574, 1.3479, 1.35448, 1.34723, 1.39596, 1.35691, 1.35164, 1.33272, 1.3221, 1.2915, 1.33915, 0, 1.28197, 1.25589, 1.19808, 1.22684, 0, 0, 0, 0, 1.24241, 1.17052, 1.27919, 1.27081, 1.29606, 1.19637, 1.20728, 1.24926, 0, 1.24506, 0, 1.27434};
   float offset_lowEn[64] = {0.533014, 0.00587677, -0.0233012, -0.0938096, -0.443748, 0.209586, 1.16536, -0.370449, 0.217215, 0.723643, -0.0155166, -0.180287, -0.149212, 0.668699, -0.716038, -0.420987, -0.720938, -0.298372, 1.12722, -0.516756, -0.0431754, 0.357272, 0.40045, 0.0590197, -0.405724, 0.298866, 0.103216, -0.892448, -0.206407, -0.846835, 0.0483823, -0.0522332, -0.356088, 0.374197, -0.108962, 0.449794, -1.25681, -0.122639, -0.188769, 0.434839, 0.516881, -0.367146, -0.239394, -1, 0.183219, 0.941919, 0.0387385, -0.833897, -1, -1, -1, -1, -0.368016, 0.436138, -0.303957, -0.414384, 0.554212, -0.295704, 0.0788524, 0.6791, -1, 0.184132, -1, 0.171751};

   //===== CALIBRATION PARAMETERS at high energy (above 1.7 MeV)  =====//
   float non_lin_highEn[64] = {5.60468e-08, 3.23964e-07, 4.82274e-08, 2.26793e-07, -5.5512e-07, -1.54283e-07, 4.49278e-07, 2.95713e-07, -2.38824e-07, -1.53712e-07, 1.44637e-07, 1.33408e-07, 3.64764e-07, 4.48092e-07, 5.81778e-07, 2.65785e-07, 2.23885e-07, 2.38296e-08, -1.99468e-07, -8.54027e-08, 4.8801e-07, 1.84858e-07, 4.52791e-09, 2.77491e-07, 6.6792e-07, 3.0411e-07, 3.10376e-07, 8.04594e-07, -1.7345e-07, -2.05826e-07, 1.58825e-07, 1.72161e-07, 2.56301e-07, -2.19837e-07, -1.90938e-07, 8.18868e-08, 1.23719e-08, 1.47036e-07, -2.2117e-07, -4.37482e-09, 2.80366e-07, -1.50591e-07, 3.75329e-07, 0, -2.1391e-07, -2.94564e-07, 2.08752e-07, 2.83284e-07, 0, 0, 0, 0, 2.50096e-07, 2.34698e-07, 2.19423e-07, 2.85085e-07, -2.30279e-07, 8.14406e-09, 9.86432e-08, -1.47228e-07, 0, 6.05631e-07, 0, 3.54e-07};
   float gain_highEn[64] = {1.2415, 1.18651, 1.10342, 1.19464, 1.37372, 1.31689, 1.3718, 1.33683, 1.34747, 1.40103, 1.37191, 1.3419, 1.26006, 1.27288, 1.27604, 1.25135, 1.26707, 1.34645, 1.32318, 1.25877, 1.24299, 1.28985, 1.24319, 1.1879, 1.23901, 1.25884, 1.28072, 1.26508, 1.3881, 1.38596, 1.39597, 1.39164, 1.31358, 1.34801, 1.35419, 1.34848, 1.39004, 1.35734, 1.35497, 1.33234, 1.32255, 1.29123, 1.33792, 0, 1.28163, 1.26111, 1.19662, 1.2247, 0, 0, 0, 0, 1.23877, 1.17172, 1.27836, 1.27022, 1.29808, 1.19601, 1.20596, 1.25281, 0, 1.24334, 0, 1.27354};
   float offset_highEn[64] = {1.59789, 1.47661, 0.00234933, 1.81855, -1.94198, -1.07366, 1.4471, 0.477113, -1.22124, -0.405967, 0.0578852, -0.109667, 1.82889, 2.13944, 1.53645, 0.878211, 0.877631, 0.140732, -0.898719, -0.0329221, 2.15681, -0.302765, 0.41365, 0.907357, 2.84564, 1.55236, 1.99065, 3.09259, -0.159309, -1.1798, 0.398758, -0.695098, 0.935762, -0.553712, -0.930279, -0.0908738, -0.151376, 0.434992, -1.6428, -0.149951, 1.28008, 0.0854645, 1.37404, -1, 0.0496726, -0.718487, 0.924334, 0.679254, -1, -1, -1, -1, 1.79351, 0.984349, 0.0887879, 0.233899, -0.141109, -0.192702, -0.000216201, -0.239276, -1, 2.56014, -1, 1.48798};

   //===== CALIBRATION RANGE LIMITS   =====//
   float lowRange[64] = {1206.93, 1262.94, 1357.7, 1252.42, 1092.47, 1140.97, 1094.14, 1120.57, 1114.62, 1071.96, 1093.89, 1116.97, 1185.94, 1178.03, 1173.39, 1195.65, 1180.88, 1111.74, 1137.27, 1189.06, 1204.06, 1164.95, 1208, 1262.33, 1205.63, 1190.47, 1169.65, 1180.63, 1078.9, 1081.4, 1072.87, 1079.08, 1140.31, 1112.56, 1107.52, 1113.06, 1075.43, 1105.54, 1109.9, 1125.19, 1134.17, 1161.72, 1120.29, 0, 1169.93, 1193.62, 1251.97, 1223.33, 0, 0, 0, 0, 1207.63, 1281.11, 1172.85, 1180.68, 1156.93, 1254.04, 1242.4, 1200.17, 0, 1204.61, 0, 1176.95};
   float highRange[64] = {1367.91, 1431.33, 1538.72, 1419.39, 1238.09, 1293.12, 1240.14, 1269.94, 1263.25, 1214.96, 1239.74, 1265.88, 1344.06, 1335.17, 1329.76, 1355.03, 1338.25, 1259.95, 1289.02, 1347.54, 1364.6, 1320.32, 1369.11, 1430.65, 1366.34, 1349.23, 1325.61, 1337.96, 1222.73, 1225.5, 1215.92, 1222.95, 1292.32, 1260.94, 1255.17, 1261.51, 1218.7, 1252.94, 1257.87, 1275.26, 1285.44, 1316.58, 1269.64, 0, 1325.94, 1352.87, 1418.9, 1386.35, 0, 0, 0, 0, 1368.6, 1451.97, 1329.2, 1338.06, 1311.24, 1421.21, 1408.06, 1360.26, 0, 1365.25, 0, 1333.89};

   // low energy calibration
   if (charge < lowRange[detector - 1])
   {
      calibratedEnergy = offset_lowEn[detector - 1] + gain_lowEn[detector - 1] * charge + non_lin_lowEn[detector - 1] * charge * charge;
      calibratedEnergy += ((double)rand() / RAND_MAX - 0.5);
   }
   // mixed-region energy calibration
   else if (charge >= lowRange[detector - 1] && charge < highRange[detector - 1])
   {
      check = (double)rand() / RAND_MAX;
      if (check <= 0.5)
      {
         calibratedEnergy = offset_lowEn[detector - 1] + gain_lowEn[detector - 1] * charge + non_lin_lowEn[detector - 1] * charge * charge;
         calibratedEnergy += ((double)rand() / RAND_MAX - 0.5);
      }
      else
      {
         calibratedEnergy = offset_highEn[detector - 1] + gain_highEn[detector - 1] * charge + non_lin_highEn[detector - 1] * charge * charge;
         calibratedEnergy += ((double)rand() / RAND_MAX - 0.5);
      }
   }
   // high-energy calibration
   else if (charge >= highRange[detector - 1])
   {
      calibratedEnergy = offset_highEn[detector - 1] + gain_highEn[detector - 1] * charge + non_lin_highEn[detector - 1] * charge * charge;
      calibratedEnergy += ((double)rand() / RAND_MAX - 0.5);
   }

   return calibratedEnergy;

} // end ApplySplitCalibration

Addback_t QuadPhotonCoincidenceHelper::GetAddback(TGriffin &grif, TGriffinBgo &grifBgo)
{

   Addback_t addbackData;
   std::vector<Int_t> skip;
   Double_t energy = 0;
   Double_t energyCT = 0;

   //===== CROSSTALK COEFFICIENTS =====//
   float CTcoeff[16][4][4] = {0, -0.000456667, -0.00134526, -0.000777811, -0.00108748, 0, -0.000827391, -0.00169368, -0.00164193, -0.000657133, 0, -0.000911018, -0.000950389, -0.00126328, -0.000590419, 0, 0, -0.0019131, -0.00244241, -0.000930193, -0.00137179, 0, -0.000700445, -0.000578935, -0.00150983, -0.000580168, 0, -0.000695303, -0.00102758, -0.00073288, -0.0011868, 0, 0, -0.00172488, -0.00145357, -0.00105393, -0.000817887, 0, -0.000314545, -0.000129661, -0.00171077, -0.000907405, 0, -0.000802641, -0.00125896, -0.00105413, -0.000859705, 0, 0, -0.00126831, -0.00148369, -0.000453308, -0.0016181, 0, -0.000407675, -0.000624059, -0.00172661, -0.00101456, 0, -0.000911773, -0.00121472, -0.000782765, -0.000859423, 0, 0, -0.000797788, -0.00156457, -0.000539448, -0.00126944, 0, -0.000909532, -0.000893915, -0.00121084, -0.000318408, 0, -0.00060478, -0.000954965, -0.000521993, -0.00137538, 0, 0, -0.000953077, -0.00168826, -0.000510058, -0.00113708, 0, -0.00063038, -0.00020561, -0.00117509, -0.00049317, 0, -0.000545118, -0.000703774, -0.000545167, -0.0010178, 0, 0, -6.02183e-05, -0.00128467, -0.0001307, -0.00116513, 0, -0.000499161, -0.000786686, -0.00175129, -0.000925007, 0, -0.000924772, -0.00134405, -0.00121831, -0.00142467, 0, 0, -0.00148818, -0.00117151, -0.000916369, -0.00209092, 0, -0.000352559, -0.000604994, -0.002097, -0.00065074, 0, -0.000662739, -0.00140899, -0.000732452, -0.000608607, 0, 0, -0.00137666, -0.00143569, -0.0013483, -0.000606209, 0, -0.000476369, -0.000827634, -0.00186699, -0.000923926, 0, -0.00109345, -0.000880478, -0.00083046, -0.000985863, 0, 0, -0.00159223, -0.00196884, -0.00144414, -0.00108956, 0, -0.000655957, -0.000776109, -0.0010764, -0.000744261, 0, -0.00108938, -0.000478777, -0.00114861, -0.000577993, 0, 0, -0.0017583, -0.00175305, 0, -0.00130465, 0, -0.000854896, 0, -0.00147421, -0.00111744, 0, 0, 0, 0, 0, 0, 0, -0.00185339, -0.00179959, -0.00140308, -0.00106553, 0, -0.000460575, -0.000782898, -0.00125028, -0.000876984, 0, -0.000946554, -0.000803364, -0.00116938, -0.00066612, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.00142392, -0.00108605, -0.000951035, -0.00127426, 0, -0.00018089, -0.000133508, -0.00174588, -0.00109485, 0, -0.000955283, -0.000956457, -0.00148811, -0.000824678, 0, 0, -0.00160278, -0.0018626, -0.00123516, -0.00155274, 0, -0.000654319, -0.00149267, -0.0015994, -0.000849707, 0, -0.0012822, -0.00108166, -0.00107383, -0.00113472, 0, 0, 0, 0, 0, 0, 0, 0, -0.000415355, 0, 0, 0, 0, 0, 0.000155135, 0, 0};

   //===== CALIBRATION PARAMETERS after Crosstalk correction =====//
   float nonLinearCoeffAfterCT[64] = {-6.26504e-08, 4.72381e-09, 2.82209e-08, -1.55191e-09, -3.26524e-08, -5.02967e-08, -3.68873e-08, -2.4608e-08, -4.44182e-08, -1.10566e-08, 8.18358e-09, -3.27018e-08, 2.91854e-08, -1.80291e-08, 2.88679e-08, 5.90686e-08, -2.07137e-08, 3.99564e-08, -9.07573e-08, 1.70897e-08, 3.96081e-09, -9.95286e-09, -1.97398e-08, -4.51152e-09, -4.46509e-09, -2.47542e-08, -1.05947e-08, -3.91987e-08, 2.71658e-08, 2.77296e-08, -8.01869e-09, -2.77003e-08, 1.34653e-08, 1.9013e-08, 8.69222e-08, 7.48364e-09, 4.73486e-08, -5.28833e-08, -1.33879e-08, -1.62006e-09, -4.1543e-08, 8.47716e-09, -3.52811e-09, 0, 6.49113e-09, -6.43855e-08, 3.26553e-08, -2.19385e-08, 0, 0, 0, 0, -2.97968e-08, -4.74955e-08, -1.2935e-08, -1.66497e-08, -2.51128e-08, -5.38882e-09, 8.03246e-09, -3.6355e-08, 0, -2.03964e-08, 0, -2.36386e-08};
   float gainCoeffAfterCT[64] = {0.999987, 0.999931, 0.99986, 0.99992, 1.00003, 1.00009, 1.00005, 0.999997, 1.00003, 1, 0.999966, 1.00002, 0.999819, 0.999977, 0.999869, 0.999815, 0.999942, 0.999964, 1.00016, 0.999906, 0.999945, 1.00003, 0.999972, 1.00002, 0.999874, 0.99997, 0.999931, 0.999992, 0.99976, 0.999857, 1.00005, 1.00004, 0.99994, 0.999886, 0.999806, 0.999946, 0.999797, 1.00004, 1.00003, 0.999903, 1.00004, 0.999838, 0.999943, -1, 0.999965, 1.00011, 0.999946, 1.00002, -1, -1, -1, -1, 0.999937, 1.00003, 1.00005, 1.00001, 0.999992, 0.999956, 0.99992, 1.00004, -1, 1.00006, -1, 1.00006};
   float offsetCoeffAfterCT[64] = {-0.0190844, 6.55963e-05, 0.0194426, 1.48277e-05, -0.0026479, -0.0249665, -0.0223625, 0.0036716, -0.00814533, -0.020074, -0.0114052, -0.00846783, 0.00414635, -0.00427748, 0.0158318, 0.0231064, 0.00909008, 0.00203632, -0.0467592, 0.0164958, -0.00683261, -0.0256637, -0.0109006, -0.0157567, 0.0151583, 0.00374787, -0.000758243, 0.0133152, 0.00857865, 0.0218093, -0.0270507, -0.0168875, -0.00786702, -0.0159949, -0.00414951, -0.0167593, 0.0324905, -0.00514884, -0.0116946, -0.000866309, -0.0147094, 0.0248591, -0.000593264, 0, 0.00350045, -0.0256675, -0.00729034, -0.00882748, 0, 0, 0, 0, 0.0104674, -0.00231033, -0.0049287, -0.00736557, -0.0201756, -0.00295571, -0.00799252, -0.0277857, 0, -0.0171159, 0, -0.0141646};

   for (auto g1 = 0; g1 < grif.GetSuppressedMultiplicity(&grifBgo); ++g1)
   {
      auto grif1 = grif.GetSuppressedHit(g1);
      Int_t det = grif1->GetArrayNumber();

      double calibratedEnergyGrif1 = ApplySplitCalibration(grif1);
      // sets energy floor at minimum value;
      if (calibratedEnergyGrif1 < gammaMinimumEnergy)
      {
         continue;
      }

      if (skip.size() > 0 && find(skip.begin(), skip.end(), g1) != skip.end())
      {
         continue; // skip events which were the second hit in an addback hit
      }

      energy = calibratedEnergyGrif1;
      energyCT = calibratedEnergyGrif1;

      for (auto g2 = g1 + 1; g2 < grif.GetSuppressedMultiplicity(&grifBgo); ++g2)
      {
         auto grif2 = grif.GetSuppressedHit(g2);
         double calibratedEnergyGrif2 = ApplySplitCalibration(grif2);
         // sets energy floor at minimum value;
         if (calibratedEnergyGrif2 < gammaMinimumEnergy)
         {
            continue;
         }

         if (DefaultGriffinAddback(grif1, grif2))
         {
            skip.push_back(g2);
            if (calibratedEnergyGrif2 > 0)
            {
               energy += calibratedEnergyGrif2;
               energyCT -= CTcoeff[grif1->GetDetector() - 1][grif1->GetCrystal()][grif2->GetCrystal()] * calibratedEnergyGrif2;
               energyCT += (calibratedEnergyGrif2 - CTcoeff[grif1->GetDetector() - 1][grif2->GetCrystal()][grif1->GetCrystal()] * calibratedEnergyGrif1);
            }
         }
      }

      // re-alignment of energy values after CrossTalk correction
      energyCT = offsetCoeffAfterCT[det - 1] + gainCoeffAfterCT[det - 1] * energyCT + nonLinearCoeffAfterCT[det - 1] * energyCT * energyCT;

      addbackData.energy.push_back(energy);
      addbackData.energyCTCorrected.push_back(energyCT);
      addbackData.time.push_back(grif1->GetTime());
      addbackData.timestamp.push_back(grif1->GetTimeStamp());
      addbackData.clover.push_back(grif1->GetDetector());
   }
   skip.clear();

   return addbackData;
} // end GetAddback