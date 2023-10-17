#include "BasicHistogramHelper.hh"

void BasicHistogramHelper::CreateHistograms(unsigned int i)
{
	// try and get the cycle length if we have a PPG provided
	if (fPpg != nullptr)
	{
		// the ODB cycle length is in microseconds!
		fCycleLength = fPpg->OdbCycleLength();
		// only print this for one worker, should be the same result for all them after all
		if (i == 0)
		{
			std::stringstream str;
			str << "Got ODB cycle length " << fCycleLength << " ns = " << fCycleLength / 1e6 << " s" << std::endl;
			std::cerr << str.str();
		}
	}
	else if (i == 0)
	{
		std::stringstream str;
		str << DRED << "No ppg provided, can't fill cycle spectra!" << RESET_COLOR << std::endl;
		std::cerr << str.str();
	}

	// some variables to easily change range and binning for multiple histograms at once
	int energyBins = 10000;
	double lowEnergy = 0.;
	double highEnergy = 2000.;

	fH2[i]["zdsMultGriffinMult"] = new TH2I("zdsMultGriffinMult", "ZDS multiplicity vs. GRIFFIN multiplicity (unsuppressed)", 65, -0.5, 64.5, 10, -0.5, 9.5);

	// unsuppressed spectra
	fH1[i]["griffinE"] = new TH1F("griffinE", Form("Unsuppressed griffin energy;energy [keV];counts/%.1f keV", (highEnergy - lowEnergy) / energyBins), energyBins, lowEnergy, highEnergy);
	// suppressed spectra
	fH1[i]["griffinESupp"] = new TH1F("griffinESupp", Form("Suppressed griffin energy;energy [keV];counts/%.1f keV", (highEnergy - lowEnergy) / energyBins), energyBins, lowEnergy, highEnergy);
	// unsuppressed addback spectra
	fH1[i]["griffinEAddback"] = new TH1F("griffinEAddback", Form("Unsuppressed griffin addback energy;energy [keV];counts/%.1f keV", (highEnergy - lowEnergy) / energyBins), energyBins, lowEnergy, highEnergy);

	// timing spectra
	fH2[i]["griffinZdsTS"] = new TH2F("griffinZdsTS", "GRIFFIN crystal vs. GRIFFIN-ZDS timestamp difference (suppressed addback);#DeltaTS_{GRIFFIN-ZDS}", 200, -1000., 1000., 64, 0.5, 64.5);
	fH2[i]["griffinZdsTime"] = new TH2F("griffinZdsTime", "GRIFFIN crystal vs. GRIFFIN-ZDS timing (suppressed addback);#Deltat_{GRIFFIN-ZDS}", 2000, -1000., 1000., 64, 0.5, 64.5);
	fH2[i]["griffinGriffinTS"] = new TH2F("griffinGriffinTS", "GRIFFIN crystal vs. GRIFFIN-GRIFFIN timestamp difference (suppressed addback);#DeltaTS_{GRIFFIN-GRIFFIN}", 2000, -1000., 1000., 64, 0.5, 64.5);
	fH2[i]["griffinGriffinTime"] = new TH2F("griffinGriffinTime", "GRIFFIN crystal vs. GRIFFIN-GRIFFIN timing (suppressed addback);#Deltat_{GRIFFIN-GRIFFIN}", 2000, -1000., 1000., 64, 0.5, 64.5);

	// cycle spectra
	if (fCycleLength > 0.)
	{
		fH2[i]["griffinCycle"] = new TH2F("griffinCycle", "GRIFFIN suppressed addback energy w/ #beta-tag vs. time in cycle;time in cycle [s];energy [keV]", 100 * fCycleLength / 1e6, 0., fCycleLength / 1e6, energyBins / 5, lowEnergy, highEnergy);
		fH1[i]["zdsCycle"] = new TH1F("zdsCycle", "ZDS hits in cycle;time in cycle [s]", 100 * fCycleLength / 1e6, 0., fCycleLength / 1e6);
	}
}

// Coincidences Gates
bool PromptCoincidence(TGriffinHit *h1, TGriffinHit *h2)
{ // Griffin-Griffin
	return -250. <= h2->GetTime() - h1->GetTime() && h2->GetTime() - h1->GetTime() <= 250.;
}
bool TimeRandom(TGriffinHit *h1, TGriffinHit *h2)
{
	return (-500. <= h1->GetTime() - h2->GetTime() && h1->GetTime() - h2->GetTime() <= -250.) || (250. <= h1->GetTime() - h2->GetTime() && h1->GetTime() - h2->GetTime() <= 500.);
}
bool PromptCoincidence(TGriffinHit *h1, TZeroDegreeHit *h2)
{ // Griffin-Zds
	return -200. <= h1->GetTime() - h2->GetTime() && h1->GetTime() - h2->GetTime() <= 20.;
}
bool TimeRandom(TGriffinHit *h1, TZeroDegreeHit *h2)
{
	return (-310. <= h1->GetTime() - h2->GetTime() && h1->GetTime() - h2->GetTime() <= -200.) || (20. <= h1->GetTime() - h2->GetTime() && h1->GetTime() - h2->GetTime() <= 130.);
}

// TODO: Change the function arguments to match the detectors you want to use and the declaration in the header file!
void BasicHistogramHelper::Exec(unsigned int slot, TGriffin &grif, TZeroDegree &zds)
{
	// we use .at() here instead of [] so that we get meaningful error message if a histogram we try to fill wasn't created
	// e.g. because of a typo

	// multiplicities
	fH2[slot].at("zdsMultGriffinMult")->Fill(grif.GetMultiplicity(), zds.GetMultiplicity());

	// loop over unsuppressed griffin hits
	for (int g = 0; g < grif.GetMultiplicity(); ++g)
	{
		auto grif1 = grif.GetGriffinHit(g);
		fH1[slot].at("griffinE")->Fill(grif1->GetEnergy());
	}

	// loop over unsuppressed griffin addback hits
	for (int g = 0; g < grif.GetAddbackMultiplicity(); ++g)
	{
		auto grif1 = grif.GetAddbackHit(g);
		fH1[slot].at("griffinEAddback")->Fill(grif1->GetEnergy());
	}

	if (fCycleLength > 0.)
	{
		for (int z = 0; z < zds.GetMultiplicity(); ++z)
		{
			auto zds1 = zds.GetZeroDegreeHit(z);
			fH1[slot].at("zdsCycle")->Fill(std::fmod(zds1->GetTime(), fCycleLength));
		}
	}
}

void BasicHistogramHelper::EndOfSort(std::shared_ptr<TList> list)
{
	// auto coincident = static_cast<TH2 *>(list->FindObject(fH2[0].at("griffinESuppAddbackMatrixBeta")));
	// if (coincident == nullptr)
	// {
	// 	std::cout << "Failed to find griffinESuppAddbackMatrixBeta histogram in list:" << std::endl;
	// 	list->Print();
	// 	return;
	// }
	// auto timeRandom = static_cast<TH2 *>(list->FindObject(fH2[0].at("griffinESuppAddbackMatrixBetaBg")));
	// if (timeRandom == nullptr)
	// {
	// 	std::cout << "Failed to find griffinESuppAddbackMatrixBetaBg histogram in list:" << std::endl;
	// 	list->Print();
	// 	return;
	// }
}
