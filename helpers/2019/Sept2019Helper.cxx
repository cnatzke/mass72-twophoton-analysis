#include "Sept2019Helper.hh"
#include <stdlib.h> // rand() function

double gbLow = -50.;   // min. time difference gamma-beta
double gbHigh = 500.;  // max. time difference gamma-beta
double ggHigh = 400.;  // max. absolute time difference gamma-gamma
double bgLow = 1000.;  // min. time difference gamma-gamma background
double bgHigh = 2000.; // max. time difference gamma-gamma background

double beta_gs_prompt_min = 200.;   // ns
double beta_gs_prompt_max = 350.;   // ns
double beta_gs_delayed_min = 350.;  // ns
double beta_gs_delayed_max = 2000.; // ns
double two_photon_time_max = 70.;   // ns, max time diff for two-photon events
double gamma_energy_min = 0.;
double gamma_energy_max = 3000.;

double zds_kvalue_default = 60;
double zds_minimum_energy = 10;  // keV
double hpge_minimum_energy = 10; // keV

void Sept2019Helper::CreateHistograms(unsigned int slot)
{

   // for each angle (and the sum) we want
   // for single crystal and addback
   // for prompt and delayed coincidences
   //*--------------------------------------------------
   //* Energy-Angle Matrices
   //*--------------------------------------------------
   // for (int i = 0; i < static_cast<int>(fAngleCombinations.size()); ++i)
   // {
   //    fH2[slot][Form("index_%02d_prompt", i)] = new TH2D(
   //        Form("index_%02d_prompt", i),
   //        Form("%.1f^{o}: #gamma-#gamma_{sum} prompt, |#Deltat_{#gamma-#gamma}| < %.1f;#gamma_{sum};#gamma_{1}", fAngleCombinations[i].first, two_photon_time_max),
   //        gamma_energy_max, gamma_energy_min, gamma_energy_max,
   //        gamma_energy_max, gamma_energy_min, gamma_energy_max);
   //    fH2[slot][Form("index_%02d_delayed", i)] = new TH2D(
   //        Form("index_%02d_delayed", i),
   //        Form("%.1f^{o}: #gamma-#gamma_{sum} delayed, |#Deltat_{#gamma-#gamma}| < %.1f;#gamma_{sum};#gamma_1", fAngleCombinations[i].first, two_photon_time_max),
   //        gamma_energy_max, gamma_energy_min, gamma_energy_max,
   //        gamma_energy_max, gamma_energy_min, gamma_energy_max);

   //    fH2[slot][Form("index_%02d_prompt_compton", i)] = new TH2D(
   //        Form("index_%02d_prompt_compton", i),
   //        Form("%.1f^{o}: #gamma-#gamma_{sum} prompt Compton, |#Deltat_{#gamma-#gamma}| < %.1f;#gamma_{sum};#gamma_1", fAngleCombinations[i].first, two_photon_time_max),
   //        gamma_energy_max, gamma_energy_min, gamma_energy_max,
   //        gamma_energy_max, gamma_energy_min, gamma_energy_max);
   //    fH2[slot][Form("index_%02d_delayed_compton", i)] = new TH2D(
   //        Form("index_%02d_delayed_compton", i),
   //        Form("%.1f^{o}: #gamma-#gamma_{sum} delayed Compton, |#Deltat_{#gamma-#gamma}| < %.1f;#gamma_{sum};#gamma_1", fAngleCombinations[i].first, two_photon_time_max),
   //        gamma_energy_max, gamma_energy_min, gamma_energy_max,
   //        gamma_energy_max, gamma_energy_min, gamma_energy_max);
   // }
   /*
   // Addback
   for(int i = 0; i < static_cast<int>(fAngleCombinationsAddback.size()); ++i) {
      fH2[slot][Form("index_%02d_prompt_addback", i)] = new TH2D(
         Form("index_%02d_prompt_addback", i),
         Form("%.1f^{o}: #gamma-#gamma_{sum} prompt with addback, |#Deltat_{#gamma-#gamma}| < %.1f", fAngleCombinationsAddback[i].first, two_photon_time_max),
         gamma_energy_max, gamma_energy_min, gamma_energy_max,
         gamma_energy_max, gamma_energy_min, gamma_energy_max);
      fH2[slot][Form("index_%02d_delayed_addback", i)] = new TH2D(
         Form("index_%02d_delayed_addback", i),
         Form("%.1f^{o}: #gamma-#gamma_{sum} delayed with addback, |#Deltat_{#gamma-#gamma}| < %.1f", fAngleCombinationsAddback[i].first, two_photon_time_max),
         gamma_energy_max, gamma_energy_min, gamma_energy_max,
         gamma_energy_max, gamma_energy_min, gamma_energy_max);
   }
   */

   // 1D histograms
   fH1[slot]["sumEnergy"] = new TH1D("sumEnergy", "#gamma sum energy;Sum Energy", gamma_energy_max, gamma_energy_min, gamma_energy_max);
   fH1[slot]["sumEnergyPrompt"] = new TH1D("sumEnergyPrompt", "Prompt #gamma sum energy;Sum Energy", gamma_energy_max, gamma_energy_min, gamma_energy_max);
   fH1[slot]["sumEnergyDelayed"] = new TH1D("sumEnergyDelayed", "Delayed #gamma sum energy;Sum Energy", gamma_energy_max, gamma_energy_min, gamma_energy_max);

   // 2D histograms
   fH2[slot]["gammaEnergyChannel"] = new TH2D("gammaEnergyChannel", "HPGe crystal number vs #gamma energy", 65, 0., 65., gamma_energy_max, gamma_energy_min, gamma_energy_max);

   fH2[slot]["gammaGammaSum"] = new TH2D(
       "gammaGammaSum",
       Form("#gamma-#gamma, |#Deltat_{#gamma-#gamma}| < %.1f ns;#gamma_sum", two_photon_time_max),
       gamma_energy_max, gamma_energy_min, gamma_energy_max,
       gamma_energy_max, gamma_energy_min, gamma_energy_max);
   fH2[slot]["gammaSumGammaPrompt"] = new TH2D(
       "gammaSumGammaPrompt",
       Form("#gamma_{sum}-#gamma_1 prompt, |#Deltat_{#gamma-#gamma}| < %.1f ns;#gamma_{sum};#gamma_1", two_photon_time_max),
       gamma_energy_max, gamma_energy_min, gamma_energy_max,
       gamma_energy_max, gamma_energy_min, gamma_energy_max);
   fH2[slot]["gammaSumGammaDelayed"] = new TH2D(
       "gammaSumGammaDelayed",
       Form("#gamma_{sum}-#gamma_1 delayed, |#Deltat_{#gamma-#gamma}| < %.1f ns;#gamma_{sum};#gamma_1", two_photon_time_max),
       gamma_energy_max, gamma_energy_min, gamma_energy_max,
       gamma_energy_max, gamma_energy_min, gamma_energy_max);

   fH2[slot]["gammaSumGammaPromptCompton"] = new TH2D(
       "gammaSumGammaPromptCompton",
       Form("#gamma_{sum}-#gamma_1 prompt Compton, |#Deltat_{#gamma-#gamma}| < %.1f ns;#gamma_{sum};#gamma_1", two_photon_time_max),
       gamma_energy_max, gamma_energy_min, gamma_energy_max,
       gamma_energy_max, gamma_energy_min, gamma_energy_max);
   fH2[slot]["gammaSumGammaDelayedCompton"] = new TH2D(
       "gammaSumGammaDelayedCompton",
       Form("#gamma_{sum}-#gamma_1 delayed Compton, |#Deltat_{#gamma-#gamma}| < %.1f ns;#gamma_{sum};#gamma_1", two_photon_time_max),
       gamma_energy_max, gamma_energy_min, gamma_energy_max,
       gamma_energy_max, gamma_energy_min, gamma_energy_max);

   fH2[slot]["gammaSumGammaPromptTotal"] = new TH2D(
       "gammaSumGammaPromptTotal",
       Form("#gamma_{sum}-#gamma_1 prompt total, |#Deltat_{#gamma-#gamma}| < %.1f ns;#gamma_{sum};#gamma_1", two_photon_time_max),
       gamma_energy_max, gamma_energy_min, gamma_energy_max,
       gamma_energy_max, gamma_energy_min, gamma_energy_max);
   fH2[slot]["gammaSumGammaDelayedTotal"] = new TH2D(
       "gammaSumGammaDelayedTotal",
       Form("#gamma_{sum}-#gamma_1 delayed total, |#Deltat_{#gamma-#gamma}| < %.1f ns;#gamma_{sum};#gamma_1", two_photon_time_max),
       gamma_energy_max, gamma_energy_min, gamma_energy_max,
       gamma_energy_max, gamma_energy_min, gamma_energy_max);

   // Gamma-gamma timing coincidence comparison
   fH2[slot]["gammaSumTimingPrompt"] = new TH2D("gammaSumTimingPrompt",
                                                "#gamma Sum Prompt Timing [275, 425]",
                                                155, 0, 155,
                                                gamma_energy_max, gamma_energy_min, gamma_energy_max);
   fH2[slot]["gammaSumTimingPromptCompton"] = new TH2D("gammaSumTimingPromptCompton",
                                                       "#gamma Sum Prompt Timing [275, 425] Compton Tagged",
                                                       155, 0, 155,
                                                       gamma_energy_max, gamma_energy_min, gamma_energy_max);
   fH2[slot]["gammaSumTimingPromptTotal"] = new TH2D("gammaSumPromptTimingTotal",
                                                     "#gamma Sum Prompt Timing [275, 425] All Events",
                                                     155, 0, 155,
                                                     gamma_energy_max, gamma_energy_min, gamma_energy_max);
   fH2[slot]["gammaSumTimingDelayed"] = new TH2D("gammaSumTimingDelayed",
                                                 "#gamma Sum Delayed Timing [425, 2000]",
                                                 155, 0, 155,
                                                 gamma_energy_max, gamma_energy_min, gamma_energy_max);
   fH2[slot]["gammaSumTimingDelayedCompton"] = new TH2D("gammaSumTimingDelayedCompton",
                                                        "#gamma Sum Delayed Timing [425, 2000] Compton Tagged",
                                                        155, 0, 155,
                                                        gamma_energy_max, gamma_energy_min, gamma_energy_max);
   fH2[slot]["gammaSumTimingDelayedTotal"] = new TH2D("gammaSumTimingDelayedTotal",
                                                      "#gamma Sum Delayed Timing [425, 2000] All Events",
                                                      155, 0, 155,
                                                      gamma_energy_max, gamma_energy_min, gamma_energy_max);

   // plus hitpatterns for gamma-gamma and beta-gamma for single crystals
   fH2[slot]["gammaGammaHP"] = new TH2D("gammaGammaHP", "#gamma-#gamma hit pattern", 65, 0., 65., 65, 0., 65.);
   fH2[slot]["betaGammaHP"] = new TH2D("betaGammaHP", "#beta-#gamma hit pattern", 21, 0., 21., 65, 0., 65.);
   // fH2[slot]["addbackAddbackHP"] =
   //     new TH2D("addbackAddbackHP", "#gamma-#gamma hit pattern with addback", 65, 0., 65., 65, 0., 65.);
   // fH2[slot]["betaAddbackHP"] =
   //     new TH2D("betaAddbackHP", "#beta-#gamma hit pattern with addback", 21, 0., 21., 65, 0., 65.);

   // additionally 1D spectra of gammas
   // for single crystal and addback
   // with and without coincident betas
   fH1[slot]["gammaEnergy"] = new TH1D("gammaEnergy", "#gamma Singles", 12000, 0, 3000);
   fH1[slot]["gammaEnergyBeta"] =
       new TH1D("gammaEnergyBeta", "#gamma singles in rough #beta coincidence", 12000, 0, 3000);
   // fH1[slot]["addbackEnergy"] = new TH1D("addbackEnergy", "#gamma singles with addback", 12000, 0, 3000);
   // fH1[slot]["addbackEnergyBeta"] =
   //     new TH1D("addbackEnergyBeta", "#gamma singles with addback in rough #beta coincidence", 12000, 0, 3000);

   //* ------- Timing -------
   fH1[slot]["gammaGammaTiming"] = new TH1D("gammaGammaTiming", "#Deltat_{#gamma-#gamma}", 2000, 0., 2000.);
   fH2[slot]["betaGammaTimingChannel"] = new TH2D("betaGammaTimingChannel", "#Deltat_{#beta-#gamma}", 2000, -1000., 1000., 64, 0.5, 64.5);
   fH1[slot]["gammaBetaTiming"] = new TH1D("gammaBetaTiming", "Beta-2Gamma #Deltat", 1500, 0, 1500);
   fH1[slot]["ggTiming"] = new TH1D("ggTiming", "Diagnostic timing, Two-Photon Coincidence)", 300, -150, 150);
   fH1[slot]["promptTiming"] = new TH1D("promptTiming", "Diagnostic timing (prompt)", 1000, 0, 1000);
   fH1[slot]["delayedTiming"] = new TH1D("delayedTiming", "Diagnostic timing (delayed)", 2500, 0, 2500);
   // fH1[slot]["addbackAddbackTiming"] = new TH1D("addbackAddbackTiming", "#Deltat_{#addback-#addback}", 2000, 0., 3000.);
   // fH1[slot]["betaAddbackTiming"] = new TH1D("betaAddbackTiming", "#Deltat_{#beta-#gamma}", 2000, -1000., 1000.);

   //* ------- Diagnostic -------
   fH1[slot]["gammaMultiplicity"] = new TH1D("gammaMultiplicity", "HPGe multiplicity", 10, 0, 10);
   fH1[slot]["zdsKValue"] = new TH1D("zdsKValue", "ZDS K-Value", 1500, 0, 1500);
   fH2[slot]["calibrationDiagnosticMatrix"] = new TH2D("calibrationDiagnosticMatrix", "Diagnostic calibrations;Calibrated;Raw", gamma_energy_max, gamma_energy_min, gamma_energy_max, gamma_energy_max, gamma_energy_min, gamma_energy_max);
   fH2[slot]["griffinZdsTS"] = new TH2F("griffinZdsTS", "GRIFFIN crystal vs. GRIFFIN-ZDS timestamp difference (suppressed addback);#DeltaTS_{GRIFFIN-ZDS}", 200, -1000., 1000., 64, 0.5, 64.5);
   fH2[slot]["griffinZdsTime"] = new TH2F("griffinZdsTime", "GRIFFIN crystal vs. GRIFFIN-ZDS timing (suppressed addback);#Deltat_{GRIFFIN-ZDS}", 2000, -1000., 1000., 64, 0.5, 64.5);

   fH2[slot]["betaGammaTimingMatrix"] = new TH2D(
       "betaGammaTimingMatrix",
       "#Deltat beta gamma timing",
       2000, -1000, 1000,
       gamma_energy_max, gamma_energy_min, gamma_energy_max);
   fH2[slot]["betaGammaTimingMatrixPileup"] = new TH2D(
       "betaGammaTimingMatrixPileup",
       "#Deltat beta gamma timing with pileup",
       3000, -1500, 1500,
       gamma_energy_max, gamma_energy_min, gamma_energy_max);
}

