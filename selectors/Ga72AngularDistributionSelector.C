#define Ga72AngularDistributionSelector_cxx
// The class definition in ggAngularCorrelationSelector.h has been generated automatically
#include "Ga72AngularDistributionSelector.h"

// ============  GAMMA conditions ============ //
bool InsideEnergyGate(Double_t energy, Double_t gate, Int_t half_range)
{
	return (energy > gate - half_range) && (energy < gate + half_range);
}

bool AboveEnergyFloor(Double_t energy, Double_t floor)
{
	return (energy > floor);
}

// ============  GAMMA-GAMMA conditions ============ //
bool IsGammaGammaCoincidence(long t1, long t2, double time_diff)
{
	return TMath::Abs(t1 - t2) < time_diff;
}

bool AreCoincidentInNS(long t1, long t2, double time_diff)
{
	return TMath::Abs(t1 - t2) < time_diff;
}

bool IsInSameClover(Int_t det1, Int_t det2)
{
	return det1 == det2;
}

bool IsGammaGammaSumTimestampCoincidence(long t1, long t2, double min, double max)
{
	// min and max in timestamp units
	return TMath::Abs(t1 - t2) > min && TMath::Abs(t1 - t2) < max;
}

// =========== ADDBACK BUILDING taken from GRSISort/libraries/TGRSIAnalysis/TGrfiffin/TGriffin.cxx ============ //
// bool DefaultGriffinAddback(const TGRSIDetectorHit& one, const TGRSIDetectorHit& two)
bool DefaultGriffinAddback(TGriffinHit *one, TGriffinHit *two)
{
	return ((one->GetDetector() == two->GetDetector()) &&
			(std::fabs(one->GetTime() - two->GetTime()) < 300.)); // TGRSIOptions::AnalysisOptions()->AddbackWindow())); // change thisw to 300 (ns) if it complains
}

// gamma binnig
Double_t g_binning = 1;
Double_t g_bin_min = 0;
Double_t g_bin_max = 3000;
// timing binnig
Double_t t_binning = 1;
Double_t t_bin_min = 0;
Double_t t_bin_max = 1500;

Double_t timestamp_binning = 1;
Double_t timestamp_bin_min = 0;
Double_t timestamp_bin_max = 1500;

