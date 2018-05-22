
#include "TFile.h"      //Write root files
#include "TTree.h"      // Make trees
#include "TObject.h"    // Write objects
#include "TCanvas.h"    // TCanvas
#include "TH1.h"        //  1D histograms
#include "TH2.h"        //  2D histograms
#include "TStopwatch.h"  // stopwatch for time
#include "TChain.h"     // Tchain to process several root files at the time
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
void MK2(){

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
      //cout << d1 << "\t" <<calibp0[d1] << "\t" << calibp1[d1] << endl;
    }
  } else {
      cout << "calib file not found" << endl;
  }
TStopwatch StopWatch; //stopwatch to keep on track of efficiency
StopWatch.Start(); // start of the stopwatch
TChain chain("data");

chain.Add("R162_0.root");
chain.Add("R163_0.root");
chain.Add("R164_0.root");
chain.Add("R166_0.root");
chain.Add("R167_0.root");
chain.Add("R168_0.root");
chain.Add("R169_0.root");
chain.Add("R170_0.root");
chain.Add("R171_0.root");
chain.Add("R172_0.root");
chain.Add("R173_0.root");
chain.Add("R174_0.root");
chain.Add("R175_0.root");
chain.Add("R176_0.root");
chain.Add("R180_0.root");
chain.Add("R181_0.root");
chain.Add("R183_0.root");
chain.Add("R184_0.root");
chain.Add("R185_0.root");
chain.Add("R186_0.root");
chain.Add("R187_0.root");
chain.Add("R188_0.root");
chain.Add("R189_0.root");

