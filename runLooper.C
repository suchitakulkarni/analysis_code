// This is the main routine which needs to be called
//
#include <string>
#include "TChain.h"
#include "TString.h"

#include "DelphesLooper.h"

int main(int argc, char **argv) 
{

  if (argc < 6) {
    std::cout << "USAGE: runLooper <input_dir> <sample> <output_dir> <xsec> <nevts>" << std::endl;
    return 1;
  }

  std::string input_dir(argv[1]); 
  std::string sample(argv[2]); 
  std::string output_dir(argv[3]);
  TString xsecS(argv[4]);
  TString nevtS(argv[5]);
  float xsec = xsecS.Atof();
  int   nevt = nevtS.Atoi();

  if (nevt < 100) {
    std::cout << "ERROR: nevt ("<<xsec<<") is very low. is this a real sample?" << std::endl;
    return 2;
  }
  if (xsec < 0.0001 || xsec > 300000) {
    std::cout << "ERROR: xsec value ("<<xsec<<") is out of reasonable bounds. is this a real sample?" << std::endl;
    return 2;
  }


// Now create a chain
  TChain *ch = new TChain("Delphes"); 

// find the name of the file
  TString infile = Form("%s/%s*.root",input_dir.c_str(),sample.c_str());
  cout << "input file is " << infile << endl;
// Add the file to chain
  ch->Add(infile);
  cout << "Made the chain " << endl;
// Get entries and check that the chain is non-empty
  int entries = ch->GetEntries();
  cout << "number of entries in the chain are "<< entries << endl;
  if (entries == 0) {
    std::cout << "ERROR: no entries in chain. filename was: " << infile << std::endl;
    return 2;
  }
  if (entries != 0) {
    std::cout << "WARNING: GetEntries ("<<entries<<") doesn't match NEVTS ("<<nevt<<") in this sample. Are you running on a skim? filename was: " << infile << std::endl;
    //return 2;
  }
   
// Now we pass the chain to the looper which is we wrote previously
  DelphesLooper *looper = new DelphesLooper();
// Note that Looper takes chain as an argeument
  looper->loop(ch, sample, output_dir, xsec, nevt); 
  cout << "[runLooper::end] now has quit delphes Looper " << endl;
  return 0;
}

