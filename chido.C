
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
//cin >> mystring; // comment this line for test porpuses if you don't want to write the name of the file each time you run it
mystring = "R175_0"; // this is for test porpuses
string rootext(".root"); // root file extention
string RF = mystring+rootext; // string sum to keep the looks
//calibration file
Float_t calibp0[100]={0}; //allocate space for the values from calibration
Float_t calibp1[100]={0};
Float_t d2=1,d3=0; // clean variables
Int_t d1,fread_dump; // fread_dump is a variable to dump the fread return value, this is only to run the progam without waring
FILE *out; // file declaration
cout<< "Enter the calib file for the run" <<  endl;
string mystringcalib;
//cin >> mystringcalib;
mystringcalib = "calibR175_0.txt";
cout <<  "running..." << endl;
out=fopen(mystringcalib.c_str(),"r"); // opens the calib file with the name typed
  if (out!=NULL) { // only works for actual files
    while(!feof(out))  { // reads until is over
      fread_dump=fscanf(out,"%i %f %f",&d1,&d2,&d3);
      calibp0[d1]=d2; // multiply
      calibp1[d1]=d3; // add
    }
  } else {
      cout << "calib file not found" << endl;
  }
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
  Int_t hitsdssd;

  tree->SetBranchAddress("emult", &emult); // this is to direct the read variables into a virutal branch
  tree->SetBranchAddress("eadc", eadc);// this is without the & because eadc is an array
  tree->SetBranchAddress("eampl",eampl);// this is without the & because eampl is an array


  TH2* mapleft = new TH2F("mapleft", "Map left", 165, -24.75, 24.75 ,165, -24.75, 24.75); // histograms declaration for position,in mm 165 bins from 0 to 49.5 mm
  TH2* mapright = new TH2F("mapright", "Map right", 165, -24.75, 24.75 ,165, -24.75, 24.75);
  //H2* mapleft = new TH2F("mapleft", "Map left", 165, 0, 49.50 ,165, 0, 49.50); // histograms declaration for position,in mm 165 bins from 0 to 49.5 mm
  //TH2* mapright = new TH2F("mapright", "Map right", 165, 0, 49.50 ,165, 0, 49.50);
  TH1* angleft = new TH1F("angleft","Angle left",350,0,3.5); // angles of events from each dssd in rads
  TH1* angright = new TH1F("angright","Angle right",350,0,3.5);
  TH1* histsONdssd = new TH1I("histsONdssd","hits on dssd",10,0,10);

  TH1 *hadcDSSD_[64];
    for(int w=0;w<=63;w++) {
    std::ostringstream name;
    name<<"DSSD_"<<w;
    name<< mystring;
    hadcDSSD_[w] = new TH1F(name.str().c_str(),name.str().c_str(),250,0,10000);
  }
  TH1 *hadcLabr3_[10];
  for(int ww=0;ww<=9;ww++) {
    std::ostringstream name;
    name<<"Labr3_"<<ww;
    name<< mystring;
    hadcLabr3_[ww] = new TH1F(name.str().c_str(),name.str().c_str(),800,0,8000);
  }

  Double_t div = RAND_MAX/1; // random numer generator to disfuse the maps
  Int_t a=200; // clean variable por positions
  for (Int_t i = 0; i < n_entries; i++) { // over all entries of the tree
    tree->GetEntry(i); // this one directs the attention to the "i" entry
    Float_t xl=a; // clean variables
    Float_t yl=a;
    Float_t xr=a;
    Float_t yr=a;
    hitsdssd=0;
    for (Int_t k = 0; k < emult; k++) { // loop over the Multiplicity
        Int_t realchan=0; // clean the real channel for the imap
        Int_t realampl=0;
          if (eadc[k]>=64 && eadc[k]<=73) {
            realchan = eadc[k];
            realampl= (eampl[k]*calibp0[k])+calibp1[k];
            hadcLabr3_[realchan-64]->Fill(realampl); // fill the hitogram number (adc calibrated) witht the amplitude calibrated
          }
          if (eadc[k]<=63 ) { // only channels comming from the dssd, the units here are in channels
            hitsdssd++;
            realchan = imap(eadc[k]); // imap exchange
            realampl= (eampl[k]*calibp0[realchan])+calibp1[realchan]; // new calibrated amplitude
            hadcDSSD_[realchan]->Fill(realampl); // fill the hitogram number (adc calibrated) witht the amplitude calibrated
            if (realchan>=0 && realchan<=15 ){xl=realchan;} //intrinsic  position in the dssd
            if (realchan>=16 && realchan<=31 ){yl=realchan-16;}
            if (realchan>=32 && realchan<=47 ){xr=realchan-32;}
            if (realchan>=48 && realchan<=63 ){yr=realchan-48;}
          }
              if (xr!=a && yr!=a) { // only enters the loop when X,Y are ready
                Float_t xri = (3*(xr+(rand()/div)))-24.75; // disfuse the map with rand, the 3 comes from the conversion to channels to mm
                Float_t yri = (3*(yr+(rand()/div)))-24.75; // -24.75 is the offset for the coordinate transformation
                mapright->Fill(xri,yri); // fill 2D histogram
                Float_t omegaYr=abs(atan(yri/45)); // calculate the angle of pojection Y of the vector from the target to the dssd hit
                Float_t omegaXr=0.7585+atan(xri/45);// calculate the angle of pojection X of the vector from the target to the dssd hit
                Float_t OmegaR =  acos(cos(omegaXr)*cos(omegaYr)); // angle respect to Z axis, Z axis is the beam axis
                angright->Fill(OmegaR); // fill histo of the angle
                xr=a,yr=a; // clean variables
              }
              if (xl!=a && yl!=a) { // same as before but for the left dssd
                Float_t xli = (3*(xl+(rand()/div)))-24.75;
                Double_t yli = (3*(yl+(rand()/div)))-24.75;
                Float_t omegaYl=abs(atan(yli/45));
                Float_t omegaXl=0.7585+atan(xli/45);
                Float_t OmegaL =  acos(cos(omegaXl)*cos(omegaYl));
                mapleft->Fill(xli,yli);
                angleft->Fill(OmegaL);
                xl=a,yl=a;
              }
      //printf("adc: %i ampl:%i \n",realchan,eampl[k]);
    }//end of loop over the event ie. multiplicty
  histsONdssd->Fill(hitsdssd);
  }//end of loop over all events
  f.Write("",TObject::kOverwrite); // DO NOT forget to write the root file with dem new histograms
  StopWatch.Stop(); // Measure time for efficiency
  cout << "Total Real Time: "<< StopWatch.RealTime() << "s" << endl;
} else {
  cout << "file not found" << endl;
}

}// end program

Int_t imap(Int_t a) // IMAP channel exchange for 2 thin dssds
{
Int_t C_Chn=a;
  if (a<64) {
    if (a>=0 && a<=7) {C_Chn=8+a;} //front right vertizal
    if (a>=8 && a<=15) {C_Chn=15-a;}// front right vertical
    if (a>=16 && a<=23) {C_Chn=a+8;} //back right horizontal
    if (a>=24 && a<=31) {C_Chn=47-a;} // back right horizontal
    if (a>=32 && a<=39) {C_Chn=a+8;} // front left y
    if (a>=40 && a<=47) {C_Chn=79-a;} // front left horizontal
    if (a>=48 && a<=55) {C_Chn=a+8;} //back left vertical
    if (a>=56 && a<=63) {C_Chn=111-a;} // back left
  }
  return C_Chn;
}
/*
how to draw
TFile *_file0 = TFile::Open("R175_0.root")
TCanvas *c1 = new TCanvas("c1","multipads",900,700);
c1->Divide(2,0);
c1->cd(1);
mapleft->SetStats(0);
mapleft->Draw("COLZ");
c1->cd(2);
mapright->Draw("COLZ");
mapright->SetStats(0);

*/
