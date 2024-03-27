#define SingleCal_HardcodedTiming_cxx
// The class definition in ggAngularCorrelationSelector.h has been generated automatically
#include "SingleCal_HardcodedTiming.h"

// ============  GAMMA-ELECTRON conditions ============ //
// gamma-electron BG coincidence: 1000 ns < abs(pacesTime - griffinTime) < 2000 ns (so 1000+1000 ns = 2000 ns window)
bool BGCoincidenceCondition(TGriffinHit *hit_one, TPacesHit *hit_two)
{
	return (TMath::Abs(hit_two->GetTime() - hit_one->GetTime()) > 1000 && TMath::Abs(hit_two->GetTime() - hit_one->GetTime()) < 2000);
}
// addback-electron coincidence: -60 ns < pacesTime - griffinTime < 40 ns (100 ns window)
bool CoincidenceCondition(long time_hit_one, TPacesHit *hit_two)
{
	return (-60. < (hit_two->GetTime() - time_hit_one)) && ((hit_two->GetTime() - time_hit_one) < 40.);
}
// addback-electron BG coincidence: 1000 ns < abs(pacesTime - griffinTime) < 2000 ns (so 1000+1000 ns = 2000 ns window)
bool BGCoincidenceCondition(long time_hit_one, TPacesHit *hit_two)
{
	return (TMath::Abs(hit_two->GetTime() - time_hit_one) > 1000 && TMath::Abs(hit_two->GetTime() - time_hit_one) < 2000);
}

// ============  GAMMA conditions ============ //
bool InsideEnergyGate(Double_t energy, Double_t gate, Int_t half_range)
{
	return (energy > gate - half_range) && (energy < gate + half_range);
}

// ============  GAMMA-GAMMA conditions ============ //
bool IsGammaGammaCoincidence(long t1, long t2, double time_diff)
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

// gamma-gamma BG coincidence: abs(DeltaT) > 500 ns && abs(DeltaT) < 2000 ns = 2 us (so 1500+1500 ns = 3000 ns window)
bool BGCoincidenceCondition(TGriffinHit *hit_one, TGriffinHit *hit_two)
{
	return (TMath::Abs(hit_one->GetTime() - hit_two->GetTime()) > 500 && TMath::Abs(hit_one->GetTime() - hit_two->GetTime()) < 2000);
}

// ============  ADDBACK-ADDBACK conditions ============ //
// gamma-gamma coincidence: abs(DeltaT) < 100 ns (200 ns window)
bool CoincidenceCondition(long time_hit_one, long time_hit_two)
{
	return TMath::Abs(time_hit_one - time_hit_two) < 100;
}
// gamma-gamma BG coincidence: abs(DeltaT) > 500 ns && abs(DeltaT) < 2000 ns = 2 us (so 1500+1500 ns = 3000 ns window)
bool BGCoincidenceCondition(long time_hit_one, long time_hit_two)
{
	return (TMath::Abs(time_hit_one - time_hit_two) > 500 && TMath::Abs(time_hit_one - time_hit_two) < 2000);
}