void Sept2019Helper::Exec(unsigned int slot, TGriffin &grif, TGriffinBgo &grifBgo, TZeroDegree &zds)
{

   //*--------------------------------------------------
   //* Filling Histograms
   //*--------------------------------------------------
   // * First gamma
   for (auto g1 = 0; g1 < grif.GetSuppressedMultiplicity(&grifBgo); ++g1)
   {
      auto grif1 = grif.GetSuppressedHit(g1);

      double calibratedEnergyGrif1 = ApplySplitCalibration(grif1);

      // sets energy floor at minimum value;
      if (calibratedEnergyGrif1 < hpge_minimum_energy)
      {
         continue;
      }

      fH2[slot].at("calibrationDiagnosticMatrix")->Fill(calibratedEnergyGrif1, grif1->GetEnergy());
      fH2[slot].at("gammaEnergyChannel")->Fill(grif1->GetArrayNumber(), calibratedEnergyGrif1);

      for (int z = 0; z < zds.GetMultiplicity(); ++z)
      {
         auto zds1 = zds.GetZeroDegreeHit(z);

         fH1[slot].at("zdsKValue")->Fill(zds1->GetKValue());

         if (zds1->GetKValue() != zds_kvalue_default)
         {
            continue;
         }
         // sets energy floor at minimum value;
         if (zds1->GetEnergy() < zds_minimum_energy)
         {
            continue;
         }

         fH2[slot].at("betaGammaTimingChannel")->Fill(zds1->GetTime() - grif1->GetTime(), grif1->GetArrayNumber());
         fH2[slot].at("betaGammaTimingMatrix")->Fill(zds1->GetTime() - grif1->GetTime(), calibratedEnergyGrif1);
      } // end beta tag

      // *============ SECOND GAMMA ============ //
      for (auto g2 = g1 + 1; g2 < grif.GetSuppressedMultiplicity(&grifBgo); ++g2)
      {
         auto grif2 = grif.GetGriffinHit(g2);

         double calibratedEnergyGrif2 = ApplySplitCalibration(grif2);

         if (calibratedEnergyGrif2 < hpge_minimum_energy)
         {
            continue;
         }

         double angle = grif1->GetPosition().Angle(grif2->GetPosition()) * 180. / TMath::Pi();
         if (angle < 0.0001)
            continue;
         auto angleIndex = fAngleMap.lower_bound(angle - 0.0005);
         double ggTime = grif1->GetTime() - grif2->GetTime();

         fH1[slot].at("gammaGammaTiming")->Fill(TMath::Abs(ggTime));
         fH2[slot].at("gammaGammaHP")->Fill(grif1->GetArrayNumber(), grif2->GetArrayNumber());

         // *============ GAMMA-GAMMA TIMING ============ //
         bool gammaCoincidenceCheck = false;
         if (gammaCoincidenceCheck)
         {
            if (TMath::Abs(ggTime) < 150)
            {
               auto sum_energy = calibratedEnergyGrif1 + calibratedEnergyGrif2;
               // beta tag
               for (auto z = 0; z < zds.GetMultiplicity(); ++z)
               {
                  auto zds1 = zds.GetHit(z);

                  // select good zds hits
                  if (zds1->GetKValue() != zds_kvalue_default)
                  {
                     continue;
                  }
                  // sets energy floor at minimum value;
                  if (zds1->GetEnergy() < zds_minimum_energy)
                  {
                     continue;
                  }

                  // time difference
                  auto grif1_beta_time_diff = TMath::Abs(zds1->GetTime() - grif1->GetTime());
                  auto grif2_beta_time_diff = TMath::Abs(zds1->GetTime() - grif2->GetTime());

                  bool promptBetaCoincidence = false;
                  bool delayedBetaCoincidence = false;

                  if (beta_gs_prompt_min <= grif1_beta_time_diff && grif1_beta_time_diff <= beta_gs_prompt_max)
                  {
                     promptBetaCoincidence = true;
                  }
                  else if (beta_gs_prompt_min <= grif2_beta_time_diff && grif2_beta_time_diff <= beta_gs_prompt_max)
                  {
                     promptBetaCoincidence = true;
                  }
                  else if (beta_gs_delayed_min <= grif1_beta_time_diff && grif1_beta_time_diff <= beta_gs_delayed_max)
                  {
                     delayedBetaCoincidence = true;
                  }
                  else if (beta_gs_delayed_min <= grif2_beta_time_diff && grif2_beta_time_diff <= beta_gs_delayed_max)
                  {
                     delayedBetaCoincidence = true;
                  }

                  if (promptBetaCoincidence && delayedBetaCoincidence)
                  {
                     std::cout << "We have a problem here" << std::endl;
                     std::cout << grif1_beta_time_diff << ", " << grif2_beta_time_diff << std::endl;
                  }

                  // prompt :
                  if (promptBetaCoincidence)
                  {
                     fH1[slot].at("promptTiming")->Fill(grif1_beta_time_diff);
                     fH1[slot].at("promptTiming")->Fill(grif2_beta_time_diff);
                     fH2[slot].at("gammaSumTimingPromptTotal")->Fill(TMath::Abs(TMath::Abs(ggTime)), sum_energy);
                     // if (compton_tag)
                     // {
                     //    fH2[slot].at("gammaSumTimingPromptCompton")->Fill(TMath::Abs(ggTime), sum_energy);
                     // }
                     // else
                     // {
                     //    fH2[slot].at("gammaSumTimingPrompt")->Fill(TMath::Abs(ggTime), sum_energy);
                     // }
                  } // end prompt

                  // delayed :
                  if (delayedBetaCoincidence)
                  {
                     fH1[slot].at("delayedTiming")->Fill(grif1_beta_time_diff);
                     fH1[slot].at("delayedTiming")->Fill(grif2_beta_time_diff);
                     fH2[slot].at("gammaSumTimingDelayedTotal")->Fill(TMath::Abs(ggTime), sum_energy);
                     // if (compton_tag)
                     // {
                     //    fH2[slot].at("gammaSumTimingDelayedCompton")->Fill(TMath::Abs(ggTime), sum_energy);
                     // }
                     // else
                     // {
                     //    fH2[slot].at("gammaSumTimingDelayed")->Fill(TMath::Abs(ggTime), sum_energy);
                     // }
                  } // end delayed
               }    // end beta tag
            }       // end gamma timing checks
         }          // end gammaCoincidenceCheck

         // * ------- TWO-PHOTON COINCIDENCE -------
         if (TMath::Abs(ggTime) < two_photon_time_max)
         {
            auto sum_energy = calibratedEnergyGrif1 + calibratedEnergyGrif2;
            fH1[slot].at("sumEnergy")->Fill(sum_energy);
            fH1[slot].at("ggTiming")->Fill(ggTime);

            // check for Compton Scatter
            double sum_energy_max = 5000;
            if (sum_energy > sum_energy_max - 1)
            {
               continue;
            }

            // beta tag
            for (auto z2 = 0; z2 < zds.GetMultiplicity(); ++z2)
            {
               bool grif1_delayed = false;
               bool grif2_delayed = false;

               auto zds1 = zds.GetHit(z2);

               // // select good zds hits
               if (zds1->GetKValue() != zds_kvalue_default)
               {
                  continue;
               }
               // sets energy floor at minimum value;
               if (zds1->GetEnergy() < zds_minimum_energy)
               {
                  continue;
               }

               // fH1[slot].at("zds_kvalue")->Fill(zds1->GetKValue());

               auto grif1_beta_time_diff = TMath::Abs(zds1->GetTime() - grif1->GetTime());
               auto grif2_beta_time_diff = TMath::Abs(zds1->GetTime() - grif2->GetTime());

               bool promptBetaCoincidence = false;
               bool delayedBetaCoincidence = false;

               if (beta_gs_prompt_min <= grif1_beta_time_diff && grif1_beta_time_diff <= beta_gs_prompt_max)
               {
                  promptBetaCoincidence = true;
               }
               else if (beta_gs_prompt_min <= grif2_beta_time_diff && grif2_beta_time_diff <= beta_gs_prompt_max)
               {
                  promptBetaCoincidence = true;
               }
               else if (beta_gs_delayed_min <= grif1_beta_time_diff && grif1_beta_time_diff <= beta_gs_delayed_max)
               {
                  delayedBetaCoincidence = true;
               }
               else if (beta_gs_delayed_min <= grif2_beta_time_diff && grif2_beta_time_diff <= beta_gs_delayed_max)
               {
                  delayedBetaCoincidence = true;
               }

               if (promptBetaCoincidence && delayedBetaCoincidence)
               {
                  std::cout << "We have a problem here" << std::endl;
                  std::cout << "hit 1 beta-gamma time: " << grif1_beta_time_diff << ", hit 2 beta-gamma time diff: " << grif2_beta_time_diff << std::endl;
               }

               // time difference
               fH1[slot].at("gammaBetaTiming")->Fill(grif1_beta_time_diff);
               fH1[slot].at("gammaBetaTiming")->Fill(grif2_beta_time_diff);

               bool compton_tag = GetComptonTag(calibratedEnergyGrif1, calibratedEnergyGrif2, angleIndex->second);

               // prompt :
               if (promptBetaCoincidence)
               {
                  fH1[slot].at("promptTiming")->Fill(grif1_beta_time_diff);

                  fH2[slot].at("gammaSumGammaPromptTotal")->Fill(sum_energy, calibratedEnergyGrif1);
                  fH2[slot].at("gammaSumGammaPromptTotal")->Fill(sum_energy, calibratedEnergyGrif2);

                  if (compton_tag)
                  {
                     // fH2[slot].at(Form("index_%02d_prompt_compton", angleIndex->second))->Fill(sum_energy, calibratedEnergyGrif1);
                     fH2[slot].at("gammaSumGammaPromptCompton")->Fill(sum_energy, calibratedEnergyGrif1);
                     fH2[slot].at("gammaSumGammaPromptCompton")->Fill(sum_energy, calibratedEnergyGrif2);
                  }
                  else
                  {
                     fH1[slot].at("sumEnergyPrompt")->Fill(sum_energy);
                     // fH2[slot].at(Form("index_%02d_prompt", angleIndex->second))->Fill(sum_energy, calibratedEnergyGrif1);
                     fH2[slot].at("gammaSumGammaPrompt")->Fill(sum_energy, calibratedEnergyGrif1);
                     fH2[slot].at("gammaSumGammaPrompt")->Fill(sum_energy, calibratedEnergyGrif2);
                  }

               } // end prompt

               if (delayedBetaCoincidence)
               {
                  fH1[slot].at("delayedTiming")->Fill(grif1_beta_time_diff);

                  fH2[slot].at("gammaSumGammaDelayedTotal")->Fill(sum_energy, calibratedEnergyGrif1);
                  fH2[slot].at("gammaSumGammaDelayedTotal")->Fill(sum_energy, calibratedEnergyGrif2);

                  if (compton_tag)
                  {
                     // fH2[slot].at(Form("index_%02d_delayed_compton", angleIndex->second))->Fill(sum_energy, calibratedEnergyGrif1);
                     fH2[slot].at("gammaSumGammaDelayedCompton")->Fill(sum_energy, calibratedEnergyGrif1);
                     fH2[slot].at("gammaSumGammaDelayedCompton")->Fill(sum_energy, calibratedEnergyGrif2);
                  }
                  else
                  {
                     fH1[slot].at("sumEnergyDelayed")->Fill(sum_energy);
                     // fH2[slot].at(Form("index_%02d_delayed", angleIndex->second))->Fill(sum_energy, calibratedEnergyGrif1);
                     fH2[slot].at("gammaSumGammaDelayed")->Fill(sum_energy, calibratedEnergyGrif1);
                     fH2[slot].at("gammaSumGammaDelayed")->Fill(sum_energy, calibratedEnergyGrif2);
                  }

               } // end delayed
            }    // end beta tag
         }       // end two-photon timing
      }          // end second gamma
   }             // end singles suppressed
}

