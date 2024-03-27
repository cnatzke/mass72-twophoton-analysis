// usage: grsisort -lbq TimeRandomSubtraction.C
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>

void runTimeRandomSubtraction(const char *inputFile, const char *outputFile, TString &histogramName, bool createProjections, bool drawGates)
{
    Float_t twoPhotonCoincidenceTime = 90;
    Float_t timeRandomTimeMin = 1000;
    Float_t timeRandomTimeMax = 2000;

    // Open input file
    TFile *inFile = new TFile(inputFile);
    if (!inFile || inFile->IsZombie())
    {
        std::cerr << "Error: Failed to open input file." << std::endl;
        return;
    }

    // Open output file
    TFile *outFile = new TFile(outputFile, "RECREATE");
    if (!outFile || outFile->IsZombie())
    {
        std::cerr << "Error: Failed to open output file." << std::endl;
        inFile->Close();
        return;
    }

    TString timeRandomHistogramName = histogramName + "TimeRandom";
    // Get histograms
    TH2 *hist1 = dynamic_cast<TH2 *>(inFile->Get(histogramName.Data()));
    TH2 *hist2 = dynamic_cast<TH2 *>(inFile->Get(timeRandomHistogramName.Data()));
    // TH2 *hist2 = dynamic_cast<TH2 *>(inFile->Get("gggSumEnergyTimeRandomGated"));

    if (!hist1 || !hist2)
    {
        std::cerr << "Error: Failed to retrieve histograms from input file." << std::endl;
        inFile->Close();
        outFile->Close();
        return;
    }

    // Create a copy of hist1 for addition
    TString histCopyName = histogramName + "TimeRandomSubtracted";
    TH2 *hist1Copy = dynamic_cast<TH2 *>(hist1->Clone());
    hist1Copy->SetName(histCopyName.Data());

    // Add histograms with a scaling factor based on times
    Float_t scalingFactor = -twoPhotonCoincidenceTime / (timeRandomTimeMax - timeRandomTimeMin);

    // Make sure errors are calculated properly since we are adding a scaled histogram
    hist1Copy->Sumw2();
    std::cout << "Scaling time-random matrix by: " << std::setprecision(3) << scalingFactor << std::endl;
    hist1Copy->Add(hist2, scalingFactor);

    // Write the summed histogram back to the root file
    outFile->cd();
    hist1Copy->Write("", TObject::kOverwrite);

    if (createProjections)
    {
        std::cout << "Projecting matrix and creating sum spectra" << std::endl;

        // Project out gate spectra
        Int_t peakGateStart = 1709; // bin therefore energy + 1
        Int_t peakGateEnd = 1714;
        Int_t lowBgGateStart = 1701;
        Int_t lowBgGateEnd = 1706;
        Int_t highBgGateStart = 1721;
        Int_t highBgGateEnd = 1726;

        TH1D *gateGamma = nullptr;
        TH1D *peakGate = nullptr;
        TH1D *bgGateLow = nullptr;
        TH1D *bgGateHigh = nullptr;
        if (hist1Copy)
        {
            gateGamma = hist1Copy->ProjectionX("gateGamma");
            peakGate = hist1Copy->ProjectionY("peakGate", peakGateStart, peakGateEnd);
            bgGateLow = hist1Copy->ProjectionY("bgGateLow", lowBgGateStart, lowBgGateEnd);
            bgGateHigh = hist1Copy->ProjectionY("bgGateHigh", highBgGateStart, highBgGateEnd);

            if (!gateGamma)
            {
                std::cerr << "Error: Failed to create projection along y-axis." << std::endl;
                delete inFile;
                delete outFile;
                return;
            }
            if (!peakGate || !bgGateLow || !bgGateHigh)
            {
                std::cerr << "Error: Failed to create projection along y-axis." << std::endl;
                delete inFile;
                delete outFile;
                return;
            }

            if (drawGates)
            {
                std::cout << "Drawing gates for visual check" << std::endl;
                // Draw lines to show where the gates are
                TCanvas *canvas = new TCanvas("canvas", "Canvas", 800, 600);
                gateGamma->Draw("hist");
                gateGamma->GetXaxis()->SetRangeUser(lowBgGateStart - 10, highBgGateEnd + 10);

                TLine peakGateLineLow(peakGateStart - 0.5, gateGamma->GetMinimum(), peakGateStart - 0.5, gateGamma->GetMaximum());
                TLine peakGateLineHigh(peakGateEnd - 0.5, gateGamma->GetMinimum(), peakGateEnd - 0.5, gateGamma->GetMaximum());
                peakGateLineLow.SetLineColor(kRed);
                peakGateLineHigh.SetLineColor(kRed);
                peakGateLineLow.Draw("same");
                peakGateLineHigh.Draw("same");

                TLine lowBgGateLineLow(lowBgGateStart - 0.5, gateGamma->GetMinimum(), lowBgGateStart - 0.5, gateGamma->GetMaximum());
                TLine lowBgGateLineHigh(lowBgGateEnd - 0.5, gateGamma->GetMinimum(), lowBgGateEnd - 0.5, gateGamma->GetMaximum());
                lowBgGateLineLow.SetLineStyle(kDashed);
                lowBgGateLineHigh.SetLineStyle(kDashed);
                lowBgGateLineLow.Draw("same");
                lowBgGateLineHigh.Draw("same");

                TLine highBgGateLineLow(highBgGateStart - 0.5, gateGamma->GetMinimum(), highBgGateStart - 0.5, gateGamma->GetMaximum());
                TLine highBgGateLineHigh(highBgGateEnd - 0.5, gateGamma->GetMinimum(), highBgGateEnd - 0.5, gateGamma->GetMaximum());
                highBgGateLineLow.Draw("same");
                highBgGateLineHigh.Draw("same");

                canvas->SaveAs("./gate_gamma_spectrum_with_gates_drawn.png");

                delete canvas;
            }

            // Reset the zoom since ROOT is weird
            gateGamma->GetXaxis()->SetRangeUser(gateGamma->GetXaxis()->GetXmin(), gateGamma->GetXaxis()->GetXmax());
        }
        else
        {
            std::cerr << "Error: Invalid histogram hist2." << std::endl;
            delete inFile;
            delete outFile;
            return;
        }
        if (peakGate && bgGateLow && bgGateHigh)
        {
            gateGamma->Write();
            peakGate->Write();
            bgGateLow->Write();
            bgGateHigh->Write();
        }
        else
        {
            std::cerr << "Error: Null pointer for gate projection" << std::endl;
        }
    }

    // Close files
    inFile->Close();
    outFile->Close();

    std::cout << "Histogram addition and projection complete." << std::endl;
}

void TimeRandomSubtraction()
{
    // Input and output file names
    TString inputFile = "/data_fast/cnatzke/two-photon/72Ge/data/histograms/triple-gamma-coincidence/multiple-runs/runs_14660_14663.root";
    const char *outputFile = "output.root";
    TString histogram = "gggSumEnergy";
    // TString histogram = "gggSumEnergyGated";
    bool createProjections = true;
    bool drawGates = true;

    // Perform TimeRandomSubtraction
    runTimeRandomSubtraction(inputFile.Data(), outputFile, histogram, createProjections, drawGates);
}