// ============  BETA-TAGGING conditions ============ //
// beta-tag requirement GRIFFIN-ZDS: -320 ns < griffinTime - zdsTime < -220 ns (100 ns window)
bool CoincidenceCondition(TGriffinHit *hit_one, TZeroDegreeHit *hit_two)
{
	return (-320. < (hit_one->GetTime() - hit_two->GetTime())) && ((hit_one->GetTime() - hit_two->GetTime()) < -220.);
}
// GRIFFIN-ZDS BG coincidence: -800 ns < griffinTime - zdsTime < -600 ns or -100 ns < griffinTime - zdsTime < 0 ns (300 ns window)
bool BGCoincidenceCondition(TGriffinHit *hit_one, TZeroDegreeHit *hit_two)
{
	return (((hit_one->GetTime() - hit_two->GetTime()) > -800 && (hit_one->GetTime() - hit_two->GetTime()) < -600) || ((hit_one->GetTime() - hit_two->GetTime()) > -100 && (hit_one->GetTime() - hit_two->GetTime()) < 0));
}
// beta-tag requirement GRIFFINADDBACK-ZDS: -320 ns < griffinTime - zdsTime < -220 ns (100 ns window)
bool CoincidenceCondition(long time_hit_one, TZeroDegreeHit *hit_two)
{
	return (-320. < (time_hit_one - hit_two->GetTime())) && ((time_hit_one - hit_two->GetTime()) < -220.);
}
// GRIFFINADDBACK-ZDS BG coincidence: -800 ns < griffinTime - zdsTime < -600 ns or -100 ns < griffinTime - zdsTime < 0 ns (300 ns window)
bool BGCoincidenceCondition(long time_hit_one, TZeroDegreeHit *hit_two)
{
	return (((time_hit_one - hit_two->GetTime()) > -800 && (time_hit_one - hit_two->GetTime()) < -600) || ((time_hit_one - hit_two->GetTime()) > -100 && (time_hit_one - hit_two->GetTime()) < 0));
}
// beta-tag requirement PACES-ZDS: -300 ns < pacesTime - zdsTime < -260 ns (40 ns window) -- HERE!!
bool CoincidenceCondition(TPacesHit *hit_one, TZeroDegreeHit *hit_two)
{ // Check if hits are less then 500 ns apart.
	return (-300. < (hit_one->GetTime() - hit_two->GetTime())) && ((hit_one->GetTime() - hit_two->GetTime()) < -260.);
}
// PACES-ZDS BG coincidence: 600 ns < abs(pacesTime - zdsTime) < 1600 ns (2000 ns window)
bool BGCoincidenceCondition(TPacesHit *hit_one, TZeroDegreeHit *hit_two)
{
	return (TMath::Abs(hit_one->GetTime() - hit_two->GetTime()) > 600 && TMath::Abs(hit_one->GetTime() - hit_two->GetTime()) < 1600);
}

// =========== ADDBACK BUILDING taken from GRSISort/libraries/TGRSIAnalysis/TGrfiffin/TGriffin.cxx ============ //
// bool DefaultGriffinAddback(const TGRSIDetectorHit& one, const TGRSIDetectorHit& two)
bool DefaultGriffinAddback(TGriffinHit *one, TGriffinHit *two)
{
	return ((one->GetDetector() == two->GetDetector()) &&
			(std::fabs(one->GetTime() - two->GetTime()) < 300.)); // TGRSIOptions::AnalysisOptions()->AddbackWindow())); // change thisw to 300 (ns) if it complains
}

double gbin = 3000., gx = 0., gy = 3000.; // energy range and binning GRIFFIN
double gamma_binning = 1, gamma_bin_max = 3000, gamma_bin_min = 0;
Double_t timestamp_binning = 1;
Double_t timestamp_bin_min = 0;
Double_t timestamp_bin_max = 1500;
double gbinPACES = 8192., gxPACES = 0., gyPACES = 4096.; // energy range and binning PACES - 0.5 keV/channel

