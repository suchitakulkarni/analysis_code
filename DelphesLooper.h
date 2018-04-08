#ifndef DELPHESLOOPER_h
#define DELPHESLOOPER_h

// ROOT includes
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "Math/LorentzVector.h"
#include "TLorentzVector.h"


#include "DelphesRecoClass.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

struct part {
    TLorentzVector vec;
    int id;
    //float mt;
};

class DelphesLooper {
  
 public:

   // Work is done here
    void loop(TChain* chain, std::string sample, std::string output_dir, float xsec, int nevt);
   // Define functions to be public e.g. deltaR, MT etc. 
  void fillHistos(std::map<std::string, TH1*>& h_1d, const std::string& dirname = "", const std::string& s = "");
  float DeltaR(float eta1, float eta2, float phi1, float phi2);
  float DeltaR(LorentzVector v1, LorentzVector v2);
  float DeltaPhi(float phi1, float phi2);
  float MT(float pt1, float phi1, float pt2, float phi2);


  private:

   float MET_;
   float lepPT_;
   float jetPT_;
   float GenMET_;
   int njets_, nlep_;
   TFile * outfile_;
   //Delphes t;
   float evtweight_;
   std::vector<part> leptons_;
   std::vector<part> neutrino_;
   std::vector<part> recoele_;
   std::vector<part> recomuon_;
   // Define four vectors as private members when only this function uses it
   // Define them to be global if they need to be used outside as well
   std::map<std::string, TH1*> h_1d;
   std::map<std::string, TH1*> h_1d_base;
};

#endif 

