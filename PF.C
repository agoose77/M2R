#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <fstream>
#include <fstream>
#include <TString.h>
#include <sstream>
#include <TH1.h>
#include <TH2.h>
#include "TCanvas.h"
#include <vector>


void PF() {
  string mystring;
  cout << "Enter the name of the ROOT run" << "\n";
  cin >> mystring;
  string rootext(".root");
  string RF = mystring+rootext;
  TFile *f = TFile::Open(RF.c_str());
  f->Print();
  cout << "Enter the channels you want to calibrate"<< endl;
  Int_t ich,fch;
  cout << "From Channel:"<< endl;
  cin >> ich;
  cout << "Until Channel"<< endl;
  cin >> fch;
  cout << "How many peaks you are expecting"<< endl;
  int peakstofind;
  cin >> peakstofind;
  vector<double> energies(peakstofind);
  cout << "Enter the energies to calibrate from lower to higher"<< endl;
  for (size_t gg = 0; gg < peakstofind; gg++) {
    cout << gg+1 << "/" << peakstofind<<" Energy: ";
    cin >> energies[gg];
  }
  TH1F *histopeaks[200];
    for(int i=ich;i<fch;i++){
      ostringstream name;
      name<<"hadc"<<i;
      f->GetObject(name.str().c_str(),histopeaks[i]);
      TSpectrum *s = new TSpectrum(peakstofind);
      const int npeaks = s->Search(histopeaks[i],2,"",0.001);
      float *xpeaks = s->GetPositionX();
      cout << "channel " << i <<": " <<xpeaks[0]<<"," << xpeaks[1]<<"," << xpeaks[2] << endl;
  cout << endl;
    }
 delete s;
 delete[] histopeaks;
  }
