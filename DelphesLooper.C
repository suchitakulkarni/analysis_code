#include "DelphesLooper.h" // This will be written and filled according to the functions used in this file

// C++ related includes

#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <sstream>
#include <stdexcept>

// ROOT related includes

#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TBenchmark.h"
#include "TMath.h"
#include "TF1.h"

// This is plotting utils header
#include "PlotUtilities.h"

float DelphesLooper::DeltaR(float eta1, float eta2, float phi1, float phi2){
    float dEta = eta1 - eta2;
    float dPhi = DeltaPhi(phi1, phi2);
    return TMath::Sqrt(dEta*dEta + dPhi*dPhi);
}

float DelphesLooper::DeltaR(LorentzVector v1, LorentzVector v2){
    return DeltaR(v1.eta(), v2.eta(), v1.phi(), v2.phi());
}

float DelphesLooper::DeltaPhi(float phi1, float phi2){
    float dPhi = phi1 - phi2;
    while (dPhi  >  TMath::Pi()) dPhi -= 2*TMath::Pi();
    while (dPhi <= -TMath::Pi()) dPhi += 2*TMath::Pi();
    return fabs(dPhi);
}

float DelphesLooper::MT(float pt1, float phi1, float pt2, float phi2){
    return sqrt( 2 * pt1 * pt2 * ( 1 - cos( phi1 - phi2 ) ) );
}

//Settings

int lumi = 300; // in fb

