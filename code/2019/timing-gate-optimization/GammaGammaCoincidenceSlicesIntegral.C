void GammaGammaCoincidenceSlices()
{
    Int_t gammaGammaTimeMin = 0;   // ns
    Int_t gammaGammaTimeMax = 210; // ns
    Int_t gammaGammaTimeStep = 2;  // ns
    Int_t peakOfInterest = 834;
    Int_t peakRangekeV = 3;

    std::ofstream myCsv;
    TFile file("/data_fast/cnatzke/two-photon/72Ge/data/histograms/multiple-runs/sept2019.10hrs.10ns.root", "read");

    myCsv.open("data/counts_gamma_gamma_coincidence.csv");
    myCsv << "start_time,end_time,integral" << std::endl;
    TH2D *matrix = static_cast<TH2D *>(file.Get("delayedGammaBetaTimingMatrix"));
    Int_t totalBinsX = matrix->GetNbinsX();
    // std::cout << "Total Bins: " << totalBinsX << std::endl;

    std::cout << "Processing " << matrix->GetName() << std::endl;

    TH1D *projection = NULL;
    for (auto windowWidth = gammaGammaTimeMin; windowWidth <= gammaGammaTimeMax - 1; windowWidth = windowWidth + gammaGammaTimeStep)
    {
        Int_t startBin = gammaGammaTimeMin + (totalBinsX / 2) + 1;
        Int_t endBin = windowWidth + (totalBinsX / 2) + 1;
        // std::cout << matrix->GetXaxis()->GetBinCenter(startBin) << ", " << matrix->GetXaxis()->GetBinCenter(endBin) << std::endl;

        projection = static_cast<TH1D *>(matrix->ProjectionY("projection", startBin, endBin));
        Int_t integral = projection->Integral(peakOfInterest - peakRangekeV, peakOfInterest + peakRangekeV);

        myCsv << gammaGammaTimeMin
              << "," << windowWidth
              << "," << integral
              << std::endl;
    }

    delete projection;
    delete matrix;

    myCsv.close();

    exit(0);
}