TFile f("tchain.root","RECREATE"); // opens the root file
string mystring;
mystring = "final";
  Int_t eadc[100]; // allocate the space for the adc
  Int_t eampl[100]; // allocate the space for the ampl
  Int_t emult=0;// start Multiplicity variables
  Int_t n_entries = chain.GetEntries(); // number of entries
  Int_t t=0;
  Int_t hitsdssd,hitslabr3;
  Float_t treshold=0.3;// in MeV
  Float_t P_beam = sqrt(2*12.3*3.7273);

  chain.SetBranchAddress("emult", &emult); // this is to direct the read variables into a virutal branch
  chain.SetBranchAddress("eadc", eadc);// this is without the & because eadc is an array
  chain.SetBranchAddress("eampl",eampl);// this is without the & because eampl is an array
  chain.SetBranchAddress("hitsdssd",&hitsdssd); // Hits on the DSSD  only for this experiment
  chain.SetBranchAddress("hitslabr3",&hitslabr3); // hits on the labr3 only for this experiment
  // For Be8
  TH2* mapleft    = new TH2F("mapleft", "Map left", 165, -24.75, 24.75 ,165, -24.75, 24.75); // histograms declaration for position,in mm 165 bins from 0 to 49.5 mm
  TH2* mapright   = new TH2F("mapright", "Map right", 165, -24.75, 24.75 ,165, -24.75, 24.75);
  TH2* alphatheta = new TH2F("alphatheta", "alphatheta",60,1.8,3,250,0,15); // histograms declaration for position,in mm 165 bins from 0 to 49.5 mm
  TH2* E1vsE2Be8     = new TH2F("E1vsE2Be8", "E1vsE2Be8", 280,0,14 ,280,0,14);
  TH1* angleft    = new TH1F("angleft","Angle left",700,0,7); // angles of events from each dssd in rads
  TH1* angright   = new TH1F("angright","Angle right",700,0,7);
  TH1* Be8        = new TH1F("Be8","Reconstured_Be8",1600,0,16);

  //Momentum reconstrucion for C12
  TH1* C12        = new TH1F("C12","Reconstured_C12",1600,0,16);
  TH1* Q_C12      = new TH1F("Q_C12","Q-value C12",1600,0,16);
  TH2* C12vstheta = new TH2F("C12vstheta", "C12vstheta ",320,0.1,0.85,3000,0,30);

  TH1 *hadcDSSD_[64]; // hitogram array declaration for DSSD
    for(int w=0;w<=63;w++) { // loop over the DSSD channels
    std::ostringstream name; // string to call them DSSD_(number)+"root file name
    name<<"DSSD_"<<w; // add the number of the detector to the string
    name<< mystring; // concatenate the string
    hadcDSSD_[w] = new TH1F(name.str().c_str(),name.str().c_str(),200,0,10);// name.str().c_str() converts the string to be use in the histogram declaration
    //hadcDSSD_[w] = new TH1F(name.str().c_str(),name.str().c_str(),4096,0,4096);// name.str().c_str() converts the string to be use in the histogram declaration
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
  for (Int_t i = 0; i < n_entries; i++) { // over all entries of the chain
    Float_t ylE=0,xlE=0,yA=0,xA=0,xli=0,yli=0,xla=0,yla=0,xra=0,yra=0,xrE=0,yrE=0,xri=0,yri=0,omegaA=0,P12=0,Pa=0,E_C12=0,P_zC12=0,thetaC12=0,Erel=0;
    Int_t e;
    Float_t xl=a,yl=a,xr=a,yr=a; // clean variables for the position comparison
    Int_t hitleft=0,hitright=0;// this is to keep track on the number of this in each detector, usefull for identify how many hits there are
    chain.GetEntry(i); // this one directs the attention to the "i" entry
  /*  if (hitslabr3>0) {
      for (Int_t k = 0; k < emult; k++) { // loop over the Multiplicity for Be 8
        if (eadc[k]>=64 && eadc[k]<=73) {
          Int_t realchan=0; // clean the real channel for the imap
          Float_t realampl=0; // clean the real amplitude
          realchan = eadc[k];// puts the read value into realchan to generate histograms
          realampl= (eampl[k]*calibp0[eadc[k]])+calibp1[eadc[k]];// Energy calibration
          hadcLabr3_[realchan-64]->Fill(realampl); // fill the hitogram number (adc calibrated) witht the amplitude calibrated
        } // Labr3 things
      }
    }*/
    if (hitsdssd==4) { //condition for only 4 hits
      for (Int_t k = 0; k < emult; k++) { // loop over the Multiplicity for Be 8
          Int_t realchan=0; // clean the real channel for the imap
          Float_t realampl=0; // clean the real amplitude
          if (eadc[k]<=63 && hitsdssd==4) { // only channels comming from the dssd, for 2 hits in dssd, reconstruction for Be8
            realchan = eadc[k];
            realampl= (eampl[k]*calibp0[realchan])+calibp1[realchan]; // new calibrated amplitude
            hadcDSSD_[realchan]->Fill(realampl); // fill the hitogram number (adc calibrated) witht the amplitude calibrated
            //intrinsic position in the dssd
            if (realchan>=0  && realchan<=15){xl=realchan;    Epos[0]=k;}// back of the detector
            if (realchan>=16 && realchan<=31){yl=realchan-16; Epos[1]=k;} // front of the detector
            if (realchan>=32 && realchan<=47){xr=realchan-32; Epos[2]=k;}// back of the detector
            if (realchan>=48 && realchan<=63){yr=realchan-48; Epos[3]=k;} // front of the detector
            //The desion for a front AND back hit is when xl and yl are different from "a" which is a dummie value
            if (xl!=a && yl!=a) {// This conditions menas that the program only will enter this loop when a hit infront AND back is registered
              hitleft++;
                  if (hitleft==1) {
                     ylE = (eampl[Epos[1]]*calibp0[Epos[1]])+calibp1[Epos[1]];// the amplitude of the front strip
                     xlE = (eampl[Epos[0]]*calibp0[Epos[0]])+calibp1[Epos[0]]; // amplitude of the back strip
                     xli = (3*(xl+(rand()/div)))-24.75; // calculates x
                     yli = (3*(yl+(rand()/div)))-24.75; // calculates y
                  }
                  if (hitleft==2 && ylE-xlE<treshold) {
                    yA = (eampl[Epos[1]]*calibp0[Epos[1]])+calibp1[Epos[1]];// auxiliar ylE in case the double alpha hit is in the same detector
                    xA = (eampl[Epos[0]]*calibp0[Epos[0]])+calibp1[Epos[0]]; // auxiliar ylE in case the double alpha hit is in the same detector
                      if (yA-xA<treshold) {
                        xla = (3*(xl+(rand()/div)))-24.75; // calculates x
                        yla = (3*(yl+(rand()/div)))-24.75; // calculates y
                        omegaA = OmegaT(xla,yla); // omega from the positions
                        mapleft->Fill(xla,yla);
                        mapleft->Fill(xli,yli);
                        angleft->Fill(OmegaT(xli,yli));
                        angleft->Fill(OmegaT(xla,yla));
                        Float_t P1 = p_alpha(ylE); // momentum for the  ylE energy, fisrt hit
                        Float_t P2 = p_alpha(yA); //  momentum for the second hit
                        Float_t P1_comp[3]={P1*sin(Omega_X(xli))*cos(Omega_Y(yli)), P1*cos(Omega_Y(yli)), P1*cos(Omega_X(xli))*cos(Omega_Y(yli))};// momentum compoents
                        Float_t P2_comp[3]={P2*sin(Omega_X(xla))*cos(Omega_Y(yla)), P2*cos(Omega_Y(yla)), P2*cos(Omega_X(xla))*cos(Omega_Y(yla))};
                        Float_t P_Be=0;
                        for (size_t g = 0; g < 3; g++) {P_Be+=(P1_comp[g]+P2_comp[g])*(P1_comp[g]+P2_comp[g]);}// reconstruted momentum for the P_Be
                        Float_t E_Be=ylE+yA-(P_Be/(2*8.005));// Energy of the Be
                        Be8->Fill(E_Be);
                        E1vsE2Be8->Fill(ylE,yA);
                        alphatheta->Fill(OmegaT(xla,yla),ylE);
                        alphatheta->Fill(omegaA,yA);
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
                  }
                  if (hitright==2 && yrE-xrE<treshold ) {
                    yA = (eampl[Epos[3]]*calibp0[Epos[3]])+calibp1[Epos[3]];// auxiliar yrE in case the double alpha hit is in the same detector
                    xA = (eampl[Epos[2]]*calibp0[Epos[2]])+calibp1[Epos[2]]; // auxiliar yrE in case the double alpha hit is in the same detector
                    if (yA-xA<treshold) {
                      xra = (3*(xr+(rand()/div)))-24.75; // calculates x
                      yra = (3*(yr+(rand()/div)))-24.75; // calculates y
                      omegaA = OmegaT(xra,yra);
                      mapleft->Fill(xra,yra);
                      mapright->Fill(xri,yri);
                      angright->Fill(OmegaT(xri,yri));
                      angleft->Fill(OmegaT(xra,yra));
                      Float_t P1 = p_alpha(yrE);
                      Float_t P2 = p_alpha(yA);
                      Float_t P1_comp[3]={P1*sin(Omega_X(xri))*cos(Omega_Y(yri)), P1*cos(Omega_Y(yri)), P1*cos(Omega_X(xri))*cos(Omega_Y(yri))};
                      Float_t P2_comp[3]={P2*sin(Omega_X(xra))*cos(Omega_Y(yra)), P2*cos(Omega_Y(yra)), P2*cos(Omega_X(xra))*cos(Omega_Y(yra))};
                      Float_t P_Be=0;
                      for (size_t g = 0; g < 3; g++) {P_Be+=(P1_comp[g]+P2_comp[g])*(P1_comp[g]+P2_comp[g]);}
                      Float_t E_Be=yA+yrE-(P_Be/(2*8.005));
                      Be8->Fill(E_Be);
                      E1vsE2Be8->Fill(yrE,yA);
                      alphatheta->Fill(OmegaT(xra,yra),yrE);
                      alphatheta->Fill(omegaA,yA);
                    }
                  }
            }
                    if (hitleft+hitright==2 && ylE-xlE<treshold && yrE-xrE<treshold) {
                      Float_t P1 = p_alpha(yrE);
                      Float_t P2 = p_alpha(ylE);
                      Float_t P1_comp[3]={
                        P1*sin(Omega_X(xri))*cos(Omega_Y(yri)),
                        P1*cos(Omega_Y(yri)),
                        P1*cos(Omega_X(xri))*cos(Omega_Y(yri))};
                      Float_t P2_comp[3]={P2*sin(Omega_X(xli))*cos(Omega_Y(yli)), P2*cos(Omega_Y(yli)), P2*cos(Omega_X(xli))*cos(Omega_Y(yli))};
                      Float_t P_Be=0;
                      for (size_t g = 0; g < 3; g++) {P_Be+=(P1_comp[g]+P2_comp[g])*(P1_comp[g]+P2_comp[g]);}
                      Float_t E_Be=yrE+ylE-(P_Be/(2*8.005));
                      Be8->Fill(E_Be);
                      E1vsE2Be8->Fill(ylE,yrE);
                      alphatheta->Fill(OmegaT(xli,yli),ylE);
                      alphatheta->Fill(OmegaT(xri,yri),yrE);

                    }
          }
      }//end of loop over the event ie. multiplicty
    }// end loop for the 4 hit dssd
    if (hitsdssd==2 && hitslabr3==1) {
      for (Int_t k = 0; k < emult; k++) { // loop over the Multiplicity
        if (eadc[k]>=64 && eadc[k]<=73) {
          Int_t realchan=0; // clean the real channel for the imap
          Float_t realampl=0; // clean the real amplitude
          realchan = eadc[k];// puts the read value into realchan to generate histograms
          realampl= (eampl[k]*calibp0[eadc[k]])+calibp1[eadc[k]];// Energy calibration
          hadcLabr3_[realchan-64]->Fill(realampl); // fill the hitogram number (adc calibrated) witht the amplitude calibrated
        } // Labr3 things
          if (eadc[k]<=63) {
            Int_t realchan=0; // clean the real channel for the imap
            Float_t realampl=0; // clean the real amplitude
            realchan = eadc[k];
            realampl= (eampl[k]*calibp0[realchan])+calibp1[realchan]; // new calibrated amplitude
            hadcDSSD_[realchan]->Fill(realampl);
            if (realchan>=0  && realchan<=15){xl=realchan;    Epos[0]=k;}// back of the detector
            if (realchan>=16 && realchan<=31){yl=realchan-16; Epos[1]=k;} // front of the detector
            if (realchan>=32 && realchan<=47){xr=realchan-32; Epos[2]=k;}// back of the detector
            if (realchan>=48 && realchan<=63){yr=realchan-48; Epos[3]=k;} // front of the detector
            if (xl!=a && yl!=a) {
              ylE = (eampl[Epos[1]]*calibp0[Epos[1]])+calibp1[Epos[1]];// the amplitude of the front strip
              xlE = (eampl[Epos[0]]*calibp0[Epos[0]])+calibp1[Epos[0]]; // amplitude of the back strip
              if (ylE-xlE<treshold) {
                xli = (3*(xl+(rand()/div)))-24.75; // calculates x
                yli = (3*(yl+(rand()/div)))-24.75; // calculates y
                Pa = p_alpha(ylE);
                P_zC12 = P_beam-(pz_(Pa,Omega_X(xli),Omega_Y(yli)));
                P12 = pow(Pa*sin(Omega_X(xli))*cos(Omega_Y(yli)),2) + pow(Pa*cos(Omega_Y(yli)),2) + pow(P_zC12,2);
                E_C12 = P12/(2*12);
                Erel=E_C12;
                C12->Fill(Erel);
                Q_C12->Fill(12.3-E_C12-ylE);
                thetaC12 = acos(P_zC12/sqrt(P12));
                C12vstheta->Fill(thetaC12,Erel);
              }
            }
            if (xr!=a && yr!=a) {
              yrE = (eampl[Epos[3]]*calibp0[Epos[3]])+calibp1[Epos[3]];// the amplitude of the front strip
              xrE = (eampl[Epos[2]]*calibp0[Epos[2]])+calibp1[Epos[2]]; // amplitude of the back strip
              if (yrE-xrE<treshold) {
                xri = (3*(xr+(rand()/div)))-24.75; // calculates x
                yri = (3*(yr+(rand()/div)))-24.75; // calculates y
                Pa = p_alpha(yrE);
                P_zC12 = P_beam-(pz_(Pa,Omega_X(xri),Omega_Y(yri)));
                P12 = pow(Pa*sin(Omega_X(xri))*cos(Omega_Y(yri)),2) + pow(Pa*cos(Omega_Y(yri)),2)+pow(P_zC12,2);
                E_C12 = P12/(2*12);
                Erel=E_C12;
                C12->Fill(Erel);
                Q_C12->Fill(12.3-E_C12-yrE);
                thetaC12 = acos(P_zC12/sqrt(P12));
                C12vstheta->Fill(thetaC12,Erel);
              }
             }
          }

      }//end of loop over the event ie. multiplicty* code */
    }// end if for the 2 hit in dssd





  }//end of loop over all events
  f.Write("",TObject::kOverwrite); // DO NOT forget to write the root file with dem new histograms
  StopWatch.Stop(); // Measure time for efficiency
  cout << "Total Real Time: "<< StopWatch.RealTime() << "s" << endl;
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
  Float_t omegaX=2.35619+atan(x/45);
  return omegaX;
}
Float_t OmegaT(Float_t x, Float_t y) {
  Float_t xrif = x;
  Float_t yrif = y;
  Float_t omegaY=abs(atan(yrif/45)); // calculate the angle of pojection Y of the vector from the target to the dssd hit
  Float_t omegaX=2.35619+atan(xrif/45);// calculate the angle of pojection X of the vector from the target to the dssd hit
  Float_t Omega_angle =  abs(acos(cos(omegaX)*cos(omegaY))); // angle respect to Z axis, Z axis is the beam axis
  return Omega_angle;
}
Float_t OmegaTT(Float_t tx,Float_t ty){
  Float_t Omega_a =  abs(acos(cos(tx)*cos(ty)));
  return Omega_a;
}
Float_t p_alpha(Float_t Energy){
  Float_t pA = sqrt(2*(3.7273)*Energy);
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