//Main function
// The main fuction will take the TChain as an input and will process it 
//

  void DelphesLooper::loop(TChain* chain, std::string sample, std::string output_dir, float xsec, int nevt){
  //TFile f("histos.root", "new");
  cout << "****************************************" << endl;
  cout << "in DelphesLooper now" << endl;

  evtweight_ = lumi*xsec*1000./nevt; //xsec in fb
  //evtweight_ = lumi*xsec*1./nevt; //xsec in pb
  cout << "[DelphesLooper::loop] Event weight for this sample, "<<sample<<", is "<<evtweight_<<endl;

//Set up loop over chain

  unsigned int nEventsDone = 0;
  unsigned int nEventsToDo = chain->GetEntries();
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  cout << "[DelphesLooper::loop] listOfFiles is " << listOfFiles << endl;
  TFile *currentFile = 0;
  std::map<std::string, TH1*> h_1d;
  gROOT->cd();
  TString output_name = Form("%s/%s.root",output_dir.c_str(),sample.c_str());
  cout << "[DelphesLooper::loop] creating output file: " << output_name << endl;
      
  outfile_ = new TFile(output_name.Data(),"RECREATE") ;

// File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
   cout << "[DelphesLooper::loop] Now accesses Delphes looper " << endl;
   cout << "[DelphesLooper::loop] starting new file" << endl;

// Get File Content

    TFile *file = new TFile( currentFile->GetTitle() );
    TString filename(currentFile->GetTitle());
    cout << "[DelphesLooper::loop] file name in Delphes Looper is " << filename << endl;
    
    TTree *tree = (TTree*)file->Get("Delphes");
    DelphesRecoClass d(tree); // Create an instance of the delphes class and connect it to this tree
    cout << "[DelphesLooper::loop] Created delphes reco class instance " << endl;

// Loop over Events in current file
   unsigned int nEventsTree = tree->GetEntriesFast();
   cout << "[DelphesLooper::loop] the number of events in tree are " << nEventsTree << endl;

   for(unsigned int evt = 0; evt < nEventsTree; evt++){  // Real loop over all events
   //for(unsigned int evt = 0; evt < 10; evt++){  // For Testing
        if (evt%1000 == 0) {cout << "[DelphesLooper::loop] evt number is " << evt << endl;}
        njets_ = 0;
        nlep_ = 0;
        leptons_.clear();
        neutrino_.clear();
        //cout << "New event " << endl;
        d.GetEntry(evt);
        /*for (unsigned int genptcl = 0; genptcl < d.Particle_size; genptcl++)
        {
        if (abs(d.Particle_PID[genptcl]) == 11 ||abs(d.Particle_PID[genptcl]) == 13||abs(d.Particle_PID[genptcl]) == 15 )
        {
           int lep1pos, lep2pos;
           if (abs(d.Particle_PID[d.Particle_M1[genptcl]]) == 24){
           lep1pos =  d.Particle_D2[d.Particle_M1[genptcl]];
           lep2pos =  d.Particle_D1[d.Particle_M1[genptcl]];
           part lep1, lep2, GenMET;
           lep1.vec.SetPtEtaPhiM(d.Particle_PT[lep1pos], d.Particle_Eta[lep1pos], d.Particle_Phi[lep1pos], d.Particle_Mass[lep1pos]);
           lep2.vec.SetPtEtaPhiM(d.Particle_PT[lep2pos], d.Particle_Eta[lep2pos], d.Particle_Phi[lep2pos], d.Particle_Mass[lep2pos]);
           lep1.vec.SetPtEtaPhiM(d.Particle_PT[lep1pos], d.Particle_Eta[lep1pos], d.Particle_Phi[lep1pos], d.Particle_Mass[lep1pos]);
           plot1D("Gen lep pT", d.Particle_PT[lep1pos], evtweight_, h_1d, "Gen lep pT", 200, 0, 2000);
           plot1D("Gen W mass", (lep1.vec+lep2.vec).M(), evtweight_, h_1d, "Gen W mass", 100, 50, 200);
           //cout << "Gen Level W mass is "<< (lep1.vec+lep2.vec).M() << endl;
            }
        }
        }
       part recomet;
       recomet.vec.SetPtEtaPhiM(d.MissingET_MET[0], d.MissingET_Eta[0], d.MissingET_Phi[0], 0);
       recomet.id =  0;
       //cout << "total reco leptons = " << d.Electron_size + d.Muon_size << endl;
       if (d.Electron_size + d.Muon_size != 2) continue;
       for (unsigned int elesize = 0; elesize < d.Electron_size; elesize++)
       {
           part recoelectron;
           //cout << "electron number is " << elesize << endl;
           //cout << "Electron pT " << d.Electron_PT[elesize] << endl;
           plot1D("Lepton pT", d.Electron_PT[elesize], evtweight_, h_1d, "Lepton pT", 100, 0, 1000);
           recoelectron.vec.SetPtEtaPhiM(d.Electron_PT[elesize], d.Electron_Eta[elesize], d.Electron_Phi[elesize], 0);
           recoelectron.id =  -11 * d.Electron_Charge[elesize];
           float mt = MT(d.Electron_PT[elesize], d.Electron_Phi[elesize], recomet.vec.Pt(), recomet.vec.Phi());
           //cout << "Computing MT " << mt << endl;
           plot1D("RecoMT", mt, evtweight_, h_1d, "RecoMT", 100, 0, 1000);
       }

       for (unsigned int muonsize = 0; muonsize < d.Muon_size; muonsize++)
       {
           part recomuon;
           //cout << "Muon number is " << muonsize << endl;
           //cout << "Muon pT " << d.Muon_PT[muonsize] << endl;
           plot1D("Lepton pT", d.Muon_PT[muonsize], evtweight_, h_1d, "Lepton pT", 100, 0, 1000);
           recomuon.vec.SetPtEtaPhiM(d.Muon_PT[muonsize], d.Muon_Eta[muonsize], d.Muon_Phi[muonsize], 0);
           recomuon.id =  -13 * d.Muon_Charge[muonsize];
           float mt = MT(d.Muon_PT[muonsize], d.Muon_Phi[muonsize], recomet.vec.Pt(), recomet.vec.Phi());
           //cout << "Computing MT " << mt << endl;
           plot1D("RecoMT", mt, evtweight_, h_1d, "RecoMT", 100, 0, 1000);
       }

        /*for (unsigned int genptcl = 0; genptcl < d.Particle_size; genptcl++)
        {
            if (abs(d.Particle_PID[genptcl]) == 11 ||abs(d.Particle_PID[genptcl]) == 13||abs(d.Particle_PID[genptcl]) == 15 )
            {
                cout << "particle pT is " << d.Particle_PT[genptcl] << endl;
                cout << "particle PID is " << d.Particle_PID[genptcl] << endl;
                part lep;
                lep.vec.SetPtEtaPhiM(d.Particle_PT[genptcl], d.Particle_Eta[genptcl], d.Particle_Phi[genptcl], d.Particle_Mass[genptcl]);
                lep.id = d.Particle_PID[genptcl];
                leptons_.push_back(lep);
            }
        }
       for (unsigned int genptcl = 0; genptcl < d.Particle_size; genptcl++)
       {
           if (abs(d.Particle_PID[genptcl]) == 12 ||abs(d.Particle_PID[genptcl]) == 14||abs(d.Particle_PID[genptcl]) == 16 )
           {
               cout << "particle pT is " << d.Particle_PT[genptcl] << endl;
               cout << "particle PID is " << d.Particle_PID[genptcl] << endl;
               part neutrino;
               neutrino.vec.SetPtEtaPhiM(d.Particle_PT[genptcl], d.Particle_Eta[genptcl], d.Particle_Phi[genptcl], d.Particle_Mass[genptcl]);
               neutrino.id = d.Particle_PID[genptcl];
               neutrino_.push_back(neutrino);
           }
       }*/
       /*cout << "number of jets are " << d.GenJet_size << endl;
       for (unsigned int genjet = 0; genjet < d.GenJet_size; genjet++)
       {
           cout << "jet pT is " << d.GenJet_PT[genjet] << endl;
           cout << "jet Eta is " << d.GenJet_Eta[genjet] << endl;
           njets_++;
       }
       cout << "number of jets in this event are  " << njets_ << endl;*/
       //cout << "leptons_ array size " << leptons_.size() << endl;
       //cout << "neutrino_ array size " << neutrino_.size() << endl;
       GenMET_ = d.GenMissingET_MET[0];
       plot1D("GenMET", GenMET_, evtweight_, h_1d, "GeneratorMET", 100, 0, 1000);
       
       MET_ = d.MissingET_MET[0];
       plot1D("MET", MET_, evtweight_, h_1d, "MET", 100, 0, 1000);
   } // end event loop
   cout << "will now write the historgram" << endl;
   file->Close();
   cout << "closed file" << endl;
   delete file;
   cout<<"[DelphesLooper::loop] File done"<<endl;
} // end file loop
cout << "Now saving plots" << endl;
savePlotsDir(h_1d, outfile_, "");
cout << "[DelphesLooper::loop] Now will quit Delphes Looper " << endl;
outfile_->Write();
outfile_->Close();
delete outfile_;
return;
}

void DelphesLooper::fillHistos(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const std::string& s){
    TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
    if (dir == 0) {
        dir = outfile_->mkdir(dirname.c_str());
    }
    dir->cd();
    plot1D("h_MET"+s, MET_,  evtweight_, h_1d, "MET [GeV]", 200, 0, 2000);
    
    return;
}
