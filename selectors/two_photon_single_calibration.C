#define two_photon_single_calibration_cxx
// The class definition in ggAngularCorrelationSelector.h has been generated automatically
#include "two_photon_single_calibration.h"

// ============  GAMMA-ELECTRON conditions ============ //
// gamma-electron coincidence: -60 ns < pacesTime - griffinTime < 40 ns (100 ns window)
bool CoincidenceCondition(TGriffinHit *hit_one, TPacesHit *hit_two)
{
	return (-60. < (hit_two->GetTime() - hit_one->GetTime())) && ((hit_two->GetTime() - hit_one->GetTime()) < 40.);
}
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

// ============  GAMMA-GAMMA conditions ============ //
// gamma-gamma coincidence: abs(DeltaT) < 100 ns (200 ns window)
bool CoincidenceCondition(TGriffinHit *hit_one, TGriffinHit *hit_two)
{
	return TMath::Abs(hit_one->GetTime() - hit_two->GetTime()) < 100;
}
bool IsGammaGammaCoincidenceInTimeStampUnits(long t1, long t2)
{
	double max_timestamp_difference = 10; // 100 ns
	return TMath::Abs(t1 - t2) < max_timestamp_difference;
}
// restrictive timing for two-photon timing
bool IsTwoPhotonCoincidenceInTimeStampUnits(long t1, long t2)
{
	double max_timestamp_difference = 3; // 30 ns
	return TMath::Abs(t1 - t2) < max_timestamp_difference;
}
bool IsTwoPhotonTimeRandomInTimeStampUnits(long t1, long t2)
{
	double min_timestamp_difference = 50;  // 500 ns
	double max_timestamp_difference = 200; // 2000 ns
	return TMath::Abs(t1 - t2) > min_timestamp_difference && TMath::Abs(t1 - t2) < max_timestamp_difference;
}
bool IsInSameClover(Int_t det1, Int_t det2)
{
	return det1 == det2;
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
Double_t timestamp_bin_max = 3000;
// double gbin=4096.,gx=0.,gy=4096.; // energy range and binning GRIFFIN
double gbinPACES = 8192., gxPACES = 0., gyPACES = 4096.; // energy range and binning PACES - 0.5 keV/channel

void two_photon_single_calibration::CreateHistograms()
{
	std::cout << "creating histograms ..." << std::endl;

	// ZDS singles
	// 20200212//fH1["zE"] = new TH1D("zE","ZeroDegree singles", 1000,0,10000);

	// GRIFFIN singles
	fH1["singles_sum"] = new TH1D("singles_sum", "#gamma sum energy", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH1["singles_sum_samedet"] = new TH1D("singles_sum_samedet", "#gamma sum energy, same clover allowed", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH2["singles_energy_channel"] = new TH2D("singles_energy_channel", "GRIFFIN crystal number vs #gamma energy", 65, 0., 65., gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	// 20200212//fH2["gE_chan"]     = new TH2D("gE_chan", "GRIFFIN crystal number vs #gamma energy", 65, 0., 65., gbin, gx, gy);
	// 20200212//fH2["gEnonsuppr_chan"]     = new TH2D("gEnonsuppr_chan", "GRIFFIN crystal number vs #gamma energy non suppressed", 65, 0., 65., gbin, gx, gy); // 20200129
	// 20200212//fH2["gE_KValue"]   = new TH2D("gE_KValue", "K-value vs #gamma energy", 800, 0., 800., gbin, gx, gy);
	// 20200212//fH1["gEnonsuppr"] = new TH1D("gEnonsuppr", "#gamma Singles;E_{#gamma} [keV]", 8192,0,8192); // gbin, gx, gy);
	// 20200212//fH1["gsE"] = new TH1D("gsE", "#gamma Singles Compton suppressed;E_{#gamma} [keV]", gbin, gx, gy); // NOTE all the other spectra labelled as gE are SUPPRESSED
	// 20200213//fH1["aE"] = new TH1D("aE","#gamma Addback;E_{#gamma} [keV]",gbin, gx, gy);
	// 20200213//fH1["aE_CT"] = new TH1D("aE_CT","#gamma Addback with Crosstalk Corrections;E_{#gamma} [keV]",gbin, gx, gy);

	// GAMMA-GAMMA matrices
	fH2["singles_gamma_gamma"] = new TH2D("singles_gamma_gamma", "#gamma-#gamma, |#Deltat_{#gamma-#gamma}| < 100 ns", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);

	// 3 dimensional matrices
	Int_t bins[3] = {static_cast<Int_t>(gamma_bin_max / gamma_binning), static_cast<Int_t>(gamma_bin_max / gamma_binning), static_cast<Int_t>(timestamp_bin_max / timestamp_binning)};
	Double_t bin_min[3] = {gamma_bin_min, gamma_bin_min, timestamp_bin_min};
	Double_t bin_max[3] = {gamma_bin_max, gamma_bin_max, timestamp_bin_max};
	fHSparse["singles_gamma_gammasum_timing"] = new THnSparseF("singles_gamma_gammasum_timing", "#gamma-#gamma_{sum}, |#Deltat_{#gamma-#gamma}| < 100 ns;#gamma Energy per 1 keV;#gamma_{sum} Energy per 1 keV;#DeltaT [samples]", 3, bins, bin_min, bin_max);
	fHSparse["singles_gamma_gammasum_timing"]->Sumw2();

	// 20200212//fH2["ggE"]  = new TH2D("ggE", "#gamma-#gamma, |#Deltat_{#gamma-#gamma}| < 100 ns", gbin, gx, gy, gbin, gx, gy);
	// 20200212//fH2["ggE_bg"] = new TH2D("ggE_bg"," time-random #gamma-#gamma coincidences",gbin, gx, gy, gbin, gx, gy);
	// 20200212//fH2["ggE_z"] = new TH2D("ggE_z"," #gamma-#gamma coincidences in ZDS coincidence",gbin, gx, gy,gbin, gx, gy);
	// 20200212//fH2["ggE_zbg"] = new TH2D("ggE_zbg"," time-random #gamma-#gamma coincidences in ZDS coincidence",gbin, gx, gy,gbin, gx, gy);
	// 20200212//fH2["gzE"] = new TH2D("gzE"," #gamma-ZDS energy coincidence",gbin, gx, gy,6000,0,6000);
	// 20200212//fH2["gzE_bg"] = new TH2D("gzE_bg","Time random #gamma-ZDS energy coincidence",gbin, gx, gy,6000,0,6000);

	// gamma-beta coincidences (singles)
	fH2["singles_gamma_beta_timing"] = new TH2D("singles_gamma_beta_timing", "#beta-#gamma time difference;Timestamp;Sum Energy per 1 keV", 2000, 0, 2000, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH2["singles_sum_beta"] = new TH2D("singles_sum_beta", "#beta-#gamma_{sum}, |#Deltat_{#gamma-#gamma}| < 30 ns;Timestamp;Sum Energy per 1 keV", 2000, 0, 2000, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH2["singles_sum_gg_beta"] = new TH2D("singles_sum_gg_beta", "#gamma-#gamma_{sum} #beta, |#Deltat_{#gamma-#gamma}| < 30 ns; Sum Energy [keV]; Single #gamma Energy [keV]", gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);
	fH2["singles_sum_beta_timerandom"] = new TH2D("singles_sum_beta_timerandom", "#beta-#gamma_{sum}, |#Deltat_{#gamma-#gamma}| > 500 ns ", 2000, 0, 2000, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);

	// gamma-beta coincidences (Addback)
	fH2["addback_gamma_beta_timing"] = new TH2D("addback_gamma_beta_timing", "Addback #beta-#gamma time difference;Timestamp;Sum Energy per 1 keV", 2000, 0, 2000, gamma_bin_max / gamma_binning, gamma_bin_min, gamma_bin_max);

	// Timing spectra in GRIFFIN singles block
	// 20200212//fH1["ggDeltaT"] = new TH1D("ggDeltaT","#gamma-#gamma time difference;#Deltat [ns]",2000,-1000,1000);
	// 20200212//fH1["zgDeltaT"] = new TH1D("zgDeltaT","#gamma-#beta time difference;#Deltat [ns]",10000,-5000,5000);
	// 20200212//fH1["zgDeltaTimestamp"] = new TH1D("zgDeltaTimestamp","#gamma-#beta time difference;#Deltat [10 ns]",600,-300,300);
	// 20200212//fH1["pgDeltaT"] = new TH1D("pgDeltaT","#gamma-PACES time difference;#Deltat [ns]",6000,-3000,3000);
	// 20200212//fH1["pgDeltaTimestamp"] = new TH1D("pgDeltaTimestamp","#gamma-PACES time difference;#Deltat [10 ns]",600,-300,300);
	// 20200212//fH1["pzDeltaT"] = new TH1D("pzDeltaT","#beta-PACES time difference;#Deltat [ns]",6000,-3000,3000);
	// 20200212//fH1["pzDeltaTimestamp"] = new TH1D("pzDeltaTimestamp","#beta-PACES time difference;#Deltat [10 ns]",600,-300,300);
	// 20200212//fH2["gETimestamp"] = new TH2D("gETimestamp","#gamma Singles vs timestamp;Time(s);Energy(keV)",3600,0,3600,2000,0,1000);
	// 20200212//fH2["gE_DeltaTge"]  = new TH2D("gE_DeltaTge","#gamma energy (keV) vs #DeltaT_{#gamma-e^{-}} (ns)",4000,-2000,2000,gbin,gx,gy);
	// 20200212//fH2["eE_DeltaTge"]  = new TH2D("eE_DeltaTge","electron energy (keV) vs #DeltaT_{#gamma-e^{-}} (ns)",4000,-2000,2000,gbin,gx,gy);
	// 20200212//fH2["gE_DeltaTgz"]  = new TH2D("gE_DeltaTgz","#gamma energy (keV) vs #DeltaT_{#gamma-ZDS} (ns)",4000,-2000,2000,gbin,gx,gy);
	// 20200212//fH2["gE_DeltaTimestampgz"]  = new TH2D("gE_DeltaTimestampgz","#gamma energy (keV) vs #DeltaTimestamp_{#gamma-ZDS} (ns)",400,-2000,2000,gbin,gx,gy);
	// 20200212//fH2["gE_DeltaTgz_noSuppr"]  = new TH2D("gE_DeltaTgz_noSuppr","#gamma energy (keV) vs #DeltaT_{#gamma-ZDS} (ns)",4000,-2000,2000,gbin,gx,gy);
	// 20200212//fH2["gE_DeltaTimestampgz_noSuppr"]  = new TH2D("gE_DeltaTimestampgz_noSuppr","#gamma energy (keV) vs #DeltaTimestamp_{#gamma-ZDS} (ns)",400,-2000,2000,gbin,gx,gy);
	// 20200212//fH2["zE_DeltaTgz"]  = new TH2D("zE_DeltaTgz","#beta energy (keV) vs #DeltaT_{#gamma-ZDS} (ns)",4000,-2000,2000,gbin,gx,gy);
	// 20200212//fH2["zE_DeltaTimestampgz"]  = new TH2D("zE_DeltaTimestampgz","#beta energy (keV) vs #DeltaTimestamp_{#gamma-ZDS} (ns)",400,-2000,2000,gbin,gx,gy);
	// 20200212//fH2["DeltaTgz_chan"] = new TH2D("DeltaTgz_chan","GRIFFIN crystal number vs #DeltaT_{#gamma-ZDS} (ns)",65, 0., 65.,400,-2000,2000);
	// 20200212//fH2["eE_DeltaTez"]  = new TH2D("eE_DeltaTez","electron energy (keV) vs #DeltaT_{electron-ZDS} (ns)",4000,-2000,2000,gbin,gx,gy);
	// 20200212//fH2["eE_DeltaTimestampez"]  = new TH2D("eE_DeltaTimestampez","electron energy (keV) vs #DeltaTimestamp_{electron-ZDS} (ns)",400,-2000,2000,gbin,gx,gy);

	// PACES histograms
	// 20200212//fH2["pE_chan"]   = new TH2D("pE_chan", "PACES crystal number vs e^{-} energy", 6,0,6,gbinPACES, gxPACES, gyPACES);
	// 20200212//fH2["pE_KValue"] = new TH2D("pE_KValue", "K-value vs PACES energy", 800, 0., 800., gbinPACES, gxPACES, gyPACES);
	// 20200212//for (int i=1; i<6; i++) {
	// 20200212//   fH1[Form("pE%i", i)] = new TH1D(Form("pE%i",i), Form("PACES %i energy (keV)", i), gbinPACES, gxPACES, gyPACES);
	// 20200212//   fH1[Form("pE%i_z", i)] = new TH1D(Form("pE%i_z",i), Form("PACES %i energy (keV) in #beta coincidence", i), gbinPACES, gxPACES, gyPACES);
	// 20200212//   fH1[Form("pE%i_zbg", i)] = new TH1D(Form("pE%i_zbg",i), Form("PACES %i energy (keV) in time random #beta coincidence", i), gbinPACES, gxPACES, gyPACES);
	// 20200212//}

	// GRIFFIN addback
	// fH2["aE_clover"] = new TH2D("aE_clover", "GRIFFIN clover number vs #gamma Addback energy", 17, 0., 17., gbin, gx, gy);
	// fH1["asE_CT"] = new TH1D("asE_CT", "#gamma Addback Compton suppressed  with Crosstalk Corrections;E_{#gamma} [keV]", gbin, gx, gy);

	// ADDBACK-ADDBACK matrices
	// fH2["aaE"] = new TH2D("aaE", "addback-addback, |#Deltat_{#gamma-#gamma}| < 100 ns", gbin, gx, gy, gbin, gx, gy);
	// fH2["aaE_bg"] = new TH2D("aaE_bg", " time-random addback-addback coincidences", gbin, gx, gy, gbin, gx, gy);
	// fH2["aaE_z"] = new TH2D("aaE_z", " addback-addback coincidences in ZDS coincidence", gbin, gx, gy, gbin, gx, gy);
	// fH2["aaE_zbg"] = new TH2D("aaE_zbg", " time-random addback-addback coincidences in ZDS coincidence", gbin, gx, gy, gbin, gx, gy);
	// fH2["azE"] = new TH2D("azE", " addback-ZDS energy coincidence", gbin, gx, gy, 6000, 0, 6000);
	// fH2["azE_bg"] = new TH2D("azE_bg", "Time random addback-ZDS energy coincidence", gbin, gx, gy, 6000, 0, 6000);

	// Timing spectra in GRIFFIN addback block
	// fH2["aE_DeltaTae"] = new TH2D("aE_DeltaTae", "addback energy (keV) vs #DeltaT_{addback-e^{-}} (ns)", 4000, -2000, 2000, gbin, gx, gy);
	// fH2["eE_DeltaTae"] = new TH2D("eE_DeltaTae", "electron energy (keV) vs #DeltaT_{addback-e^{-}} (ns)", 4000, -2000, 2000, gbin, gx, gy);
	// fH2["aE_DeltaTaz"] = new TH2D("aE_DeltaTaz", "addback energy (keV) vs #DeltaT_{addback-ZDS} (ns)", 4000, -2000, 2000, gbin, gx, gy);

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

void two_photon_single_calibration::FillHistograms()
{

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

	double check;
	int det; // GRIFFIN detector number
	int clover;
	TGriffinHit *griffin_hit;  // used in addback hit building
	TGriffinHit *griffin_hit2; // used in addback hit building
	float energy = 0;		   // temporary variable to store energy information to fill the add_en vector (addback energy vector)
	float CTenergy = 0;		   // temporary variable to store energy information to fill the add_en vector (addback energy vector)
	bool gotZDS = false;
	bool gotPaces = false;

	// clear previous event vectors
	singles_energy.clear();
	singles_charge.clear();
	singles_detector.clear();
	singles_clover.clear();
	singles_timestamp.clear();

	addSuppr_en.clear();
	addSuppr_CTen.clear();
	addSuppr_time.clear();
	addSuppr_timestamp.clear();
	addSuppr_detector.clear();

	if (fZDS)
		gotZDS = true;
	// bool gotLaBr = false; if(fLaBr) gotLaBr = true;
	// bool gotTAC = false; if(fTAC) gotTAC = true;            //no TAC spectra at the moment
	if (fPaces)
		gotPaces = true;

	double energy_g[fGrif->GetMultiplicity()];
	for (auto i = 0; i < fGrif->GetMultiplicity(); ++i)
	{
		energy_g[i] = 0;
	}
	double energySuppressed_g[fGrif->GetSuppressedMultiplicity(fGriffinBgo)];
	for (auto i = 0; i < fGrif->GetSuppressedMultiplicity(fGriffinBgo); ++i)
	{
		energySuppressed_g[i] = 0;
	}

	// applying energy calibrations
	for (auto i = 0; i < fGrif->GetSuppressedMultiplicity(fGriffinBgo); ++i)
	{
		double charge = fGrif->GetSuppressedHit(i)->GetCharge();
		double energy = fGrif->GetSuppressedHit(i)->GetEnergy();
		long timestamp = fGrif->GetSuppressedHit(i)->GetTimeStamp();
		det = fGrif->GetSuppressedHit(i)->GetArrayNumber();
		clover = fGrif->GetSuppressedHit(i)->GetDetector();

		/*
			  // low energy calibration
			  if (fGrif->GetSuppressedHit(i)->GetCharge() < lowRange[det - 1])
			  {
				  energySuppressed_g[i] = offset_lowEn[det - 1] + gain_lowEn[det - 1] * fGrif->GetSuppressedHit(i)->GetCharge() + non_lin_lowEn[det - 1] * fGrif->GetSuppressedHit(i)->GetCharge() * fGrif->GetSuppressedHit(i)->GetCharge();
				  energySuppressed_g[i] += ((double)rand() / RAND_MAX - 0.5);
			  }
			  // mixed-region energy calibration
			  else if (fGrif->GetSuppressedHit(i)->GetCharge() >= lowRange[det - 1] && fGrif->GetSuppressedHit(i)->GetCharge() < highRange[det - 1])
			  {
				  check = (double)rand() / RAND_MAX;
				  if (check <= 0.5)
				  {
					  energySuppressed_g[i] = offset_lowEn[det - 1] + gain_lowEn[det - 1] * fGrif->GetSuppressedHit(i)->GetCharge() + non_lin_lowEn[det - 1] * fGrif->GetSuppressedHit(i)->GetCharge() * fGrif->GetSuppressedHit(i)->GetCharge();
					  energySuppressed_g[i] += ((double)rand() / RAND_MAX - 0.5);
				  }
				  else
				  {
					  energySuppressed_g[i] = offset_highEn[det - 1] + gain_highEn[det - 1] * fGrif->GetSuppressedHit(i)->GetCharge() + non_lin_highEn[det - 1] * fGrif->GetSuppressedHit(i)->GetCharge() * fGrif->GetSuppressedHit(i)->GetCharge();
					  energySuppressed_g[i] += ((double)rand() / RAND_MAX - 0.5);
				  }
			  }
			  // high-energy calibration
			  else if (fGrif->GetSuppressedHit(i)->GetCharge() >= highRange[det - 1])
			  {
				  energySuppressed_g[i] = offset_highEn[det - 1] + gain_highEn[det - 1] * fGrif->GetSuppressedHit(i)->GetCharge() + non_lin_highEn[det - 1] * fGrif->GetSuppressedHit(i)->GetCharge() * fGrif->GetSuppressedHit(i)->GetCharge();
				  energySuppressed_g[i] += ((double)rand() / RAND_MAX - 0.5);
			  }
		*/

		singles_charge.push_back(charge);
		// singles_energy.push_back(energySuppressed_g[i]);
		singles_energy.push_back(energy);
		singles_detector.push_back(det);
		singles_clover.push_back(clover);
		singles_timestamp.push_back(timestamp);
	}

	// Filling vector containing ADDBACK SUPPRESSED ENERGIES //
	for (auto i = 0; i < fGrif->GetSuppressedMultiplicity(fGriffinBgo); ++i)
	{
		griffin_hit = fGrif->GetSuppressedHit(i);
		det = fGrif->GetSuppressedHit(i)->GetArrayNumber();
		if (griffin_hit->GetKValue() != 700)
		{
			continue;
		} // removes GRIFFIN hits pileup events
		if (skipSuppr.size() > 0 && find(skipSuppr.begin(), skipSuppr.end(), i) != skipSuppr.end())
			continue; // it skips events which were the second hit in an addback hit
		energy = energySuppressed_g[i];
		CTenergy = energySuppressed_g[i];
		if (fGrif->GetSuppressedMultiplicity(fGriffinBgo) > 1)
		{
			for (int j = i + 1; j < fGrif->GetSuppressedMultiplicity(fGriffinBgo); ++j)
			{
				griffin_hit2 = fGrif->GetSuppressedHit(j);
				if (griffin_hit2->GetKValue() != 700)
				{
					continue;
				} // removes GRIFFIN hits pileup events
				if (DefaultGriffinAddback(griffin_hit, griffin_hit2))
				{
					skipSuppr.push_back(j);
					if (energySuppressed_g[j] > 0)
					{
						energy += energySuppressed_g[j];
					} // if statement excludes second hit if it was in one of the (bad) crystals with had gain set to 0 and offset to -1
					if (energySuppressed_g[j] > 0)
					{
						CTenergy -= CTcoeff[griffin_hit->GetDetector() - 1][griffin_hit->GetCrystal()][griffin_hit2->GetCrystal()] * energySuppressed_g[j];
					}
					if (energySuppressed_g[j] > 0)
					{
						CTenergy += (energySuppressed_g[j] - CTcoeff[griffin_hit->GetDetector() - 1][griffin_hit2->GetCrystal()][griffin_hit->GetCrystal()] * energySuppressed_g[i]);
					}
				}
			}
		}
		CTenergy = offset_afterCT[det - 1] + gain_afterCT[det - 1] * CTenergy + non_lin_afterCT[det - 1] * CTenergy * CTenergy; // re-alignment of energy values after CrossTalk correction
		addSuppr_en.push_back(energy);
		addSuppr_CTen.push_back(CTenergy);
		addSuppr_time.push_back(griffin_hit->GetTime());
		addSuppr_timestamp.push_back(griffin_hit->GetTimeStamp());
		addSuppr_detector.push_back(griffin_hit->GetDetector());
	}
	skipSuppr.clear();

	// -------------------------------------------------
	// Filling Loops
	// -------------------------------------------------

	// Non-symmetric singles
	for (unsigned int i = 0; i < singles_energy.size(); ++i)
	{
		fH2["singles_energy_channel"]->Fill(singles_detector.at(i), singles_energy.at(i));

		// zds beta tag
		if (gotZDS)
		{
			for (auto k = 0; k < fZDS->GetMultiplicity(); ++k)
			{
				TZeroDegreeHit *zds_hit = fZDS->GetZeroDegreeHit(k);
				if (zds_hit->GetKValue() != 60)
				{
					continue;
				}

				long beta_time_diff = TMath::Abs(singles_timestamp.at(i) - zds_hit->GetTimeStamp());
				fH2["singles_gamma_beta_timing"]->Fill(beta_time_diff, singles_energy.at(i));
			}
		}

		// second gamma
		for (unsigned int j = i + 1; j < singles_energy.size(); j++)
		{
			if (IsGammaGammaCoincidenceInTimeStampUnits(singles_timestamp.at(i), singles_timestamp.at(j)))
			{
				fH2["singles_gamma_gamma"]->Fill(singles_energy.at(i), singles_energy.at(j));
			}

			if (IsTwoPhotonCoincidenceInTimeStampUnits(singles_timestamp.at(i), singles_timestamp.at(j)))
			{
				fH1["singles_sum_samedet"]->Fill(singles_energy.at(i) + singles_energy.at(j));
				if (!IsInSameClover(singles_clover.at(i), singles_clover.at(j)))
				{
					fH1["singles_sum"]->Fill(singles_energy.at(i) + singles_energy.at(j));
				}
			}
			// ZDS coincidence
			if (gotZDS)
			{
				for (auto k = 0; k < fZDS->GetMultiplicity(); ++k)
				{
					TZeroDegreeHit *zds_hit = fZDS->GetZeroDegreeHit(k);
					if (zds_hit->GetKValue() != 60)
					{
						continue;
					}
					// prompt coincidence
					if (IsTwoPhotonCoincidenceInTimeStampUnits(singles_timestamp.at(i), singles_timestamp.at(j)) && !IsInSameClover(singles_clover.at(i), singles_clover.at(j)))
					{
						long beta_time_diff = TMath::Abs(singles_timestamp.at(i) - zds_hit->GetTimeStamp());

						fH2["singles_sum_beta"]->Fill(beta_time_diff, singles_energy.at(i) + singles_energy.at(j));
						fH2["singles_sum_gg_beta"]->Fill(singles_energy.at(i) + singles_energy.at(j), singles_energy.at(i));
					}
					if (IsTwoPhotonTimeRandomInTimeStampUnits(singles_timestamp.at(i), singles_timestamp.at(j)) && (!IsInSameClover(singles_clover.at(i), singles_clover.at(j))))
					{
						long beta_time_diff = TMath::Abs(singles_timestamp.at(i) - zds_hit->GetTimeStamp());

						fH2["singles_sum_beta_timerandom"]->Fill(beta_time_diff, singles_energy.at(i) + singles_energy.at(j));
					}
				}
			}
		}
	} // end singles fill block

	// symmetric singles
	for (unsigned int i = 0; i < singles_energy.size(); ++i)
	{
		for (unsigned int j = 0; j < singles_energy.size(); j++)
		{
			if (i == j)
			{
				continue;
			}

			for (unsigned int k = 0; k < singles_energy.size(); k++)
			{
				if ((i == k) || (j == k) || (j > k))
				{
					continue;
				}

				if (IsTwoPhotonCoincidenceInTimeStampUnits(singles_timestamp.at(j), singles_timestamp.at(k)) && (!IsInSameClover(singles_clover.at(i), singles_clover.at(j))))
				{
					Double_t gamma_gammasum_time_difference = TMath::Abs(singles_timestamp.at(i) - singles_timestamp.at(j));
					Double_t fill_values[3] = {singles_energy.at(i), singles_energy.at(j) + singles_energy.at(k), gamma_gammasum_time_difference};
					fHSparse["singles_gamma_gammasum_timing"]->Fill(fill_values, 1.0);
				} // end two-photon prompt
			}	  // end k
		}		  // end j
	}			  // end symmetric singles

	/*
	   // ADDBACK SUPPRESSED block
	   for (auto i = 0; i < addSuppr_CTen.size(); ++i)
	   {
		   // zds beta tag
		   if (gotZDS)
		   {
			   for (auto k = 0; k < fZDS->GetMultiplicity(); ++k)
			   {
				   TZeroDegreeHit *zds_hit = fZDS->GetZeroDegreeHit(k);
				   if (zds_hit->GetKValue() != 60)
				   {
					   continue;
				   }
				   long beta_time_diff = TMath::Abs(addSuppr_timestamp.at(i) - zds_hit->GetTimeStamp());
				   fH2["addback_gamma_beta_timing"]->Fill(beta_time_diff, addSupper_CTen.at(i));
			   }
		   }

		   fH1["asE_CT"]->Fill(addSuppr_CTen.at(i));
		   fH2["aE_clover"]->Fill(addSuppr_detector.at(i), addSuppr_CTen.at(i));
		   for (auto j = 0; j < addSuppr_CTen.size(); ++j)
		   {
			   if (i == j)
				   continue;
			   if (CoincidenceCondition(addSuppr_time.at(i), addSuppr_time.at(j)))
			   {
				   fH2["aaE"]->Fill(addSuppr_CTen.at(i), addSuppr_CTen.at(j));
			   }
			   if (BGCoincidenceCondition(addSuppr_time.at(i), addSuppr_time.at(j)))
			   {
				   fH2["aaE_bg"]->Fill(addSuppr_CTen.at(i), addSuppr_CTen.at(j));
			   }
			   if (gotZDS)
			   {
				   for (auto k = 0; k < fZDS->GetMultiplicity(); ++k)
				   {
					   if (fZDS->GetZeroDegreeHit(k)->GetKValue() != 60)
					   {
						   continue;
					   }
					   if (CoincidenceCondition(addSuppr_time.at(i), fZDS->GetZeroDegreeHit(k)))
					   {
						   if (CoincidenceCondition(addSuppr_time.at(i), addSuppr_time.at(j)))
						   {
							   fH2["aaE_z"]->Fill(addSuppr_CTen.at(i), addSuppr_CTen.at(j));
						   }
						   if (BGCoincidenceCondition(addSuppr_time.at(i), addSuppr_time.at(j)))
						   {
							   fH2["aaE_zbg"]->Fill(addSuppr_CTen.at(i), addSuppr_CTen.at(j));
						   }
					   }
				   } // for(auto k = 0; k < fZDS->GetMultiplicity(); ++k)
			   }	  // if(gotZDS)
		   }		  // for(auto j = 0; j < fGrif->GetSuppressedMultiplicity(fGriffinBgo); ++j)

		   // ADDBACKsuppressed-PACES histograms//
		   if (gotPaces)
		   {
			   for (auto j = 0; j < fPaces->GetMultiplicity(); ++j)
			   {
				   if (fPaces->GetPacesHit(j)->GetEnergy() < 50)
				   {
					   continue;
				   } // 50 keV threshold on PACES energy
				   if (fPaces->GetPacesHit(j)->GetKValue() != 100)
				   {
					   continue;
				   } // removes pileup events
				   fH2["aE_DeltaTae"]->Fill(fPaces->GetPacesHit(j)->GetTime() - addSuppr_time.at(i), addSuppr_CTen.at(i));
				   fH2["eE_DeltaTae"]->Fill(fPaces->GetPacesHit(j)->GetTime() - addSuppr_time.at(i), fPaces->GetPacesHit(j)->GetEnergy());
			   }
		   }

		   // ADDBACKsuppressed-ZDS and ADDBACKsuppressed-ZDS-PACES histograms//
		   if (gotZDS)
		   {
			   for (auto k = 0; k < fZDS->GetMultiplicity(); ++k)
			   {
				   if (fZDS->GetZeroDegreeHit(k)->GetKValue() != 60)
				   {
					   continue;
				   }
				   fH2["aE_DeltaTaz"]->Fill(addSuppr_time.at(i) - fZDS->GetZeroDegreeHit(k)->GetTime(), addSuppr_CTen.at(i));
				   if (CoincidenceCondition(addSuppr_time.at(i), fZDS->GetZeroDegreeHit(k)))
				   {
					   fH2["azE"]->Fill(addSuppr_CTen.at(i), fZDS->GetZeroDegreeHit(k)->GetEnergy());
				   }
				   if (BGCoincidenceCondition(addSuppr_time.at(i), fZDS->GetZeroDegreeHit(k)))
				   {
					   fH2["azE_bg"]->Fill(addSuppr_CTen.at(i), fZDS->GetZeroDegreeHit(k)->GetEnergy());
				   }
				   if (gotPaces)
				   {
					   for (auto j = 0; j < fPaces->GetMultiplicity(); ++j)
					   {
						   if (fPaces->GetPacesHit(j)->GetEnergy() < 50)
						   {
							   continue;
						   } // 50 keV threshold on PACES energy
						   if (fPaces->GetPacesHit(j)->GetKValue() != 100)
						   {
							   continue;
						   } // removes pileup events
					   }
				   }
			   } // for(auto k = 0; k < fZDS->GetMultiplicity(); ++k)
		   }	  // if(gotZDS)
	   }		  // for(auto i=0; a1 < fGrif->GetSuppressedAddbackMultiplicity(fGriffinBgo); ++i)
	   */

	addSuppr_en.clear();
	addSuppr_CTen.clear();
	addSuppr_time.clear();
	addSuppr_timestamp.clear();
	addSuppr_detector.clear();
}