bool Sept2019Helper::GetComptonTag(double energy1, double energy2, int angle_index)
{
   std::vector<double> comptonLimits(4);
   bool possible_compton = false;
   GetComptonLimits(energy1 + energy2, angle_index, comptonLimits);

   if (!possible_compton && comptonLimits.at(0) <= energy1 && energy1 <= comptonLimits.at(1))
   {
      possible_compton = true;
   }
   else if (!possible_compton && comptonLimits.at(3) <= energy1 && energy1 <= comptonLimits.at(2))
   {
      possible_compton = true;
   }
   else if (!possible_compton && comptonLimits.at(0) <= energy2 && energy2 <= comptonLimits.at(1))
   {
      possible_compton = true;
   }
   else if (!possible_compton && comptonLimits.at(3) <= energy2 && energy2 <= comptonLimits.at(2))
   {
      possible_compton = true;
   }

   return possible_compton;

} // end GetComptonTag

void Sept2019Helper::GetComptonLimits(double energy, int index, std::vector<double> &vec)
{
   // first get angular index
   struct comptonData indexData = fComptonMapping.at(index);

   // then get energy and bands
   int energy_index = round(energy);
   vec.at(0) = indexData.comptonLimit0.at(energy_index);
   vec.at(1) = indexData.comptonLimit1.at(energy_index);
   vec.at(2) = indexData.comptonLimit2.at(energy_index);
   vec.at(3) = indexData.comptonLimit3.at(energy_index);
}

