// usage: grsisort -lbq TripleGammaCoincidenceCounts.C

void TripleGammaCoincidenceCounts()
{
    Int_t ggTiming = 90;           // ns
    Int_t gateGammaTimeMin = 0;    // ns
    Int_t gateGammaTimeMax = 1999; // ns
    Int_t gateGammaTimeStep = 10;  // ns

    Int_t peakOfInterest = 691;
    Int_t peakMaskHalfWidth = 6; // keV ~4 sigma
    Int_t integralRange = 10;

    std::ofstream myCsv;
    TFile file("/data_fast/cnatzke/two-photon/72Ge/data/histograms/triple-gamma-coincidence/multiple-runs/runs_14660_14663.root", "read");
    TString csvFileName("data/triple-gamma/counts_triple_gamma_coincidence_gated.csv");

    myCsv.open(csvFileName.Data());
    myCsv << "start_time,end_time,integral_left,integral_right,integral_total" << std::endl;
    TH2D *matrix = static_cast<TH2D *>(file.Get("gggGatedTiming"));
    Int_t binOffset = 1001;
    // std::cout << "Total Bins: " << totalBinsX << std::endl;

    // std::cout << "Processing " << matrix->GetName() << std::endl;

    TH1D *projection = NULL;
    for (auto startTime = gateGammaTimeMin; startTime <= gateGammaTimeMax; startTime = startTime + gateGammaTimeStep)
    {
        Int_t startBin = startTime + binOffset;
        // std::cout << matrix->GetXaxis()->GetBinCenter(startBin) << std::endl;
        for (auto endTime = startTime; endTime <= gateGammaTimeMax; endTime = endTime + gateGammaTimeStep)
        {
            if (endTime % 100 == 0)
            {
                std::cout << "\rProcessing start time: " << startTime
                          << " end time: " << endTime
                          << std::flush;
            }

            Int_t endBin = endTime + binOffset;

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