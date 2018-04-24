
#include "TFile.h"      //Write root files
#include "TTree.h"      // Make trees
#include "TObject.h"    // Write objects
#include "TCanvas.h"    // TCanvas
#include "TH1.h"        //  1D histograms
#include "TH2.h"        //  2D histograms
#include "TStopwatch.h"  // stopwatch for time
#include <sstream>      // for ostringstream
#include <stdlib.h>     // srand, rand
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
int imap(int);
// to execute this progam type in root: .x chido.C++
void chido(){

string mystring; //string to store the name of the file
cout << "Enter the name of the run you want to read WITHOUT extention" << "\n";
cin >> mystring; // comment this line for test porpuses if you don't want to write the name of the file each time you run it
//mystring = "R175_0"; // this is for test porpuses
string rootext(".root"); // root file extention
string RF = mystring+rootext; // string sum to keep the looks
TStopwatch StopWatch; //stopwatch to keep on track of efficiency
StopWatch.Start(); // start of the stopwatch
TFile f(RF.c_str(),"UPDATE"); // opens the root file
if (f.IsOpen()) { // checks if the root file is open
  TTree *tree = (TTree*)f.Get("data"); // "grabs" the tree from the root file and puts in a new direction
  Int_t eadc[100]; // allocate the space for the adc
  Int_t eampl[100]; // allocate the space for the ampl
  Int_t emult=0;// start Multiplicity variables
  Int_t n_entries = tree->GetEntries(); // number of entries
  Int_t t=0;

  tree->SetBranchAddress("emult", &emult); // this is to direct the read variables into a virutal branch
  tree->SetBranchAddress("eadc", eadc);
  tree->SetBranchAddress("eampl",eampl);
  TH2* mapleft = new TH2F("mapleft", "Map left", 165, 0, 49.50 ,165, 0, 49.50); // histograms declaration for position,in mm 165 bins from 0 to 49.5 mm
  TH2* mapright = new TH2F("mapright", "Map right", 165, 0, 49.50 ,165, 0, 49.50);
  TH1* angleft = new TH1F("angleft","Angle left",300,2,3.5); // angles of events from each dssd in rads
  TH1* angright = new TH1F("angright","Angle right",300,2,3.5);


  Double_t div = RAND_MAX/1; // random numer generator to disfuse the maps
  Int_t a=200; // clean variable por positions
  for (Int_t i = 0; i < n_entries; i++) { // over all entries of the tree
    tree->GetEntry(i); // this one directs the attention to the "i" entry
    Float_t xl=a; // clean variables
    Float_t yl=a;
    Float_t xr=a;
    Float_t yr=a;
    for (Int_t k = 0; k < emult; k++) { // loop over the Multiplicity
      Int_t realchan=0; // clean the real channel for the imap
      realchan = imap(eadc[k]); // imap exchange
      if (realchan<=63 ) { // only channels comming from the dssd, the units here are in channels
        if (realchan>=0 && realchan<=15 ){xl=realchan;}
        if (realchan>=16 && realchan<=31 ){yl=realchan-16;}
        if (realchan>=32 && realchan<=47 ){xr=realchan-32;}
        if (realchan>=48 && realchan<=63 ){yr=realchan-48;}
      }
      if (xr!=a && yr!=a) { // only enters the loop when X,Y are ready
        Float_t xri = 3*(xr+(rand()/div)); // disfuse the map with rand, the 3 comes from the conversion to channels to mm
        Double_t yri = 3*(yr+(rand()/div));
        mapright->Fill(xri,yri); // fill 2D histogram
        Float_t omegaYr=atan(yri/45); // calculate the angle of pojection Y of the vector from the target to the dssd hit
        Float_t omegaXr=135-atan(yri/45);// calculate the angle of pojection X of the vector from the target to the dssd hit
        Float_t OmegaR =  acos(cos(omegaXr)*cos(omegaYr)); // angle respect to Z axis, Z axis is the beam axis
        angright->Fill(OmegaR); // fill histo of the angle
        xr=a,yr=a; // clean variables
      }
      if (xl!=a && yl!=a) { // same as before but for the left dssd
        Float_t xli = 3*(xl+(rand()/div));
        Double_t yli = 3*(yl+(rand()/div));
        mapright->Fill(xli,yli);
        Float_t omegaYl=atan(yli/45);
        Float_t omegaXl=135-atan(yli/45);
        Float_t OmegaL =  acos(cos(omegaXl)*cos(omegaYl));
        angleft->Fill(OmegaL);
        xl=a,yl=a;
      }
      //printf("adc: %i ampl:%i \n",realchan,eampl[k]);
    }
    //Energy of Be8 reconstructed
  }
  f.Write("",TObject::kOverwrite); // DO NOT forget to write the root file with dem new histograms
  StopWatch.Stop(); // Measure time for efficiency
  cout << "Total Real Time: "<< StopWatch.RealTime() << "s" << endl;
} else {
  cout << "file not found" << endl;
}

}

Int_t imap(Int_t a) // IMAP channel exchange for 2 thin dssds
{
  Int_t C_Chn=a;
  if (a>=0 && a<=7) {C_Chn=8+a;} //front right vertizal
  if (a>=8 && a<=15) {C_Chn=15-a;}// front right vertical

  if (a>=16 && a<=23) {C_Chn=a+8;} //back right horizontal
  if (a>=24 && a<=31) {C_Chn=47-a;} // back right horizontal

  if (a>=32 && a<=39) {C_Chn=a+8;} // front left y
  if (a>=40 && a<=47) {C_Chn=79-a;} // front left horizontal

  if (a>=48 && a<=55) {C_Chn=a+8;} //back left vertical
  if (a>=56 && a<=63) {C_Chn=111-a;} // back left
  return C_Chn;
}
/*
how to draw
TFile *_file0 = TFile::Open("R156_0.root")
TCanvas *c1 = new TCanvas("c1","multipads",900,700);
c1->Divide(2,0);
c1->cd(1);
mapleft->SetStats(0);
mapleft->Draw("COLZ");
c1->cd(2);
mapright->Draw("COLZ");
mapright->SetStats(0);

*/