void Ga72AngularDistributionSelector::CreateHistograms()
{
	std::cout << "creating histograms ..." << std::endl;

	// GRIFFIN singles
	fH1["singles_sum"] = new TH1D("singles_sum", "#gamma sum energy", g_bin_max / g_binning, g_bin_min, g_bin_max);
	fH2["singles_energy_channel"] = new TH2D("singles_energy_channel", "GRIFFIN crystal number vs #gamma energy", 65, 0., 65., g_bin_max / g_binning, g_bin_min, g_bin_max);

	fH2["singles_hpge_zds_energy"] = new TH2D("singles_hpge_zds_energy", ";HPGe Energy [keV];ZDS Energy [keV]", g_bin_max / g_binning, g_bin_min, g_bin_max, g_bin_max / g_binning, g_bin_min, g_bin_max);

	// GAMMA-GAMMA matrices
	fH2["singles_gamma_gamma"] = new TH2D("singles_gamma_gamma", "#gamma-#gamma, |#Deltat_{#gamma-#gamma}| < 100 ns", g_bin_max / g_binning, g_bin_min, g_bin_max, g_bin_max / g_binning, g_bin_min, g_bin_max);

	// Sum energy
	fH2["singles_gamma_sum"] = new TH2D("singles_gamma_sum", "#gamma_{sum} w/ constituent gamma, |#Deltat_{#gamma-#gamma}| < 30 ns; #gamma_{sum} Energy per 1 keV; #gamma_1 Energy per 1 keV", g_bin_max / g_binning, g_bin_min, g_bin_max, g_bin_max / g_binning, g_bin_min, g_bin_max);

	// Timing
	fH2["g_gs_timing_prompt"] = new TH2D("g_gs_timing_prompt", "Timing between two photons (prompt); #gamma_{sum} per 1 kev; #Deltat [ns]", g_bin_max / g_binning, g_bin_min, g_bin_max, t_bin_max / t_binning, t_bin_min, t_bin_max);
	fH2["g_gs_timing_delayed"] = new TH2D("g_gs_timing_delayed", "Timing between two photons (delayed); #gamma_{sum} per 1 kev; #Deltat [ns]", g_bin_max / g_binning, g_bin_min, g_bin_max, t_bin_max / t_binning, t_bin_min, t_bin_max);

	// gamma-beta coincidences (singles)
	fH2["singles_gamma_beta_timing"] = new TH2D("singles_gamma_beta_timing", "#beta-#gamma time difference;#Delta_t [ns];Energy per 1 keV", 5000, -2500, 2500, g_bin_max / g_binning, g_bin_min, g_bin_max);
	fH2["singles_gamma_beta_timing_timestamp"] = new TH2D("singles_gamma_beta_timing_timestamp", "#beta-#gamma time difference;#Delta_t * 10 [ns];Energy per 1 keV", 5000, -2500, 2500, g_bin_max / g_binning, g_bin_min, g_bin_max);

	fH2["singles_sum_beta"] = new TH2D("singles_sum_beta", "#beta-#gamma_{sum}, |#Deltat_{#gamma-#gamma}| < 30 ns;Time [ns];Sum Energy per 1 keV", 5000, -2500, 2500, g_bin_max / g_binning, g_bin_min, g_bin_max);
	fH2["singles_sum_gg_beta"] = new TH2D("singles_sum_gg_beta", "#gamma-#gamma_{sum} #beta, |#Deltat_{#gamma-#gamma}| < 30 ns; Sum Energy [keV]; Single #gamma Energy [keV]", g_bin_max / g_binning, g_bin_min, g_bin_max, g_bin_max / g_binning, g_bin_min, g_bin_max);
	fH2["singles_sum_beta_timerandom"] = new TH2D("singles_sum_beta_timerandom", "#beta-#gamma_{sum}, |#Deltat_{#gamma-#gamma}| > 500 ns ", 5000, -2500, 2500, g_bin_max / g_binning, g_bin_min, g_bin_max);

	//* zds-energy
	// singles
	fH2["singles_sum_zds_energy_prompt"] = new TH2D("singles_sum_zds_energy_prompt", "#beta-#gamma, |#Deltat_{#beta-#gamma_{sum}}| < 200 ns; HPGe Energy [keV]; ZDS Energy [kev]", g_bin_max / g_binning, g_bin_min, g_bin_max, g_bin_max / g_binning, g_bin_min, g_bin_max);
	fH2["singles_sum_zds_energy_delayed"] = new TH2D("singles_sum_zds_energy_delayed", "#beta-#gamma, 200 ns < |#Deltat_{#beta-#gamma_{sum}}| < 1200 ns; HPGe Energy [keV]; ZDS Energy [kev]", g_bin_max / g_binning, g_bin_min, g_bin_max, g_bin_max / g_binning, g_bin_min, g_bin_max);
	fH2["singles_sum_zds_energy_timerandom"] = new TH2D("singles_sum_zds_energy_timerandom", "#beta-#gamma, 1400 ns < |#Deltat_{#beta-#gamma_{sum}}| < 2400 ns; HPGe Energy [keV]; ZDS Energy [kev]", g_bin_max / g_binning, g_bin_min, g_bin_max, g_bin_max / g_binning, g_bin_min, g_bin_max);
	// sum
	fH2["singles_zds_energy_prompt"] = new TH2D("singles_zds_energy_prompt", "#beta-#gamma_{sum}, |#Deltat_{#beta-#gamma_{sum}}| < 200 ns; HPGe Energy [keV]; ZDS Energy [kev]", g_bin_max / g_binning, g_bin_min, g_bin_max, g_bin_max / g_binning, g_bin_min, g_bin_max);
	fH2["singles_zds_energy_delayed"] = new TH2D("singles_zds_energy_delayed", "#beta-#gamma_{sum}, 200 ns < |#Deltat_{#beta-#gamma_{sum}}| < 1200 ns; HPGe Energy [keV]; ZDS Energy [kev]", g_bin_max / g_binning, g_bin_min, g_bin_max, g_bin_max / g_binning, g_bin_min, g_bin_max);
	fH2["singles_zds_energy_timerandom"] = new TH2D("singles_zds_energy_timerandom", "#beta-#gamma_{sum}, 1400 ns < |#Deltat_{#beta-#gamma_{sum}}| < 2400 ns; HPGe Energy [keV]; ZDS Energy [kev]", g_bin_max / g_binning, g_bin_min, g_bin_max, g_bin_max / g_binning, g_bin_min, g_bin_max);

	//* Energy-Angular Matrics
	fH2["g_gs_energy_angle_prompt"] = new TH2D("g_gs_energy_angle_prompt", "#gamma_{1} vs Angle Between Photons, |#Deltat_{#beta-#gamma_{sum}}| < 200 ns; Cos(#theta); #gamma Energy [keV]", 100, -1.0, 1.0, g_bin_max / g_binning, g_bin_min, g_bin_max);
	fH2["g_gs_energy_angle_delayed"] = new TH2D("g_gs_energy_angle_delayed", "#gamma_{1} vs Angle Between Photons, 200 < |#Deltat_{#beta-#gamma_{sum}}| < 1200 ns; Cos(#theta); #gamma Energy [keV]", 100, -1.0, 1.0, g_bin_max / g_binning, g_bin_min, g_bin_max);

	for (int i = 0; static_cast<int>(fAngleCombinations.size()); ++i)
	{
		fH2[Form("index_%02i_prompt", i)] = new TH2D(Form("index_%02i_prompt", i), Form("%.1f^{o} |#Deltat_{#beta-#gamma_{sum}}| < 200 ns;sum energy [keV];#gamma_1 energy [keV]", fAngleCombinations[i].first), energy_bins_max, energy_bins_min, energy_bins_max, energy_bins_max, energy_bins_min, energy_bins_max);
	}

	// ZDS
	fH1["zds_multiplicity"] = new TH1D("zds_multiplicity", "", 10, -0.5, 9.5);
	// fH1["zds_kvalue"] = new TH1D("zds_kvalue", ";k-value", 1000, -0.5, 999.5);
	// fH1["hpge_kvalue"] = new TH1D("hpge_kvalue", ";k-value", 1000, -0.5, 999.5);
	// fH2["zds_energy_kvalue"] = new TH2D("zds_energy_kvalue", ";Energy;k-value", 3000, 0, 3000, 100, 0, 100);

	for (auto it : fH1)
	{
		GetOutputList()->Add(it.second);
	}
	for (auto it : fH2)
	{
		GetOutputList()->Add(it.second);
	}
	for (auto it : fHSparse)
	{
		GetOutputList()->Add(it.second);
	}
	std::cout << "done" << std::endl;
}

