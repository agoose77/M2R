
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
#include <math.h>       // sqrt

Int_t imap(Int_t); // change the maping of thin detectors
Float_t OmegaT(Float_t,Float_t); // scattering angle from positions
Float_t p_alpha(Float_t); // alpha momentum from energy meassured in DSSD
Float_t px_(Float_t,Float_t,Float_t); // component x from Energy,theta x, theta y
Float_t py_(Float_t,Float_t); // componet y from Energy, theta  y
Float_t pz_(Float_t,Float_t,Float_t); // componet z from Energy, Thetha x, theta y
Float_t Omega_X(Float_t); // angle between Z axis(reaction target) and X position (DSSD)
Float_t Omega_Y(Float_t);// angle between Z axis(reaction target) and Y position (DSSD)
Float_t OmegaTT(Float_t,Float_t); // scattering angle from Omega_X and Omega_Y

// to execute this progam type in root: .x chido.C++
void MK1(){

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
mystringcalib = "R178_0.txt";
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
  Int_t hitsdssd,hitslabr3;
  Float_t treshold=0.0003;

  tree->SetBranchAddress("emult", &emult); // this is to direct the read variables into a virutal branch
  tree->SetBranchAddress("eadc", eadc);// this is without the & because eadc is an array
  tree->SetBranchAddress("eampl",eampl);// this is without the & because eampl is an array
  tree->SetBranchAddress("hitsdssd",&hitsdssd); // Hits on the DSSD  only for this experiment
  tree->SetBranchAddress("hitslabr3",&hitslabr3); // hits on the labr3 only for this experiment

  TH2* mapleft = new TH2F("mapleft", "Map left", 165, -24.75, 24.75 ,165, -24.75, 24.75); // histograms declaration for position,in mm 165 bins from 0 to 49.5 mm
  TH2* mapright = new TH2F("mapright", "Map right", 165, -24.75, 24.75 ,165, -24.75, 24.75);
  TH2* alphatheta = new TH2F("alphatheta", "alphatheta",1000,0,30, 350,0,3.5 ); // histograms declaration for position,in mm 165 bins from 0 to 49.5 mm
  TH2* E1vsE2 = new TH2F("E1vsE2", "E1vsE2", 1000,0,30 ,1000,0,30);

  TH1* angleft = new TH1F("angleft","Angle left",350,0,3.5); // angles of events from each dssd in rads
  TH1* angright = new TH1F("angright","Angle right",350,0,3.5);

  //Momentum reconstrucion for 8Be
  TH1* Be8 = new TH1F("Be8","Reconstured_Be8",1000,0,30);

  TH1 *hadcDSSD_[64]; // hitogram array declaration for DSSD
    for(int w=0;w<=63;w++) { // loop over the DSSD channels
    std::ostringstream name; // string to call them DSSD_(number)+"root file name
    name<<"DSSD_"<<w; // add the number of the detector to the string
    name<< mystring; // concatenate the string
    hadcDSSD_[w] = new TH1F(name.str().c_str(),name.str().c_str(),200,0,10);// name.str().c_str() converts the string to be use in the histogram declaration
  }
  TH1 *hadcLabr3_[10]; // hitogram array declaration for DSSD
  for(int ww=0;ww<=9;ww++) {// loop over the LaBr3  channels
    std::ostringstream name;// string to call them Labrr_(number)+"root file name
    name<<"Labr3_"<<ww; // add the number of the detector to the string
    name<< mystring; // concatenate the string
    hadcLabr3_[ww] = new TH1F(name.str().c_str(),name.str().c_str(),4096,0,4096); // name.str().c_str() converts the string to be use in the histogram declaration
  }
  Float_t div = RAND_MAX/1; // random numer generator to disfuse the maps
  Int_t a=200; // clean variable for positions
  Int_t Epos[4]={0};// this will store the index number of this in dssd when the program calculates the position
  for (Int_t i = 0; i < n_entries; i++) { // over all entries of the tree
    Float_t ylE=0,xlE=0,yA=0,xA=0,xli=0,yli=0,xla=0,yla=0,xra=0,yra=0,xrE=0,yrE=0,xri=0,yri=0,omegaA=0;
    // ylE energy of front on left,
    tree->GetEntry(i); // this one directs the attention to the "i" entry
    Float_t xl=a,yl=a,xr=a,yr=a; // clean variables for the position comparison
    Int_t hitleft=0,hitright=0;// this is to keep track on the number of this in each detector, usefull for identify how many hits there are
    for (Int_t k = 0; k < emult; k++) { // loop over the Multiplicity
        Int_t realchan=0; // clean the real channel for the imap
        Float_t realampl=0; // clean the real amplitude

          if (eadc[k]>=64 && eadc[k]<=73) {
            realchan = eadc[k];// puts the read value into realchan to generate histograms
            realampl= (eampl[k]*calibp0[k])+calibp1[k];// Energy calibration
            hadcLabr3_[realchan-64]->Fill(realampl); // fill the hitogram number (adc calibrated) witht the amplitude calibrated
          } // Labr3 things

          if (eadc[k]<=63 && hitsdssd==4) { // only channels comming from the dssd, for 2 hits in dssd, reconstruction for Be8
            realchan = imap(eadc[k]); // imap exchange
            realampl= (eampl[k]*calibp0[k])+calibp1[k]; // new calibrated amplitude
            hadcDSSD_[realchan]->Fill(realampl); // fill the hitogram number (adc calibrated) witht the amplitude calibrated
            //intrinsic position in the dssd
            if (realchan>=0  && realchan<=15){xl=realchan;Epos[0]=k;}// back of the detector
            if (realchan>=16 && realchan<=31){yl=realchan-16;Epos[1]=k;} // front of the detector
            if (realchan>=32 && realchan<=47){xr=realchan-32;Epos[2]=k;}// back of the detector
            if (realchan>=48 && realchan<=63){yr=realchan-48;Epos[3]=k;} // front of the detector
            //The desion for a front AND back hit is when xl and yl are different from "a" which is a dummie value
            if (xl!=a && yl!=a) {// This conditions menas that the program only will enter this loop when a hit infront AND back is registered
              hitleft++;
                  if (hitleft==1) {
                     ylE = (eampl[Epos[1]]*calibp0[Epos[1]])+calibp1[Epos[1]];// the amplitude of the front strip
                     xlE = (eampl[Epos[0]]*calibp0[Epos[0]])+calibp1[Epos[0]]; // amplitude of the back strip
                     xli = (3*(xl+(rand()/div)))-24.75; // calculates x
                     yli = (3*(yl+(rand()/div)))-24.75; // calculates y
                     mapleft->Fill(xli,yli);
                     angleft->Fill(OmegaT(xli,yli));
                  }

                  if (hitleft==2 && ylE-xlE<treshold) {
                    yA = (eampl[Epos[1]]*calibp0[Epos[1]])+calibp1[Epos[1]];// auxiliar ylE in case the double alpha hit is in the same detector
                    xA = (eampl[Epos[0]]*calibp0[Epos[0]])+calibp1[Epos[0]]; // auxiliar ylE in case the double alpha hit is in the same detector
                      if (yA-xA<0.30) {
                        xla = (3*(xl+(rand()/div)))-24.75; // calculates x
                        yla = (3*(yl+(rand()/div)))-24.75; // calculates y
                        omegaA = OmegaT(xla,yla); // omega from the positions
                        mapleft->Fill(xla,yla);
                        angleft->Fill(OmegaT(xla,yla));
                        Float_t P1 = p_alpha(ylE); // momentum for the  ylE energy, fisrt hit
                        Float_t P2 = p_alpha(yA); //  momentum for the second hit
                        Float_t P1_comp[3]={P1*sin(Omega_X(xli))*cos(Omega_Y(yli)), P1*cos(Omega_Y(yli)), P1*cos(Omega_X(xli))*cos(Omega_Y(yli))};// momentum compoents
                        Float_t P2_comp[3]={P2*sin(Omega_X(xla))*cos(Omega_Y(yla)), P2*cos(Omega_Y(yla)), P2*cos(Omega_X(xla))*cos(Omega_Y(yla))};
                        Float_t P_Be=0;
                        for (size_t g = 0; g < 3; g++) {P_Be+=pow(P1_comp[g]+P2_comp[g],2);}// reconstruted momentum for the P_Be
                        Float_t E_Be=P_Be/(2*8.005)-ylE-yA;// Energy of the Be
                        Be8->Fill(E_Be);
                        E1vsE2->Fill(ylE,yA);
                        alphatheta->Fill(ylE,OmegaT(xla,yla));
                        alphatheta->Fill(yA,omegaA);
                      }

                  }
            }
            if (xr!=a && yr!=a) { // only enters the loop when X,Y are ready
              hitright++;
                  if (hitright==1) {
                    yrE = (eampl[Epos[3]]*calibp0[Epos[3]])+calibp1[Epos[3]];// the amplitude of the front strip
                    xrE = (eampl[Epos[2]]*calibp0[Epos[2]])+calibp1[Epos[2]]; // amplitude of the back strip
                    xri = (3*(xr+(rand()/div)))-24.75; // calculates x
                    yri = (3*(yr+(rand()/div)))-24.75; // calculates y
                    mapright->Fill(xri,yri);
                    angright->Fill(OmegaT(xri,yri));
                  }
                  if (hitright==2 && yrE-xrE<treshold ) {
                    yA = (eampl[Epos[3]]*calibp0[Epos[3]])+calibp1[Epos[3]];// auxiliar yrE in case the double alpha hit is in the same detector
                    xA = (eampl[Epos[2]]*calibp0[Epos[2]])+calibp1[Epos[2]]; // auxiliar yrE in case the double alpha hit is in the same detector
                    if (yA-xA<0.30) {
                      xra = (3*(xr+(rand()/div)))-24.75; // calculates x
                      yra = (3*(yr+(rand()/div)))-24.75; // calculates y
                      omegaA = OmegaT(xra,yra);
                      mapleft->Fill(xra,yra);
                      angleft->Fill(OmegaT(xra,yra));
                      Float_t P1 = p_alpha(yrE);
                      Float_t P2 = p_alpha(yA);
                      Float_t P1_comp[3]={P1*sin(Omega_X(xri))*cos(Omega_Y(yri)), P1*cos(Omega_Y(yri)), P1*cos(Omega_X(xri))*cos(Omega_Y(yri))};
                      Float_t P2_comp[3]={P2*sin(Omega_X(xra))*cos(Omega_Y(yra)), P2*cos(Omega_Y(yra)), P2*cos(Omega_X(xra))*cos(Omega_Y(yra))};
                      Float_t P_Be=0;
                      for (size_t g = 0; g < 3; g++) {P_Be+=pow(P1_comp[g]+P2_comp[g],2);}
                      Float_t E_Be=P_Be/(2*8.005)-yA-yrE;
                      Be8->Fill(E_Be);
                      E1vsE2->Fill(yrE,yA);
                      alphatheta->Fill(yrE,OmegaT(xra,yra));
                      alphatheta->Fill(yA,omegaA);
                    }

                  }
            }
                    if (hitleft+hitright==2 && ylE-xlE<treshold && yrE-xrE<treshold) {
                      Float_t P1 = p_alpha(yrE);
                      Float_t P2 = p_alpha(ylE);
                      Float_t P1_comp[3]={P1*sin(Omega_X(xri))*cos(Omega_Y(yri)), P1*cos(Omega_Y(yri)), P1*cos(Omega_X(xri))*cos(Omega_Y(yri))};
                      Float_t P2_comp[3]={P2*sin(Omega_X(xli))*cos(Omega_Y(yli)), P2*cos(Omega_Y(yli)), P2*cos(Omega_X(xli))*cos(Omega_Y(yli))};
                      Float_t P_Be=0;
                      for (size_t g = 0; g < 3; g++) {P_Be+=pow(P1_comp[g]+P2_comp[g],2);}
                      Float_t E_Be=P_Be/(2*8.005)-yrE-ylE;
                      Be8->Fill(E_Be);
                      E1vsE2->Fill(ylE,yrE);
                      alphatheta->Fill(ylE,OmegaT(xla,yla));
                      alphatheta->Fill(yrE,OmegaT(xra,yra));

                    }
          }
    }//end of loop over the event ie. multiplicty
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
    // 0 -> 31 are channels in the left detector
    if (a>=0 && a<=7) {C_Chn=8+a;}      // Vertical, meaning back of detector
    if (a>=8 && a<=15) {C_Chn=15-a;}    // Vertical, meaning back of detector
    if (a>=16 && a<=23) {C_Chn=a+8;}    // Horizontal, meaing front of detecto
    if (a>=24 && a<=31) {C_Chn=47-a;}   // Horizontal, meaing front of detecto
    // 32 -> 63 are channels in the right detector
    if (a>=32 && a<=39) {C_Chn=a+8;}    // Vertical, meaning back of detector
    if (a>=40 && a<=47) {C_Chn=79-a;}   // Vertical, meaning back of detector
    if (a>=48 && a<=55) {C_Chn=a+8;}    // Horizontal, meaing front of detector
    if (a>=56 && a<=63) {C_Chn=111-a;}  // Horizontal, meaing front of detector
  }
  return C_Chn;
}
Float_t Omega_Y(Float_t y){
  Float_t omegaY=abs(atan(y/45));
  return omegaY;
}
Float_t Omega_X(Float_t x){
  Float_t omegaX=0.7585+atan(x/45);
  return omegaX;
}
Float_t OmegaT(Float_t x, Float_t y) {
  Float_t xrif = x;
  Float_t yrif = y;
  Float_t omegaY=abs(atan(yrif/45)); // calculate the angle of pojection Y of the vector from the target to the dssd hit
  Float_t omegaX=0.7585+atan(xrif/45);// calculate the angle of pojection X of the vector from the target to the dssd hit
  Float_t Omega_angle =  abs(acos(cos(omegaX)*cos(omegaY))); // angle respect to Z axis, Z axis is the beam axis
  return Omega_angle;
}
Float_t OmegaTT(Float_t tx,Float_t ty){
  Float_t Omega_a =  abs(acos(cos(tx)*cos(ty)));
  return Omega_a;
}
Float_t p_alpha(Float_t Energy){
  Float_t pA = sqrt(2*(4)*Energy);
  return pA;
}
Float_t px_(Float_t P,Float_t thetax,Float_t thetay){
  Float_t px=P*sin(thetax)*cos(thetay);
  return px;
}
Float_t py_(Float_t P, Float_t thetay){
  Float_t py=P*cos(thetay);
  return py;
}
Float_t pz_(Float_t P,Float_t thetax,Float_t thetay){
  Float_t pz=P*cos(thetax)*cos(thetay);
  return pz;
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
