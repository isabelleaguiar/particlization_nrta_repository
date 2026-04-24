#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <iostream>
#include <string>

void fig_3_get_histogram() {

    TFile *fout = new TFile("fig_3_histograms.root", "RECREATE");

    Double_t centralityBins[] = {0, 5, 10, 20, 30, 40, 50, 60, 70, 80};
    Int_t nBins = 9;
    
    std::string species[] = {"Pion", "Kaon", "Proton"};
    std::string gammas[] = {"g0", "g0p5", "g1", "g1p5"};

    
    for (int i = 0; i < 3; i++) { 
        for (int k = 0; k < 4; k++) { 
            
            std::string path = "./arquivos_root/no_decay/meanpT_dN_PbPb_nodecay_" + gammas[k] + ".root";
            TFile *fin = TFile::Open(path.c_str(), "READ");
            
            if (!fin || fin->IsZombie()) {
                std::cout << "Error opening file: " << path << std::endl;
                continue;
            }

            TH2D *h2d = (TH2D*)fin->Get(("fHistPtVsCentralityS_" + species[i]).c_str());
            TH1D *hCent = (TH1D*)fin->Get("hCentS");

            std::string histName = "h_dN_" + species[i] + "_" + gammas[k];
            TH1D *hResult = new TH1D(histName.c_str(), (species[i] + " " + gammas[k]).c_str(), nBins, centralityBins);

            for (int b = 0; b < nBins; b++) {
                int bLo = h2d->GetXaxis()->FindBin(centralityBins[b] + 1e-5);
                int bHi = h2d->GetXaxis()->FindBin(centralityBins[b+1] - 1e-5);

                TH1D *hProj = h2d->ProjectionY("tmp", bLo, bHi);
                Double_t evts = hCent->Integral(hCent->FindBin(centralityBins[b]+1e-5), hCent->FindBin(centralityBins[b+1]-1e-5));
                
                Double_t value = hProj->Integral() / evts;
                Double_t error = TMath::Sqrt(hProj->GetEntries()) / evts;

                hResult->SetBinContent(b + 1, value);
                hResult->SetBinError(b + 1, error);

                delete hProj; 
            }

            fout->cd();
            hResult->Write();
            
            fin->Close();
            std::cout << "Processed and recorded: " << histName << std::endl;
        }
    }

    fout->Close();
    std::cout << "\nDone!" << std::endl;
}
