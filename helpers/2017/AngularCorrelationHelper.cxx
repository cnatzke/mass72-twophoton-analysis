#include "AngularCorrelationHelper.hh"
#include "csv.h"

double gbLow = -50.;   // min. time difference gamma-beta
double gbHigh = 500.;  // max. time difference gamma-beta
double ggHigh = 400.;  // max. absolute time difference gamma-gamma
double bgLow = 1000.;  // min. time difference gamma-gamma background
double bgHigh = 2000.; // max. time difference gamma-gamma background

double beta_gs_prompt_min = 275.;   // ns
double beta_gs_prompt_max = 425.;   // ns
double beta_gs_delayed_min = 425.;  // ns
double beta_gs_delayed_max = 2000.; // ns
double two_photon_time_max = 70.;   // ns, max time diff for two-photon events
double gamma_energy_min = 0.;
double gamma_energy_max = 3000.;

double zds_kvalue_default = 200;
double zds_minimum_energy = 10;  // keV
double hpge_minimum_energy = 10; // keV

void AngularCorrelationHelper::CreateHistograms(unsigned int slot)
{

   // for each angle (and the sum) we want
   // for single crystal and addback
   // for prompt and delayed coincidences
   for (int i = 0; i < static_cast<int>(fAngleCombinations.size()); ++i)
   {
      fH2[slot][Form("index_%02d_prompt", i)] = new TH2D(
          Form("index_%02d_prompt", i),
          Form("%.1f^{o}: #gamma-#gamma_{sum} prompt, |#Deltat_{#gamma-#gamma}| < %.1f;#gamma_{sum};#gamma_{1}", fAngleCombinations[i].first, two_photon_time_max),
          gamma_energy_max, gamma_energy_min, gamma_energy_max,
          gamma_energy_max, gamma_energy_min, gamma_energy_max);
      fH2[slot][Form("index_%02d_delayed", i)] = new TH2D(
          Form("index_%02d_delayed", i),
          Form("%.1f^{o}: #gamma-#gamma_{sum} delayed, |#Deltat_{#gamma-#gamma}| < %.1f;#gamma_{sum};#gamma_1", fAngleCombinations[i].first, two_photon_time_max),
          gamma_energy_max, gamma_energy_min, gamma_energy_max,
          gamma_energy_max, gamma_energy_min, gamma_energy_max);

      fH2[slot][Form("index_%02d_prompt_compton", i)] = new TH2D(
          Form("index_%02d_prompt_compton", i),
          Form("%.1f^{o}: #gamma-#gamma_{sum} prompt Compton, |#Deltat_{#gamma-#gamma}| < %.1f;#gamma_{sum};#gamma_1", fAngleCombinations[i].first, two_photon_time_max),
          gamma_energy_max, gamma_energy_min, gamma_energy_max,
          gamma_energy_max, gamma_energy_min, gamma_energy_max);
      fH2[slot][Form("index_%02d_delayed_compton", i)] = new TH2D(
          Form("index_%02d_delayed_compton", i),
          Form("%.1f^{o}: #gamma-#gamma_{sum} delayed Compton, |#Deltat_{#gamma-#gamma}| < %.1f;#gamma_{sum};#gamma_1", fAngleCombinations[i].first, two_photon_time_max),
          gamma_energy_max, gamma_energy_min, gamma_energy_max,
          gamma_energy_max, gamma_energy_min, gamma_energy_max);
   }
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

   // and timing spectra for gamma-gamma and beta-gamma
   fH1[slot]["gammaGammaTiming"] = new TH1D("gammaGammaTiming", "#Deltat_{#gamma-#gamma}", 3000, 0., 3000.);
   fH1[slot]["betaGammaTiming"] = new TH1D("betaGammaTiming", "#Deltat_{#beta-#gamma}", 2000, -1000., 1000.);
   // fH1[slot]["addbackAddbackTiming"] = new TH1D("addbackAddbackTiming", "#Deltat_{#addback-#addback}", 2000, 0., 3000.);
   // fH1[slot]["betaAddbackTiming"] = new TH1D("betaAddbackTiming", "#Deltat_{#beta-#gamma}", 2000, -1000., 1000.);

   // diagnostic histograms
   fH1[slot]["gammaMultiplicity"] = new TH1D("gammaMultiplicity", "HPGe multiplicity", 10, 0, 10);
   fH1[slot]["gammaBetaTiming"] = new TH1D("gammaBetaTiming", "Beta-2Gamma #Deltat", 1500, 0, 1500);
   fH1[slot]["promptTiming"] = new TH1D("promptTiming", "Diagnostic timing (prompt)", 1000, 0, 1000);
   fH1[slot]["delayedTiming"] = new TH1D("delayedTiming", "Diagnostic timing (delayed)", 2500, 0, 2500);
   // fH1[slot]["zds_kvalue"] = new TH1D("zds_kvalue", "Diagnostic timing (delayed)", 1500, 0, 1500);

   fH2[slot]["betaGammaTimingMatrix"] = new TH2D(
       "betaGammaTimingMatrix",
       "#Deltat beta gamma timing",
       3000, -1500, 1500,
       gamma_energy_max, gamma_energy_min, gamma_energy_max);
   fH2[slot]["betaGammaTimingMatrixPileup"] = new TH2D(
       "betaGammaTimingMatrixPileup",
       "#Deltat beta gamma timing with pileup",
       3000, -1500, 1500,
       gamma_energy_max, gamma_energy_min, gamma_energy_max);
}