double Sept2019Helper::ApplySplitCalibration(TGriffinHit *grifHit)
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

   //===== CROSSTALK COEFFICIENTS =====//
   float CTcoeff[16][4][4] = {0, -0.000456667, -0.00134526, -0.000777811, -0.00108748, 0, -0.000827391, -0.00169368, -0.00164193, -0.000657133, 0, -0.000911018, -0.000950389, -0.00126328, -0.000590419, 0, 0, -0.0019131, -0.00244241, -0.000930193, -0.00137179, 0, -0.000700445, -0.000578935, -0.00150983, -0.000580168, 0, -0.000695303, -0.00102758, -0.00073288, -0.0011868, 0, 0, -0.00172488, -0.00145357, -0.00105393, -0.000817887, 0, -0.000314545, -0.000129661, -0.00171077, -0.000907405, 0, -0.000802641, -0.00125896, -0.00105413, -0.000859705, 0, 0, -0.00126831, -0.00148369, -0.000453308, -0.0016181, 0, -0.000407675, -0.000624059, -0.00172661, -0.00101456, 0, -0.000911773, -0.00121472, -0.000782765, -0.000859423, 0, 0, -0.000797788, -0.00156457, -0.000539448, -0.00126944, 0, -0.000909532, -0.000893915, -0.00121084, -0.000318408, 0, -0.00060478, -0.000954965, -0.000521993, -0.00137538, 0, 0, -0.000953077, -0.00168826, -0.000510058, -0.00113708, 0, -0.00063038, -0.00020561, -0.00117509, -0.00049317, 0, -0.000545118, -0.000703774, -0.000545167, -0.0010178, 0, 0, -6.02183e-05, -0.00128467, -0.0001307, -0.00116513, 0, -0.000499161, -0.000786686, -0.00175129, -0.000925007, 0, -0.000924772, -0.00134405, -0.00121831, -0.00142467, 0, 0, -0.00148818, -0.00117151, -0.000916369, -0.00209092, 0, -0.000352559, -0.000604994, -0.002097, -0.00065074, 0, -0.000662739, -0.00140899, -0.000732452, -0.000608607, 0, 0, -0.00137666, -0.00143569, -0.0013483, -0.000606209, 0, -0.000476369, -0.000827634, -0.00186699, -0.000923926, 0, -0.00109345, -0.000880478, -0.00083046, -0.000985863, 0, 0, -0.00159223, -0.00196884, -0.00144414, -0.00108956, 0, -0.000655957, -0.000776109, -0.0010764, -0.000744261, 0, -0.00108938, -0.000478777, -0.00114861, -0.000577993, 0, 0, -0.0017583, -0.00175305, 0, -0.00130465, 0, -0.000854896, 0, -0.00147421, -0.00111744, 0, 0, 0, 0, 0, 0, 0, -0.00185339, -0.00179959, -0.00140308, -0.00106553, 0, -0.000460575, -0.000782898, -0.00125028, -0.000876984, 0, -0.000946554, -0.000803364, -0.00116938, -0.00066612, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.00142392, -0.00108605, -0.000951035, -0.00127426, 0, -0.00018089, -0.000133508, -0.00174588, -0.00109485, 0, -0.000955283, -0.000956457, -0.00148811, -0.000824678, 0, 0, -0.00160278, -0.0018626, -0.00123516, -0.00155274, 0, -0.000654319, -0.00149267, -0.0015994, -0.000849707, 0, -0.0012822, -0.00108166, -0.00107383, -0.00113472, 0, 0, 0, 0, 0, 0, 0, 0, -0.000415355, 0, 0, 0, 0, 0, 0.000155135, 0, 0};

   //===== CALIBRATION PARAMETERS after Crosstalk correction =====//
   float non_lin_afterCT[64] = {-6.26504e-08, 4.72381e-09, 2.82209e-08, -1.55191e-09, -3.26524e-08, -5.02967e-08, -3.68873e-08, -2.4608e-08, -4.44182e-08, -1.10566e-08, 8.18358e-09, -3.27018e-08, 2.91854e-08, -1.80291e-08, 2.88679e-08, 5.90686e-08, -2.07137e-08, 3.99564e-08, -9.07573e-08, 1.70897e-08, 3.96081e-09, -9.95286e-09, -1.97398e-08, -4.51152e-09, -4.46509e-09, -2.47542e-08, -1.05947e-08, -3.91987e-08, 2.71658e-08, 2.77296e-08, -8.01869e-09, -2.77003e-08, 1.34653e-08, 1.9013e-08, 8.69222e-08, 7.48364e-09, 4.73486e-08, -5.28833e-08, -1.33879e-08, -1.62006e-09, -4.1543e-08, 8.47716e-09, -3.52811e-09, 0, 6.49113e-09, -6.43855e-08, 3.26553e-08, -2.19385e-08, 0, 0, 0, 0, -2.97968e-08, -4.74955e-08, -1.2935e-08, -1.66497e-08, -2.51128e-08, -5.38882e-09, 8.03246e-09, -3.6355e-08, 0, -2.03964e-08, 0, -2.36386e-08};
   float gain_afterCT[64] = {0.999987, 0.999931, 0.99986, 0.99992, 1.00003, 1.00009, 1.00005, 0.999997, 1.00003, 1, 0.999966, 1.00002, 0.999819, 0.999977, 0.999869, 0.999815, 0.999942, 0.999964, 1.00016, 0.999906, 0.999945, 1.00003, 0.999972, 1.00002, 0.999874, 0.99997, 0.999931, 0.999992, 0.99976, 0.999857, 1.00005, 1.00004, 0.99994, 0.999886, 0.999806, 0.999946, 0.999797, 1.00004, 1.00003, 0.999903, 1.00004, 0.999838, 0.999943, -1, 0.999965, 1.00011, 0.999946, 1.00002, -1, -1, -1, -1, 0.999937, 1.00003, 1.00005, 1.00001, 0.999992, 0.999956, 0.99992, 1.00004, -1, 1.00006, -1, 1.00006};
   float offset_afterCT[64] = {-0.0190844, 6.55963e-05, 0.0194426, 1.48277e-05, -0.0026479, -0.0249665, -0.0223625, 0.0036716, -0.00814533, -0.020074, -0.0114052, -0.00846783, 0.00414635, -0.00427748, 0.0158318, 0.0231064, 0.00909008, 0.00203632, -0.0467592, 0.0164958, -0.00683261, -0.0256637, -0.0109006, -0.0157567, 0.0151583, 0.00374787, -0.000758243, 0.0133152, 0.00857865, 0.0218093, -0.0270507, -0.0168875, -0.00786702, -0.0159949, -0.00414951, -0.0167593, 0.0324905, -0.00514884, -0.0116946, -0.000866309, -0.0147094, 0.0248591, -0.000593264, 0, 0.00350045, -0.0256675, -0.00729034, -0.00882748, 0, 0, 0, 0, 0.0104674, -0.00231033, -0.0049287, -0.00736557, -0.0201756, -0.00295571, -0.00799252, -0.0277857, 0, -0.0171159, 0, -0.0141646};

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