void SingleCal_HardcodedTiming::CreateHistograms()
{
	std::cout << "creating histograms ..." << std::endl;

	// ZDS singles
	// 20200212//fH1["zE"] = new TH1D("zE","ZeroDegree singles", 1000,0,10000);

	// GRIFFIN singles
	fH1["singles_sum"] = new TH1D("singles_sum", "#gamma sum energy", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH1["singles_sum_samedet"] = new TH1D("singles_sum_samedet", "#gamma sum energy, same clover allowed", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH2["singles_energy_channel"] = new TH2D("singles_energy_channel", "GRIFFIN crystal number vs #gamma energy", 65, 0., 65., gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);

	fH2["singles_hpge_zds_energy"] = new TH2D("singles_hpge_zds_energy", ";HPGe Energy [keV];ZDS Energy [keV]", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);

	// GAMMA-GAMMA matrices
	fH2["singles_gamma_gamma"] = new TH2D("singles_gamma_gamma", "#gamma-#gamma, |#Deltat_{#gamma-#gamma}| < 100 ns", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);

	// Sum energy
	fH2["singles_gamma_sum"] = new TH2D("singles_gamma_sum", "#gamma_{sum} w/ constituent gamma, |#Deltat_{#gamma-#gamma}| < 30 ns; #gamma_{sum} Energy per 1 keV; #gamma_1 Energy per 1 keV", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH2["singles_gamma_sum_angular_gated691"] = new TH2D("singles_gamma_sum_angular_gated691", "#gamma_1 angular distribution, |#Deltat_{#gamma-#gamma}| < 30 ns, 691keV; Cos(#Theta);#gamma_{sum} Energy per 1 keV", 180, 0, 180, 750 / gamma_binning, gamma_bin_min, 750);
	fH2["singles_gamma_gammasum_energy"] = new TH2D("singles_gamma_gammasum_energy", "#gamma-#gamma_{sum}, |#Deltat_{#gamma-#gamma}| < 30 ns;Trigger #gamma Energy per 1 keV; #gamma_{sum} Energy per 1 keV", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);

	// Timing
	fH2["singles_gamma_gammasum_trigger_773"] = new TH2D("singles_gamma_gammasum_trigger_773", "Timing between single gamma and gamma sum gated on 773 keV, |#Deltat_{#gamma-#gamma}| < 30 ns; #gamma Sum Energy per 1 kev; #Deltat [samples]", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, timestamp_bin_max / timestamp_binning, timestamp_bin_min, timestamp_bin_max);
	fH2["singles_gamma_gammasum_timing"] = new TH2D("singles_gamma_gammasum_timing", "Timing between single gamma and gamma sum, |#Deltat_{#gamma-#gamma}| < 30 ns; #gamma Energy per 1 kev; #Deltat [samples]", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, timestamp_bin_max / timestamp_binning, timestamp_bin_min, timestamp_bin_max);

	// gamma-beta coincidences (singles)
	fH2["singles_gamma_beta_timing"] = new TH2D("singles_gamma_beta_timing", "#beta-#gamma time difference;#Delta_t [ns];Energy per 1 keV", 5000, -2500, 2500, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH2["singles_gamma_beta_timing_timestamp"] = new TH2D("singles_gamma_beta_timing_timestamp", "#beta-#gamma time difference;#Delta_t * 10 [ns];Energy per 1 keV", 5000, -2500, 2500, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);

	fH2["singles_sum_beta"] = new TH2D("singles_sum_beta", "#beta-#gamma_{sum}, |#Deltat_{#gamma-#gamma}| < 30 ns;Time [ns];Sum Energy per 1 keV", 5000, -2500, 2500, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH2["singles_sum_gg_beta"] = new TH2D("singles_sum_gg_beta", "#gamma-#gamma_{sum} #beta, |#Deltat_{#gamma-#gamma}| < 30 ns; Sum Energy [keV]; Single #gamma Energy [keV]", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH2["singles_sum_beta_timerandom"] = new TH2D("singles_sum_beta_timerandom", "#beta-#gamma_{sum}, |#Deltat_{#gamma-#gamma}| > 500 ns ", 5000, -2500, 2500, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);

	//* zds-energy
	// singles
	fH2["singles_sum_zds_energy_prompt"] = new TH2D("singles_sum_zds_energy_prompt", "#beta-#gamma, |#Deltat_{#beta-#gamma_{sum}}| < 200 ns; HPGe Energy [keV]; ZDS Energy [kev]", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH2["singles_sum_zds_energy_delayed"] = new TH2D("singles_sum_zds_energy_delayed", "#beta-#gamma, 200 ns < |#Deltat_{#beta-#gamma_{sum}}| < 1200 ns; HPGe Energy [keV]; ZDS Energy [kev]", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH2["singles_sum_zds_energy_timerandom"] = new TH2D("singles_sum_zds_energy_timerandom", "#beta-#gamma, 1400 ns < |#Deltat_{#beta-#gamma_{sum}}| < 2400 ns; HPGe Energy [keV]; ZDS Energy [kev]", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	// sum
	fH2["singles_zds_energy_prompt"] = new TH2D("singles_zds_energy_prompt", "#beta-#gamma_{sum}, |#Deltat_{#beta-#gamma_{sum}}| < 200 ns; HPGe Energy [keV]; ZDS Energy [kev]", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH2["singles_zds_energy_delayed"] = new TH2D("singles_zds_energy_delayed", "#beta-#gamma_{sum}, 200 ns < |#Deltat_{#beta-#gamma_{sum}}| < 1200 ns; HPGe Energy [keV]; ZDS Energy [kev]", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH2["singles_zds_energy_timerandom"] = new TH2D("singles_zds_energy_timerandom", "#beta-#gamma_{sum}, 1400 ns < |#Deltat_{#beta-#gamma_{sum}}| < 2400 ns; HPGe Energy [keV]; ZDS Energy [kev]", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);

	//* timing diagnostic
	// sum
	fH2["timing_diagnostic_prompt"] = new TH2D("timing_diagnostic_prompt", ";#beta-#gamma_{sum} [ns];#gamma-#gamma [ns]", 5000, -2500, 2500, 5000, -2500, 2500);
	fH2["timing_diagnostic_delayed"] = new TH2D("timing_diagnostic_delayed", ";#beta-#gamma_{sum} [ns];#gamma-#gamma [ns]", 5000, -2500, 2500, 5000, -2500, 2500);
	fH2["timing_diagnostic_timerandom"] = new TH2D("timing_diagnostic_timerandom", ";#beta-#gamma_{sum} [ns];#gamma-#gamma [ns]", 5000, -2500, 2500, 5000, -2500, 2500);

	fH2["timing_diagnostic_gamma_gamma_time"] = new TH2D("timing_diagnostic_gamma_gamma_time", ";Time difference [ns]; #gamma Energy [keV]", 5000, -2500, 2500, 3000, 0, 3000);
	fH2["timing_diagnostic_gamma_gamma_timestamp"] = new TH2D("timing_diagnostic_gamma_gamma_timestamp", ";Time difference [10 ns]; #gamma Energy [keV]", 5000, -2500, 2500, 3000, 0, 3000);

	fH2["time_timestamp"] = new TH2D("time_timestamp", ";Time difference [ns];Time difference [10ns]", 5000, -2500, 2500, 500, -250, 250);

	fH2["cfd_diagnostic_hpge"] = new TH2D("cfd_diagnostic_hpge", "HPGe;GetTime() - GetTimeStamp() * 10;Energy [keV]", 5000, -2500, 2500, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH2["cfd_diagnostic_zds"] = new TH2D("cfd_diagnostic_zds", "ZDS;GetTime() - GetTimeStamp() * 10;Energy [keV]", 5000, -2500, 2500, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);

	// ZDS
	fH1["zds_multiplicity"] = new TH1D("zds_multiplicity", "", 10, -0.5, 9.5);
	// fH1["zds_kvalue"] = new TH1D("zds_kvalue", ";k-value", 1000, -0.5, 999.5);
	// fH1["hpge_kvalue"] = new TH1D("hpge_kvalue", ";k-value", 1000, -0.5, 999.5);
	// fH2["zds_energy_kvalue"] = new TH2D("zds_energy_kvalue", ";Energy;k-value", 3000, 0, 3000, 100, 0, 100);

	// gamma-beta coincidences (Addback)
	fH2["addback_sum_beta"] = new TH2D("addback_sum_beta", "#beta-#gamma_{sum} (Addback), |#Deltat_{#gamma-#gamma}| < 30 ns;Timestamp;Sum Energy per 1 keV", 1200, 0, 1200, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);

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

void SingleCal_HardcodedTiming::FillHistograms()
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
		fH2["cfd_diagnostic_hpge"]->Fill(griffin_hit1->GetTime() - griffin_hit1->GetTimeStamp() * 10, griffin_hit1->GetEnergy());
		// fH1["hpge_kvalue"]->Fill(griffin_hit1->GetKValue());

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

				fH2["cfd_diagnostic_zds"]->Fill(zds_hit->GetTime() - zds_hit->GetTimeStamp() * 10, zds_hit->GetEnergy());
				fH2["singles_hpge_zds_energy"]->Fill(griffin_hit1->GetEnergy(), zds_hit->GetEnergy());

				// timing
				long beta_time_diff = griffin_hit1->GetTime() - zds_hit->GetTime();
				long beta_time_diff_timestamp = griffin_hit1->GetTimeStamp() * 10 - zds_hit->GetTimeStamp() * 10;

				fH2["singles_gamma_beta_timing"]->Fill(beta_time_diff, griffin_hit1->GetEnergy());
				fH2["singles_gamma_beta_timing_timestamp"]->Fill(beta_time_diff_timestamp, griffin_hit1->GetEnergy());
				fH2["time_timestamp"]->Fill(beta_time_diff, beta_time_diff_timestamp);

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

			// diagnostic timing
			long gamma_gamma_time_diff = griffin_hit1->GetTime() - griffin_hit2->GetTime();
			long gamma_gamma_timestamp_diff = griffin_hit1->GetTimeStampNs() - griffin_hit2->GetTimeStampNs();
			fH2["timing_diagnostic_gamma_gamma_time"]->Fill(gamma_gamma_time_diff, griffin_hit2->GetEnergy());
			fH2["timing_diagnostic_gamma_gamma_timestamp"]->Fill(gamma_gamma_timestamp_diff, griffin_hit2->GetEnergy());

			if (IsGammaGammaCoincidence(griffin_hit1->GetTime(), griffin_hit2->GetTime(), 200)) // 200 ns
			{
				fH2["singles_gamma_gamma"]->Fill(griffin_hit1->GetEnergy(), griffin_hit2->GetEnergy());
			}

			if (IsGammaGammaCoincidence(griffin_hit1->GetTime(), griffin_hit2->GetTime(), 30)) // 30 ns
			{
				Double_t sum_energy = griffin_hit1->GetEnergy() + griffin_hit2->GetEnergy();
				fH1["singles_sum_samedet"]->Fill(sum_energy);
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

							if (TMath::Abs(beta_time_diff) < 200)
							{
								fH2["singles_sum_zds_energy_prompt"]->Fill(sum_energy, zds_hit->GetEnergy());
								fH2["timing_diagnostic_prompt"]->Fill(TMath::Abs(beta_time_diff), gg_time_diff);
							}
							if (TMath::Abs(beta_time_diff) > 200 && TMath::Abs(beta_time_diff) < 1200)
							{
								fH2["singles_sum_zds_energy_delayed"]->Fill(sum_energy, zds_hit->GetEnergy());
								fH2["timing_diagnostic_delayed"]->Fill(TMath::Abs(beta_time_diff), gg_time_diff);
							}
							if (TMath::Abs(beta_time_diff) > 1400 && TMath::Abs(beta_time_diff) < 2400)
							{
								fH2["singles_sum_zds_energy_timerandom"]->Fill(sum_energy, zds_hit->GetEnergy());
								fH2["timing_diagnostic_timerandom"]->Fill(TMath::Abs(beta_time_diff), gg_time_diff);
							}
						}
					}

					Double_t energy_gate = 691; // keV
					Int_t gate_half_width = 3;
					if (InsideEnergyGate(sum_energy, energy_gate, gate_half_width))
					{
						Double_t angle = griffin_hit1->GetPosition(110.).Angle(griffin_hit2->GetPosition(110.)); // radians
						if (angle < 0.0001)
							continue;

						// auto angleIndex = fAngleMap.lower_bound(angle - 0.0005);
						fH2["singles_gamma_sum_angular_gated691"]->Fill(angle * 180 / TMath::Pi(), griffin_hit1->GetEnergy());
					}
				}
			}
		} // end second gamma
	}	  // end singles fill block

	// symmetric singles
	/*
	for (auto i = 0; i < fGrif->GetMultiplicity(); ++i)
	{
		griffin_hit1 = fGrif->GetGriffinHit(i);
		for (auto j = 0; j < fGrif->GetMultiplicity(); j++)
		{
			if (i == j)
			{
				continue;
			}
			griffin_hit2 = fGrif->GetGriffinHit(j);

			for (auto k = 0; k < fGrif->GetMultiplicity(); k++)
			{
				if ((i == k) || (j == k) || (j > k))
				{
					continue;
				}
				griffin_hit3 = fGrif->GetGriffinHit(k);

				if (IsTwoPhotonCoincidenceInTimeStampUnits(griffin_hit2->GetTimeStamp() * 10, griffin_hit3->GetTimeStamp() * 10) && (!IsInSameClover(griffin_hit1->GetDetector(), griffin_hit2->GetDetector())) && (!IsInSameClover(griffin_hit1->GetDetector(), griffin_hit3->GetDetector())))
				{
					Double_t gamma_gammasum_time_difference = TMath::Abs(griffin_hit1->GetTimeStamp() * 10 - griffin_hit2->GetTimeStamp() * 10);
					Double_t sum_energy = griffin_hit2->GetEnergy() + griffin_hit3->GetEnergy();
					fH2["singles_gamma_gammasum_energy"]->Fill(griffin_hit1->GetEnergy(), sum_energy);
					fH2["singles_gamma_gammasum_timing"]->Fill(griffin_hit1->GetEnergy(), gamma_gammasum_time_difference);

					Double_t energy_gate = 772.6;
					Int_t gate_half_width = 3;
					if (InsideEnergyGate(griffin_hit1->GetEnergy(), energy_gate, gate_half_width))
					{
						fH2["singles_gamma_gammasum_trigger_773"]->Fill(sum_energy, gamma_gammasum_time_difference);
					}
					// if (IsGammaGammaSumTimestampCoincidence(griffin_hit1->GetTimeStamp() * 10, griffin_hit2->GetTimeStamp() * 10, 0, 500))
					// {
					// 	fH2["singles_gamma_gammasum_0_5000"]->Fill(griffin_hit1->GetEnergy(), sum_energy);
					// }
				} // end two-photon prompt
			}	  // end k
		}		  // end j
	}			  // end symmetric singles
	*/

	// ADDBACK
	for (auto i = 0; i < fGrif->GetAddbackMultiplicity(); ++i)
	{
		int a1 = fGrif->GetAddbackHit(i)->GetArrayNumber(); // crystal number (from 1 to 64) of the first addback hit
		if (a1 == 10 || a1 == 28 || a1 == 57 || a1 == 59)
		{
			continue;
		} // excludes addback hits in which the first hit occurred in one of those 4 crystals with bad resolution

		// second gamma
		for (auto j = i + 1; j < fGrif->GetAddbackMultiplicity(); ++j)
		{
			if (IsGammaGammaCoincidence(fGrif->GetAddbackHit(i)->GetTime(), fGrif->GetAddbackHit(j)->GetTime(), 30))
			{
				Double_t sum_energy = fGrif->GetAddbackHit(i)->GetEnergy() + fGrif->GetAddbackHit(j)->GetEnergy();

				// ZDS coincidence if (gotZDS)
				{
					for (auto k = 0; k < fZDS->GetMultiplicity(); ++k)
					{
						TZeroDegreeHit *zds_hit = fZDS->GetZeroDegreeHit(k);
						if (zds_hit->GetKValue() != zds_kvalue_default)
						{
							continue;
						}

						long beta_time_diff = TMath::Abs(fGrif->GetAddbackHit(i)->GetTime() - zds_hit->GetTime());
						fH2["addback_sum_beta"]->Fill(beta_time_diff, sum_energy);
					}
				}
			}
		}
	}
}