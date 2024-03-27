void GammaGammaCoincidenceSlices()
{
    Int_t gammaGammaTimeMin = 0;   // ns
    Int_t gammaGammaTimeMax = 210; // ns
    Int_t gammaGammaTimeStep = 2;  // ns
    std::vector<Int_t> peakCentroids = {630, 834, 1464};
    Int_t peakFitHalfRange = 15;

    std::ofstream myCsv;
    TFile file("/data_fast/cnatzke/two-photon/72Ge/data/histograms/multiple-runs/sept2019.10hrs.root", "read");

    // --- delayed
    // myCsv.open("data/counts_gamma_gamma_coincidence_delayed.csv");
    // TH2D *matrix = static_cast<TH2D *>(file.Get("delayedGammaGammaTimingMatrix"));
    // --- full range
    myCsv.open("data/counts_gamma_gamma_coincidence.csv");
    TH2D *matrix = static_cast<TH2D *>(file.Get("gammaGammaTimingMatrix"));

    myCsv << "start_time,gate_width,centroid,mean,mean_error,area,area_error" << std::endl;
    Int_t totalBinsX = matrix->GetNbinsX();
    // std::cout << "Total Bins: " << totalBinsX << std::endl;

    std::cout << "Processing " << matrix->GetName() << std::endl;

    TH1D *projection = NULL;
    for (auto windowWidth = gammaGammaTimeMin; windowWidth <= gammaGammaTimeMax; windowWidth = windowWidth + gammaGammaTimeStep)
    {
        Int_t startBin = gammaGammaTimeMin + (totalBinsX / 2) + 1;
        Int_t endBin = windowWidth + (totalBinsX / 2) + 1;
        // std::cout << matrix->GetXaxis()->GetBinLowEdge(startBin) << ", " << matrix->GetXaxis()->GetBinUpEdge(endBin) << std::endl;

        Int_t gateWidth = endBin - startBin;
        if (gateWidth == 0)
        {
            continue;
        }
        std::cout << "\rProcessing gate: " << gateWidth << " ns" << std::flush;

        projection = static_cast<TH1D *>(matrix->ProjectionY("projection", startBin, endBin));

        for (auto &myPeak : peakCentroids)
        {
            // std::cout << "Fitting " << myPeak << ", start " << startBin << ", width " << gateWidth << std::endl;
            Int_t centroidGuess = myPeak + 1;
            TPeakFitter pf(centroidGuess - peakFitHalfRange, centroidGuess + peakFitHalfRange);
            TRWPeak peak(centroidGuess);
            pf.AddPeak(&peak);

            TF1 *peakFunc = peak.GetFitFunction();
            peakFunc->SetParameter(0, 10000);
            peakFunc->SetParLimits(0, 1, 100000);
            peakFunc->SetParameter(1, centroidGuess);
            peakFunc->SetParLimits(1, centroidGuess - 5, centroidGuess + 5);
            peakFunc->SetParameter(2, 1.3);
            peakFunc->SetParLimits(2, 1.0, 2.0);

            pf.Fit(projection, "QLEM");

            myCsv << gammaGammaTimeMin
                  << "," << windowWidth
                  << "," << myPeak
                  << "," << peak.Centroid()
                  << "," << peak.CentroidErr()
                  << "," << peak.Area()
                  << "," << peak.AreaErr()
                  << std::endl;

            delete peakFunc;
        }
    }

    delete projection;
    delete matrix;

    myCsv.close();

    exit(0);
}