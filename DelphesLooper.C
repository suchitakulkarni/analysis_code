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

//Settings

int lumi = 300; // in fb

//Main function
// The main fuction will take the TChain as an input and will process it 
//

  void DelphesLooper::loop(TChain* chain, std::string sample, std::string output_dir, float xsec, int nevt){
  lep1pT = new TH1F("lep1pT", "lep1 pT", 100, 0, 500.);
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
  //std::map<std::string;

  //gROOT->cd();

// File Loop

  while ( (currentFile = (TFile*)fileIter.Next()) ) {
   cout << "[DelphesLooper::loop] Now accesses Delphes looper " << endl; 
   cout << "(TFile*)fileIter.Next()  " << (TFile*)fileIter.Next() << endl;
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
   //for(unsigned int evt = 0; evt < 2500; evt++){  // For Testing
        d.GetEntry(evt);
   } // end event loop
   cout << "will now write the historgram" << endl;
   file->Close();
   delete file;
   cout<<"\nFile done"<<endl;
} // end file loop
cout << "[DelphesLooper::loop] Now will quit Delphes Looper " << endl;
return ;
}