void AngularCorrelationHelper::Exec(unsigned int slot, TGriffin &grif, TZeroDegree &zds)
{

   // without addback
   for (auto g1 = 0; g1 < grif.GetMultiplicity(); ++g1)
   {

      fH1[slot].at("gammaMultiplicity")->Fill(grif.GetMultiplicity());

      // first gamma hit
      auto grif1 = grif.GetGriffinHit(g1);

      // sets energy floor at minimum value;
      if (grif1->GetEnergy() < hpge_minimum_energy)
      {
         continue;
      }

      fH2[slot].at("gammaEnergyChannel")->Fill(grif1->GetArrayNumber(), grif1->GetEnergy());
      for (auto z1 = 0; z1 < zds.GetMultiplicity(); ++z1)
      {
         auto zds1 = zds.GetHit(z1);

         fH2[slot].at("betaGammaTimingMatrixPileup")->Fill(zds1->GetTime() - grif1->GetTime(), grif1->GetEnergy());

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

         fH1[slot].at("betaGammaTiming")->Fill(zds1->GetTime() - grif1->GetTime());
         fH2[slot].at("betaGammaTimingMatrix")->Fill(zds1->GetTime() - grif1->GetTime(), grif1->GetEnergy());
      }

      //* SECOND GAMMA HIT
      for (auto g2 = g1 + 1; g2 < grif.GetMultiplicity(); ++g2)
      {
         auto grif2 = grif.GetGriffinHit(g2);

         if (grif2->GetEnergy() < hpge_minimum_energy)
         {
            continue;
         }

         double angle = grif1->GetPosition().Angle(grif2->GetPosition()) * 180. / TMath::Pi();
         if (angle < 0.0001)
            continue;
         auto angleIndex = fAngleMap.lower_bound(angle - 0.0005);
         double ggTime = grif1->GetTime() - grif2->GetTime();

         fH1[slot].at("gammaGammaTiming")->Fill(ggTime);
         fH2[slot].at("gammaGammaHP")->Fill(grif1->GetArrayNumber(), grif2->GetArrayNumber());

         // * Temp filter
         if (grif1->GetEnergy() + grif2->GetEnergy() > 3000)
         {
            continue;
         }

         //* Gamma-gamma timing checks
         /*
         if (TMath::Abs(ggTime) < 150)
         {
            auto sum_energy = grif1->GetEnergy() + grif2->GetEnergy();
            // beta tag
            for (auto z = 0; z < zds.GetMultiplicity(); ++z)
            {
               bool grif1_prompt = false;
               bool grif2_prompt = false;
               bool grif1_delayed = false;
               bool grif2_delayed = false;

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
               auto grif1_beta_time_diff = grif1->GetTime() - zds1->GetTime();
               auto grif2_beta_time_diff = grif2->GetTime() - zds1->GetTime();

               if (beta_gs_prompt_min <= grif1_beta_time_diff && grif1_beta_time_diff <= beta_gs_prompt_max)
               {
                  grif1_prompt = true;
               }
               else if (beta_gs_prompt_min <= grif2_beta_time_diff && grif2_beta_time_diff <= beta_gs_prompt_max)
               {
                  grif2_prompt = true;
               }
               else if (beta_gs_delayed_min <= grif1_beta_time_diff && grif1_beta_time_diff <= beta_gs_delayed_max)
               {
                  grif1_delayed = true;
               }
               else if (beta_gs_delayed_min <= grif2_beta_time_diff && grif2_beta_time_diff <= beta_gs_delayed_max)
               {
                  grif2_delayed = true;
               }

               // std::cout << grif1->GetEnergy() << ", " << grif2->GetEnergy() << ", " << angleIndex->second << std::endl;
               // ! Breaks for high energy values
               bool compton_tag = GetComptonTag(grif1->GetEnergy(), grif2->GetEnergy(), angleIndex->second);

               // prompt :
               if (grif1_prompt || grif2_prompt)
               {
                  fH1[slot].at("promptTiming")->Fill(grif1_beta_time_diff);
                  fH1[slot].at("promptTiming")->Fill(grif2_beta_time_diff);
                  fH2[slot].at("gammaSumTimingPromptTotal")->Fill(TMath::Abs(TMath::Abs(ggTime)), sum_energy);
                  if (compton_tag)
                  {
                     fH2[slot].at("gammaSumTimingPromptCompton")->Fill(TMath::Abs(ggTime), sum_energy);
                  }
                  else
                  {
                     fH2[slot].at("gammaSumTimingPrompt")->Fill(TMath::Abs(ggTime), sum_energy);
                  }
               } // end prompt

               // delayed :
               if (grif1_delayed || grif2_delayed)
               {
                  fH1[slot].at("delayedTiming")->Fill(grif1_beta_time_diff);
                  fH1[slot].at("delayedTiming")->Fill(grif2_beta_time_diff);
                  fH2[slot].at("gammaSumTimingDelayedTotal")->Fill(TMath::Abs(ggTime), sum_energy);
                  if (compton_tag)
                  {
                     fH2[slot].at("gammaSumTimingDelayedCompton")->Fill(TMath::Abs(ggTime), sum_energy);
                  }
                  else
                  {
                     fH2[slot].at("gammaSumTimingDelayed")->Fill(TMath::Abs(ggTime), sum_energy);
                  }
               } // end delayed
            }    // end beta tag
         }
         //* End gamma-gamma timing checks
         */

         // TWO PHOTON EVENTS
         if (TMath::Abs(ggTime) < two_photon_time_max)
         {
            auto sum_energy = grif1->GetEnergy() + grif2->GetEnergy();
            fH1[slot].at("sumEnergy")->Fill(sum_energy);

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

               // time difference
               auto grif1_beta_time_diff = grif1->GetTime() - zds1->GetTime();
               auto grif2_beta_time_diff = grif2->GetTime() - zds1->GetTime();

               fH1[slot].at("gammaBetaTiming")->Fill(grif1_beta_time_diff);
               fH1[slot].at("gammaBetaTiming")->Fill(grif2_beta_time_diff);

               if (beta_gs_delayed_min <= grif1_beta_time_diff && grif1_beta_time_diff <= beta_gs_delayed_max)
               {
                  grif1_delayed = true;
               }
               if (beta_gs_delayed_min <= grif2_beta_time_diff && grif2_beta_time_diff <= beta_gs_delayed_max)
               {
                  grif2_delayed = true;
               }

               bool compton_tag = GetComptonTag(grif1->GetEnergy(), grif2->GetEnergy(), angleIndex->second);

               // prompt :
               if (TMath::Abs(grif1_beta_time_diff) < beta_gs_prompt_max || TMath::Abs(grif2_beta_time_diff) < beta_gs_prompt_max)
               {
                  fH1[slot].at("promptTiming")->Fill(grif1_beta_time_diff);

                  fH2[slot].at("gammaSumGammaPromptTotal")->Fill(sum_energy, grif1->GetEnergy());
                  fH2[slot].at("gammaSumGammaPromptTotal")->Fill(sum_energy, grif2->GetEnergy());

                  if (compton_tag)
                  {
                     // fH2[slot].at(Form("index_%02d_prompt_compton", angleIndex->second))->Fill(sum_energy, grif1->GetEnergy());
                     fH2[slot].at("gammaSumGammaPromptCompton")->Fill(sum_energy, grif1->GetEnergy());
                     fH2[slot].at("gammaSumGammaPromptCompton")->Fill(sum_energy, grif2->GetEnergy());
                  }
                  else
                  {
                     fH1[slot].at("sumEnergyPrompt")->Fill(sum_energy);
                     // fH2[slot].at(Form("index_%02d_prompt", angleIndex->second))->Fill(sum_energy, grif1->GetEnergy());
                     fH2[slot].at("gammaSumGammaPrompt")->Fill(sum_energy, grif1->GetEnergy());
                     fH2[slot].at("gammaSumGammaPrompt")->Fill(sum_energy, grif2->GetEnergy());
                  }

               } // end prompt

               if (grif1_delayed || grif2_delayed)
               {
                  fH1[slot].at("delayedTiming")->Fill(grif1_beta_time_diff);

                  fH2[slot].at("gammaSumGammaDelayedTotal")->Fill(sum_energy, grif1->GetEnergy());
                  fH2[slot].at("gammaSumGammaDelayedTotal")->Fill(sum_energy, grif2->GetEnergy());

                  if (compton_tag)
                  {
                     // fH2[slot].at(Form("index_%02d_delayed_compton", angleIndex->second))->Fill(sum_energy, grif1->GetEnergy());
                     fH2[slot].at("gammaSumGammaDelayedCompton")->Fill(sum_energy, grif1->GetEnergy());
                     fH2[slot].at("gammaSumGammaDelayedCompton")->Fill(sum_energy, grif2->GetEnergy());
                  }
                  else
                  {
                     fH1[slot].at("sumEnergyDelayed")->Fill(sum_energy);
                     // fH2[slot].at(Form("index_%02d_delayed", angleIndex->second))->Fill(sum_energy, grif1->GetEnergy());
                     fH2[slot].at("gammaSumGammaDelayed")->Fill(sum_energy, grif1->GetEnergy());
                     fH2[slot].at("gammaSumGammaDelayed")->Fill(sum_energy, grif2->GetEnergy());
                  }

               } // end delayed
            }    // end beta tag
         }       // end two-photon timing
      }          // end second gamma
   }             // end first gamma

   /*
   // with addback
   for(auto g1 = 0; g1 < grif.GetAddbackMultiplicity(); ++g1) {
      auto grif1 = grif.GetAddbackHit(g1);
      // check for coincident betas
      bool coincBeta = false;
      for(auto s = 0; s < zds.GetMultiplicity(); ++s) {
         auto zds1 = zds.GetSceptarHit(s);
         if(!coincBeta && gbLow <= grif1->GetTime() - zds1->GetTime() && grif1->GetTime() - zds1->GetTime() <= gbHigh)
            coincBeta = true;
         fH1[slot].at("betaAddbackTiming")->Fill(zds1->GetTime() - grif1->GetTime());
         fH2[slot].at("betaAddbackHP")->Fill(zds1->GetDetector(), grif1->GetArrayNumber());
      }
      fH1[slot].at("addbackEnergy")->Fill(grif1->GetEnergy());
      if(coincBeta) fH1[slot].at("addbackEnergyBeta")->Fill(grif1->GetEnergy());
      for(auto g2 = 0; g2 < grif.GetAddbackMultiplicity(); ++g2) {
         if(g1 == g2) continue;
         auto   grif2 = grif.GetAddbackHit(g2);
         double angle = grif1->GetPosition().Angle(grif2->GetPosition()) * 180. / TMath::Pi();
         if(angle < 0.0001) continue;
         auto   angleIndex = fAngleMapAddback.lower_bound(angle - 0.0005);
         double ggTime     = TMath::Abs(grif1->GetTime() - grif2->GetTime());
         fH1[slot].at("addbackAddbackTiming")->Fill(ggTime);
         fH2[slot].at("addbackAddbackHP")->Fill(grif1->GetArrayNumber(), grif2->GetArrayNumber());

         if(ggTime < ggHigh) {
            fH2[slot].at("addbackAddback")->Fill(grif1->GetEnergy(), grif2->GetEnergy());
            fH2[slot].at(Form("addbackAddback%d", angleIndex->second))->Fill(grif1->GetEnergy(), grif2->GetEnergy());
            if(coincBeta) {
               fH2[slot].at("addbackAddbackBeta")->Fill(grif1->GetEnergy(), grif2->GetEnergy());
               fH2[slot]
                  .at(Form("addbackAddbackBeta%d", angleIndex->second))
                  ->Fill(grif1->GetEnergy(), grif2->GetEnergy());
            }
         } else if(bgLow < ggTime && ggTime < bgHigh) {
            fH2[slot].at("addbackAddbackBG")->Fill(grif1->GetEnergy(), grif2->GetEnergy());
            fH2[slot].at(Form("addbackAddbackBG%d", angleIndex->second))->Fill(grif1->GetEnergy(), grif2->GetEnergy());
            if(coincBeta) {
               fH2[slot].at("addbackAddbackBetaBG")->Fill(grif1->GetEnergy(), grif2->GetEnergy());
               fH2[slot]
                  .at(Form("addbackAddbackBetaBG%d", angleIndex->second))
                  ->Fill(grif1->GetEnergy(), grif2->GetEnergy());
            }
         }
      }
      // event mixing, we use the last event as second griffin
      for(auto g2 = 0; g2 < fLastGrif.GetAddbackMultiplicity(); ++g2) {
         if(g1 == g2) continue;
         auto   grif2 = fLastGrif.GetAddbackHit(g2);
         double angle = grif1->GetPosition().Angle(grif2->GetPosition()) * 180. / TMath::Pi();
         if(angle < 0.0001) continue;
         auto   angleIndex = fAngleMapAddback.lower_bound(angle - 0.0005);
         double ggTime     = TMath::Abs(grif1->GetTime() - grif2->GetTime());
         fH2[slot].at("addbackAddbackHPMixed")->Fill(grif1->GetArrayNumber(), grif2->GetArrayNumber());

         fH2[slot].at("addbackAddbackMixed")->Fill(grif1->GetEnergy(), grif2->GetEnergy());
         fH2[slot].at(Form("addbackAddbackMixed%d", angleIndex->second))->Fill(grif1->GetEnergy(), grif2->GetEnergy());
         if(coincBeta) {
            fH2[slot].at("addbackAddbackBetaMixed")->Fill(grif1->GetEnergy(), grif2->GetEnergy());
            fH2[slot]
               .at(Form("addbackAddbackBetaMixed%d", angleIndex->second))
               ->Fill(grif1->GetEnergy(), grif2->GetEnergy());
         }
      }
   }

   // update "last" event
   fLastGrif = grif;
   fLastScep = zds;
   */
}

bool AngularCorrelationHelper::GetComptonTag(double energy1, double energy2, int angle_index)
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

void AngularCorrelationHelper::GetComptonLimits(double energy, int index, std::vector<double> &vec)
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