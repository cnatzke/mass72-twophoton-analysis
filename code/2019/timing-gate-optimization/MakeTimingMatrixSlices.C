
void MakeTimingMatrixSlices()
{
    std::vector<Int_t> gammaGammaCoincidenceLimits = {30, 60, 90, 120, 150};
    Int_t betaGammaTimeMin = -2000; // ns
    Int_t betaGammaTimeMax = 0;     // ns
    Int_t betaGammaTimeStep = 10;   // ns
    Int_t peakOfInterest = 691;
    Int_t peakRangekeV = 3;

    std::ofstream myCsv;
    TFile file("/data_fast/cnatzke/two-photon/72Ge/data/histograms/multiple-runs/sept2019.10hrs.root", "read");

    myCsv.open("background_counts.csv");
    myCsv << "coincidence_time,start_time,end_time,integral" << std::endl;
    for (Int_t myLimit : gammaGammaCoincidenceLimits)
    {
        TH2D *matrix = static_cast<TH2D *>(file.Get(Form("gammaSumBetaTiming%ins", myLimit)));
        Int_t totalBinsX = matrix->GetNbinsX();

        std::cout << "Processing " << matrix->GetName() << std::endl;

        TH1D *projection = NULL;
        for (auto startTime = betaGammaTimeMin; startTime <= betaGammaTimeMax; startTime = startTime + betaGammaTimeStep)
        {
            Int_t startBin = startTime + 2000 + 1;
            for (auto endTime = startTime; endTime <= betaGammaTimeMax; endTime = endTime + betaGammaTimeStep)
            {
                Int_t endBin = endTime + 2000 + 1;
                projection = static_cast<TH1D *>(matrix->ProjectionY("projection", startBin, endBin));
                Int_t integral = projection->Integral(peakOfInterest - peakRangekeV, peakOfInterest + peakRangekeV);

                myCsv << myLimit
                      << "," << startTime
                      << "," << endTime
                      << "," << integral
                      << std::endl;
            }
        }

        delete projection;
        delete matrix;
    }

    myCsv.close();

    exit(0);
}