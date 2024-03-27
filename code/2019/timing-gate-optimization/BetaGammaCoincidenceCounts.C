void BetaGammaCoincidenceCounts()
{
    Int_t ggTiming = 90;           // ns
    Int_t betaGammaTimeMin = 0;    // ns
    Int_t betaGammaTimeMax = 1999; // ns
    Int_t betaGammaTimeStep = 10;  // ns

    Int_t peakOfInterest = 691;
    Int_t peakMaskHalfWidth = 6; // keV ~4 sigma
    Int_t integralRange = 10;

    std::ofstream myCsv;
    TFile file("/data_fast/cnatzke/two-photon/72Ge/data/histograms/multiple-runs/sept2019.10hrs.root", "read");
    TString csvFileName("data/counts_beta_gamma_coincidence.csv");

    myCsv.open(csvFileName.Data());
    myCsv << "start_time,end_time,integral_left,integral_right,integral_total" << std::endl;
    TH2D *matrix = static_cast<TH2D *>(file.Get(Form("gammaSumBetaTiming%ins", ggTiming)));
    Int_t totalBinsX = matrix->GetNbinsX();
    // std::cout << "Total Bins: " << totalBinsX << std::endl;

    // std::cout << "Processing " << matrix->GetName() << std::endl;

    TH1D *projection = NULL;
    for (auto startTime = betaGammaTimeMin; startTime <= betaGammaTimeMax; startTime = startTime + betaGammaTimeStep)
    {
        Int_t startBin = startTime + 2000 + 1;
        // std::cout << matrix->GetXaxis()->GetBinCenter(startBin) << std::endl;
        for (auto endTime = startTime; endTime <= betaGammaTimeMax; endTime = endTime + betaGammaTimeStep)
        {
            if (endTime % 100 == 0)
            {
                std::cout << "\rProcessing start time: " << startTime
                          << " end time: " << endTime
                          << std::flush;
            }

            Int_t endBin = endTime + 2000 + 1;

            // --- Project out the delayed gate
            projection = static_cast<TH1D *>(matrix->ProjectionY("projection", startBin, endBin));

            // --- Get the integral of a section to the left and right of the expected peak
            Int_t rangeLeftHigh = peakOfInterest - peakMaskHalfWidth;
            Int_t rangeRightLow = peakOfInterest + peakMaskHalfWidth;

            Int_t integralLeft = projection->Integral(rangeLeftHigh - integralRange, rangeLeftHigh);
            Int_t integralRight = projection->Integral(rangeRightLow, rangeRightLow + integralRange);

            myCsv << startTime
                  << "," << endTime
                  << "," << integralLeft
                  << "," << integralRight
                  << "," << integralLeft + integralRight
                  << std::endl;
        }
    }

    delete projection;
    delete matrix;

    myCsv.close();

    std::cout << "\nFinished processing, data written to: " << csvFileName << std::endl;

    exit(0);
}