void Ga72AngularDistributionSelector::FillHistograms()
{
	TGriffinHit *griffin_hit1; // used in addback hit building
	TGriffinHit *griffin_hit2; // used in addback hit building
	TGriffinHit *griffin_hit3; // used in addback hit building
	bool gotZDS = false;
	Double_t zds_kvalue_default = 200;
	Double_t zds_minimum_energy = 10;  // keV
	Double_t hpge_minimum_energy = 10; // keV

	if (fZDS)
		gotZDS = true;

	// -------------------------------------------------
	// Filling Loops
	// -------------------------------------------------

	// Non-symmetric singles
	for (auto i = 0; i < fGrif->GetMultiplicity(); ++i)
	{
		griffin_hit1 = fGrif->GetGriffinHit(i);

		// sets energy floor at minimum value;
		if (griffin_hit1->GetEnergy() < hpge_minimum_energy)
		{
			continue;
		}

		fH2["singles_energy_channel"]->Fill(griffin_hit1->GetArrayNumber(), griffin_hit1->GetEnergy());

		// zds beta tag
		if (gotZDS)
		{
			fH1["zds_multiplicity"]->Fill(fZDS->GetMultiplicity());

			for (auto k = 0; k < fZDS->GetMultiplicity(); ++k)
			{
				TZeroDegreeHit *zds_hit = fZDS->GetZeroDegreeHit(k);
				// fH1["zds_kvalue"]->Fill(zds_hit->GetKValue());

				// remove pileup events
				if (zds_hit->GetKValue() != zds_kvalue_default)
				{
					continue;
				}
				// sets energy floor at minimum value;
				if (zds_hit->GetEnergy() < zds_minimum_energy)
				{
					continue;
				}

				// timing
				long beta_time_diff = griffin_hit1->GetTime() - zds_hit->GetTime();
				long beta_time_diff_timestamp = griffin_hit1->GetTimeStamp() * 10 - zds_hit->GetTimeStamp() * 10;

				fH2["singles_gamma_beta_timing"]->Fill(beta_time_diff, griffin_hit1->GetEnergy());
				fH2["singles_gamma_beta_timing_timestamp"]->Fill(beta_time_diff_timestamp, griffin_hit1->GetEnergy());

				if (TMath::Abs(beta_time_diff) < 200)
				{
					fH2["singles_zds_energy_prompt"]->Fill(griffin_hit1->GetEnergy(), zds_hit->GetEnergy());
				}
				if (TMath::Abs(beta_time_diff) > 200 && TMath::Abs(beta_time_diff) < 1200)
				{
					fH2["singles_zds_energy_delayed"]->Fill(griffin_hit1->GetEnergy(), zds_hit->GetEnergy());
				}
				if (TMath::Abs(beta_time_diff) > 1400 && TMath::Abs(beta_time_diff) < 2400)
				{
					fH2["singles_zds_energy_timerandom"]->Fill(griffin_hit1->GetEnergy(), zds_hit->GetEnergy());
				}
			}
		}

		// second gamma
		for (auto j = i + 1; j < fGrif->GetMultiplicity(); ++j)
		{
			griffin_hit2 = fGrif->GetGriffinHit(j);

			// sets energy floor at minimum value;
			if (griffin_hit1->GetEnergy() < hpge_minimum_energy)
			{
				continue;
			}

			// check gamma-sum timing gate
			long gamma_gamma_timing = TMath::Abs(griffin_hit1->GetTime() - griffin_hit2->GetTime());
			if (gamma_gamma_timing < 200)
			{
				fH2["g_gs_timing_prompt"]->Fill(griffin_hit1->GetEnergy() + griffin_hit2->GetEnergy(), gamma_gamma_timing);
			}
			if (gamma_gamma_timing > 200 && gamma_gamma_timing < 1200)
			{
				fH2["g_gs_timing_delayed"]->Fill(griffin_hit1->GetEnergy() + griffin_hit2->GetEnergy(), gamma_gamma_timing);
			}

			if (IsGammaGammaCoincidence(griffin_hit1->GetTime(), griffin_hit2->GetTime(), 200)) // 200 ns
			{
				fH2["singles_gamma_gamma"]->Fill(griffin_hit1->GetEnergy(), griffin_hit2->GetEnergy());
			}

			// gamma sum timing from here down
			if (IsGammaGammaCoincidence(griffin_hit1->GetTime(), griffin_hit2->GetTime(), 30)) // 30 ns
			{
				Double_t sum_energy = griffin_hit1->GetEnergy() + griffin_hit2->GetEnergy();
				if (!IsInSameClover(griffin_hit1->GetDetector(), griffin_hit2->GetDetector()))
				{
					fH1["singles_sum"]->Fill(sum_energy);
					fH2["singles_gamma_sum"]->Fill(sum_energy, griffin_hit1->GetEnergy());

					// ZDS coincidence if (gotZDS)
					{
						for (auto k = 0; k < fZDS->GetMultiplicity(); ++k)
						{
							TZeroDegreeHit *zds_hit = fZDS->GetZeroDegreeHit(k);

							if (zds_hit->GetKValue() != zds_kvalue_default)
							{
								continue;
							}
							// sets energy floor at minimum value;
							if (zds_hit->GetEnergy() < zds_minimum_energy)
							{
								continue;
							}

							fH2["singles_sum_gg_beta"]->Fill(griffin_hit1->GetEnergy() + griffin_hit2->GetEnergy(), griffin_hit1->GetEnergy());

							long beta_time_diff = griffin_hit1->GetTime() - zds_hit->GetTime();
							long gg_time_diff = griffin_hit1->GetTime() - griffin_hit2->GetTime();
							fH2["singles_sum_beta"]->Fill(beta_time_diff, sum_energy);

							Double_t angle = griffin_hit1->GetPosition(110.).Angle(griffin_hit2->GetPosition(110.)); // radians
							if (angle < 0.0001)
							{
								continue;
							}

							if (TMath::Abs(beta_time_diff) < 200)
							{
								fH2["singles_sum_zds_energy_prompt"]->Fill(sum_energy, zds_hit->GetEnergy());
								fH2["g_gs_energy_angle_prompt"]->Fill(TMath::Cos(angle), griffin_hit1->GetEnergy());
								fH2["g_gs_energy_angle_prompt"]->Fill(TMath::Cos(angle), griffin_hit2->GetEnergy());
							}
							if (TMath::Abs(beta_time_diff) > 200 && TMath::Abs(beta_time_diff) < 1200)
							{
								fH2["singles_sum_zds_energy_delayed"]->Fill(sum_energy, zds_hit->GetEnergy());
								fH2["g_gs_energy_angle_delayed"]->Fill(TMath::Cos(angle), griffin_hit1->GetEnergy());
								fH2["g_gs_energy_angle_delayed"]->Fill(TMath::Cos(angle), griffin_hit2->GetEnergy());
							}
							if (TMath::Abs(beta_time_diff) > 1400 && TMath::Abs(beta_time_diff) < 2400)
							{
								fH2["singles_sum_zds_energy_timerandom"]->Fill(sum_energy, zds_hit->GetEnergy());
							}
						}
					}

					continue;

					// auto angleIndex = fAngleMap.lower_bound(angle - 0.0005);
				}
			}
		} // end second gamma
	}	  // end singles fill block
}