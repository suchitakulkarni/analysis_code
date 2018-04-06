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
  //cout << "(TFile*)fileIter.Next()  " << (TFile*)fileIter.Next() << endl;
  //while ( (currentFile = (TFile*)fileIter.Next()) ) { 
   currentFile = (TFile*)fileIter.Next();
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
        //DelphesRecoClass d(tree); // Create an instance of the delphes class and connect it to this tree
	//cout << "[DelphesLooper::loop] evt number is " << evt << endl;
        //if (evt%5 !=0) continue;
        // Get Event Content
        //cout << "[DelphesLooper::loop] nEventsDone " << nEventsDone << endl;
        //cout << "[DelphesLooper::loop] nEventsToDo " << nEventsToDo << endl;
        //if(nEventsDone >= nEventsToDo-1) {cout << "[DelphesLooper::loop] will now continue" << endl; continue;}
        //if ((evt < 2000) || (evt > 4500) && (evt < 5000)){
        //if ((evt > 2995) && (evt < 3005)) {
        if (evt < 3001) {
        cout << "[DelphesLooper::loop] evt number is " << evt << endl;
        //DelphesRecoClass d(tree); // Create an instance of the delphes class and connect it to this tree
        d.GetEntry(evt);
        //d = 0;
 	}
	//cout << "[DelphesLooper::loop] got an entry " << endl;
        //nEventsDone++;
	//cout << "[DelphesLooper::loop] MET is " << d.MissingET_MET[0] << endl;
	//MET = d.MissingET_MET[0];    
        //cout << "[DelphesLooper::loop] MET is " << MET << endl;
	//cout << "[DelphesLooper::loop] Now fill histo" << endl;
        //lep1pT->Fill(MET);
   } // end event loop
   //lep1pT->Fill(MET);
   cout << "will now write the historgram" << endl;
   file->Close();
   delete file;
   cout<<"\nFile done"<<endl;
   cout << "[DelphesLooper::loop] exits event loop" << endl;
   //lep1pT->Fill(MET);  
   cout << "[DelphesLooper::loop] writing histo file" << endl;
   TFile f("histos.root", "new");
   cout << "[DelphesLooper::loop]  Writing histograms now " << endl;
   //lep1pT->Write();
   //f->Write();
   cout << "[DelphesLooper::loop] Closing file " << endl;
   f.Close();
   cout << "[DelphesLooper::loop] file closed " << endl;
//} // end file loop
cout << "[DelphesLooper::loop] Now will quit Delphes Looper " << endl;
//return 0;
}
