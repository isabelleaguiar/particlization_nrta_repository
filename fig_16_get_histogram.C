#include <TFile.h>
#include <TH1D.h>
#include <iostream>
#include <string>

void fig_16_get_histogram() {
    
    TFile *fout = new TFile("fig_16_histograms.root", "RECREATE");

    std::string IDs[] = {"211", "321", "2212", "ch"}; 
    std::string species[] = {"Pion", "Kaon", "Proton", "Charged"};
    std::string gammas[] = {"g0", "g0p5", "g1", "g1p5"};
    std::string baseDir = "./arquivos_root/usual/";

    for (int iSys = 0; iSys < 2; iSys++) {
        
        int centBin = (iSys == 0) ? 4 : 5;
        std::string histName = (iSys == 0) ? "hPtChargedCent_040_050" : "hPtChargedCent_030_040";
        std::string sysLabel = (iSys == 0) ? "Cent1020" : "Cent4050";

        for (int iSpec = 0; iSpec < 4; iSpec++) {
            
            std::string fileNameRef = baseDir + "out_spectraOld_pPb_5p02_gammas_g0_2000_" + IDs[iSpec] + ".root";
            TFile *fRef = TFile::Open(fileNameRef.c_str(), "READ");
            if (!fRef || fRef->IsZombie()) continue;

            TH1D *hRefRaw = (TH1D*)fRef->Get(histName.c_str());
            TH1D *hCentRef = (TH1D*)fRef->Get("hEventCentrality");
            
            TH1D *hRef = (TH1D*)hRefRaw->Clone("hRef_tmp");
            hRef->Scale(1.0 / hCentRef->GetBinContent(centBin), "width");

            for (int iG = 1; iG < 4; iG++) {
                std::string fileName = baseDir + "out_spectraOld_pPb_5p02_gammas_" + gammas[iG] + "_2000_" + IDs[iSpec] + ".root";
                TFile *f = TFile::Open(fileName.c_str(), "READ");
                if (!f || f->IsZombie()) continue;

                TH1D *hSpecRaw = (TH1D*)f->Get(histName.c_str());
                TH1D *hCent = (TH1D*)f->Get("hEventCentrality");

                TH1D *hSpec = (TH1D*)hSpecRaw->Clone("hSpec_tmp");
                hSpec->Scale(1.0 / hCent->GetBinContent(centBin), "width");

                std::string outName = "Ratio_" + species[iSpec] + "_" + gammas[iG] + "_" + sysLabel;
                TH1D *hRatio = (TH1D*)hSpec->Clone(outName.c_str());
                hRatio->SetTitle((species[iSpec] + " " + gammas[iG] + " / g0 (" + sysLabel + ")").c_str());
                
                hRatio->Divide(hRef);

                fout->cd();
                hRatio->Write();

                delete hSpec;
                f->Close();
            }

            delete hRef;
            fRef->Close();
        }
    }

    fout->Close();
    std::cout << "Done!" << std::endl;
}
