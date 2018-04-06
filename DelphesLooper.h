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

class DelphesLooper {
  
 public:

   // Work is done here
    void loop(TChain* chain, std::string sample, std::string output_dir, float xsec, int nevt);
   // Define functions to be public e.g. deltaR, MT etc. 
   TH1         *lep1pT;
   float 	MET;

  private:
  
   TFile * outfile_;
   //Delphes t;
   float evtweight_;
   // Define four vectors as private members when only this function uses it
   // Define them to be global if they need to be used outside as well
};

#endif 

