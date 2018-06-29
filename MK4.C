#include "TFile.h"      //Write root files
#include "TTree.h"      // Make trees
#include "TObject.h"    // Write objects
#include "TCanvas.h"    // TCanvas
#include "TH1.h"        //  1D histograms
#include "TH2.h"        //  2D histograms
#include "TStopwatch.h"  // stopwatch for time
#include "TChain.h"     // Tchain to process several root files at the time
#include "TMath.h"      //TMath::Sort
#include <sstream>      // for ostringstream
#include <stdlib.h>     // srand, rand
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <math.h>       // sqrt
#include <string>       //string

Float_t position(Int_t); // calculates the position using the adc number
Float_t p_alpha(Float_t); // alpha momentum from energy meassured in DSSD
Float_t theta_x(Float_t); // angle between Z axis(reaction target) and X position (DSSD)
Float_t theta_y(Float_t);// angle between Z axis(reaction target) and Y position (DSSD)
Int_t imap(Int_t); // change the maping of thin detectors

// to execute this progam type in root: .x MK3.C++
void MK4(){
//calibration file read
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
/*
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
chain.Add("R188_0.root");*/
//chain.Add("R189_0.root"); // labr experiment
//chain.Add("R156_0.root");// triple alpha
chain.Add("R156_0.root");// triple alpha
//chain.Add("R9_0.root"); //  mask tron course
TFile f("tchain.root","RECREATE"); // opens the root file
  string mystring;
  mystring = "final";
  Int_t eadc[100]; // allocate the space for the adc
  Int_t eampl[100]; // allocate the space for the ampl

  Int_t emult=0;// start Multiplicity variables
  Int_t n_entries = chain.GetEntries(); // number of entries
  cout << "entries: " << n_entries << endl;
  Int_t t=0;

  chain.SetBranchAddress("emult", &emult); // this is to direct the read variables into a virutal branch
  chain.SetBranchAddress("eadc", eadc);// this is without the & because eadc is an array
  chain.SetBranchAddress("eampl",eampl);// this is without the & because eampl is an array
  // carbon 12 from scattered particle
  TH1* Ex_C12       = new TH1F("Ex_C12","Ex_C12",1600,-16,16);
  TH1* E_C12       = new TH1F("E_C12","E_C12",1600,-16,16);
  TH2F *ADCmap =  new TH2F("ADCmap","ADCmap",100,0,100,800,0,16);// maps of adc
  //TH2* mapleft   = new TH2F("mapleft", "Map left", 320, -25, 25 ,320, -25, 25); // histograms declaration for position,in mm 165 bins from 0 to 49.5 mm
  TH2* mapleft   = new TH2F("mapleft", "Map left", 200, 1.5, 3.5 ,200, -0.6, 0.6 );
  TH2* mapright  = new TH2F("mapright", "Map right", 200, 1.5, 3.5 ,200, -0.6, 0.6 );
  TH1F *ceadc[100]; // hitogram array declaration for DSSD
    for(Int_t w=0;w<100;w++) { // loop over the DSSD channels
    std::ostringstream name; // string to call them DSSD_(number)+"root file name
    name<<"ceadc"<<w; // add the number of the detector to the string
    name<< mystring; // concatenate the string;
    ceadc[w] = new TH1F(name.str().c_str(),name.str().c_str(),200,0,10);// name.str().c_str() converts the string to be use in the histogram declaration
  }
  Float_t xl[16],yl[16],xr[16],yr[16],labr[10];
  Float_t xle[16],yle[16],xre[16],yre[16],labre[10];
  Float_t Ethreshold=0.1;
  Float_t Eb=12.3;
  Float_t Pb=sqrt(2*(4.00260)*(Eb));
  Float_t Mc=12.0;
  Float_t Qv = 0; // scattering carbon alpha

  for (Int_t i = 0; i < n_entries; i++) { // over all entries of the chain
  chain.GetEntry(i); // this one directs the attention to the "i" entry
  Int_t xlc=0,xlec=0, // these four lines are for the counters for the indexes
        ylc=0,ylec=0, // in th the position and energy arrays
        xrc=0,xrec=0,
        yrc=0,yrec=0,
        labrc=0,labrec=0, // counters for the labr arrays
        hitondl=0,hitondr=0; /// keep track of hits on detectors
  Int_t s_lxe[16],s_lye[16];
  Int_t s_rxe[16],s_rye[16];
  for (Int_t k = 0; k < emult; k++) {
    Int_t realchan=0; // clean the real channel for the imap
    Float_t realampl=0; // clean the real amplitude
    realchan = imap(eadc[k]);
    realampl= (eampl[k]*calibp0[realchan])+calibp1[realchan];
    ceadc[realchan]->Fill(realampl);
    if (realampl>1) {
      ADCmap->Fill(realchan,realampl);
    }
    if (eadc[k]>=0 && eadc[k]<32) {hitondl++;}
    if (eadc[k]>=32 && eadc[k]<64) {hitondr++;}

           if (eadc[k]>=0 && eadc[k]<16) {
      xl[xlc]=theta_x(position(realchan)); xle[xlec]=realampl;
      xlc++;xlec++;
    } else if (eadc[k]>=16 && eadc[k]<32) {
      yl[ylc]=theta_y(position(realchan)); yle[ylec]=realampl;
      ylc++;ylec++;
    } else if (eadc[k]>=32 && eadc[k]<48) {
      xr[xrc]=theta_x(position(realchan)); xre[xrec]=realampl;
      xrc++;xrec++;
    } else if (eadc[k]>=48 && eadc[k]<64) {
      yr[yrc]=theta_y(position(realchan)); yre[yrec]=realampl;
      yrc++;yrec++;
    }
    else if (eadc[k]>=64 && eadc[k]<73) {
      labr[labrc]=eadc[i]; labre[labrec]=realampl;
      labrc++;labrec++;
    }
  }

  TMath::Sort(int(hitondl/2.0),xle,s_lxe,1);
  TMath::Sort(int(hitondl/2.0),yle,s_lye,1);
  TMath::Sort(int(hitondl/2.0),xle,s_rxe,1);
  TMath::Sort(int(hitondl/2.0),yle,s_rye,1);
    for (int i = 0; i < int(hitondl/2); i++) {
      if (abs(s_lxe[i]-s_lye[i])<Ethreshold) {
        mapleft->Fill(xl[s_lxe[i]],yl[s_lye[i]]);
      }
    }
    for (int i = 0; i < int(hitondr/2); i++) {
      if (abs(s_rxe[i]-s_rye[i])<Ethreshold) {
        mapright->Fill(xr[s_rxe[i]],yr[s_rye[i]]);
      }

    }


    if (xlc==1 && ylc==1 && xle[0]>1.0 && abs(xle[0]-yle[0])<Ethreshold ) { // one hit on detector 1
      //mapleft->Fill(xl[0],yl[0]);
      //cout << "position "<< xl[0] << " , " << yl[0] << endl;
      //cout << "energy "<< xle[0] << " , " << yle[0] << endl;
      Float_t alpha1 = p_alpha((xle[0]+yle[0])/2);
      Float_t alpha1_comp[3]={
        alpha1*sin(xl[0])*cos(yl[0]),
        alpha1*sin(yl[0]),
        alpha1*cos(xl[0])*cos(yl[0])};
      Float_t Pc_sqrt=(alpha1_comp[0]*alpha1_comp[0])+
                      (alpha1_comp[1]*alpha1_comp[1])+
                      ((alpha1_comp[2]-Pb)*(alpha1_comp[2]-Pb));
      Float_t Ec=Pc_sqrt/(2*Mc);
      Float_t ExC12 = Eb + Qv - Ec - ((xle[0]+yle[0])/2);
      E_C12->Fill(Ec);
      Ex_C12->Fill(ExC12);
      //cout << ExC12 << endl;
    }









  }//end of loop over all events
  f.Write("",TObject::kOverwrite); // DO NOT forget to write the root file with dem new histograms
  StopWatch.Stop(); // Measure time for efficiency

  cout << "Total Real Time: "<< StopWatch.RealTime() << "s" << endl;
}// end program
Float_t position(Int_t realchan){
  Float_t div = RAND_MAX;
  Int_t pre_pos=0;
  Float_t pos;
  Float_t LO=-0.5;
  Float_t HI=0.5;
  if (realchan>=0  && realchan<=15){pre_pos=realchan;}// back of the detector
  if (realchan>=16 && realchan<=31){pre_pos=realchan-16;} // front of the detector
  if (realchan>=32 && realchan<=47){pre_pos=realchan-32;}// back of the detector
  if (realchan>=48 && realchan<=63){pre_pos=realchan-48;} // front of the detector
  float r3 = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
  //cout << r3 << endl;
  pos = ((50.0/16)*(pre_pos+0.5+r3))-25.0;
  return pos;

}
Float_t p_alpha(Float_t Energy){
  Float_t pA = sqrt(2*(4.00260)*Energy);
  return pA;
}
Float_t theta_y(Float_t y){
  Float_t thetay=atan(y/45);
  return thetay;
}
Float_t theta_x(Float_t x){
  Float_t thetax=2.35619+atan(x/45);
  return thetax;
}
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
    /*if (a>=32 && a<=39) {C_Chn=a+8;}    // Vertical, meaning back of detector
    if (a>=40 && a<=47) {C_Chn=79-a;}   // Vertical, meaning back of detector
*/
    if (a>=32 && a<=39) {C_Chn=39-a+32;}    // Vertical, meaning back of detector
    if (a>=40 && a<=47) {C_Chn=10;}   // Vertical, meaning back of detector

    if (a>=48 && a<=55) {C_Chn=a+8;}    // Horizontal, meaing front of detector
    if (a>=56 && a<=63) {C_Chn=111-a;}  // Horizontal, meaing front of detector
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
