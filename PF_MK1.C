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
Float_t energyloss(Float_t);
Float_t thetaLaBr(Int_t);
Float_t phiLaBr(Int_t);
// to execute this progam type in root:
void PF_MK1(){
  //calibration file read
  Float_t calibp0[100]={0}; //allocate space for the values from calibration
  Float_t calibp1[100]={0};
  Float_t calibp2[100]={0};
  Float_t calibp3[100]={0};

  Float_t d2=1,d3=0,d4=0,d5=0; // clean variables
  Int_t d1,fread_dump; // fread_dump is a variable to dump the fread return value, this is only to run the progam without waring
  FILE *out; // file declaration
  string mystringcalib;
  //cin >> mystringcalib;
  // ENERGY CALIBRATION
  mystringcalib = "R178_0.txt";

  out=fopen(mystringcalib.c_str(),"r"); // opens the calib file with the name typed
  if (out!=NULL) { // only works for actual files
    cout <<  "running...1st calibration file found" << endl;
    while(!feof(out))  { // reads until is over
      fread_dump=fscanf(out,"%i %f %f %f %f",&d1,&d2,&d3,&d4,&d5);
      calibp0[d1]=d2; // multiply
      calibp1[d1]=d3; // add
      calibp2[d1]=d4; //
      calibp3[d1]=d5;
      //cout << d1 << "\t" <<calibp0[d1] << "\t" << calibp1[d1] << endl;
    }
  } else {
    cout << "1st Energy Calibration file not found" << endl;
  }
  // TIME CALIBRATION
  FILE *out_t;
  Float_t calibt[10][10][4];
  string mystringcalibt;
  mystringcalibt = "tchainshift.txt";
  out_t=fopen(mystringcalibt.c_str(),"r"); // opens the calib file with the name typed
  Int_t fread_dumpt;
  Float_t d0t=1,d1t=0,d2t=0,d3t=0;
  if (out_t!=NULL) { // only works for actual files
    cout <<  "running...2nd calibration file found" << endl;
    while(!feof(out_t))  { // reads until is over
      fread_dumpt=fscanf(out_t,"%f %f %f %f",&d0t,&d1t,&d2t,&d3t);
      Int_t d0i=int(d0t);
      Int_t d1i=int(d1t);
      Int_t d2i=int(d2t);
      calibt[d0i][d1i][d2i]=d3t;
    }
  } else {
    cout << "Time Calibration file not found" << endl;
  }
  ////
  TStopwatch StopWatch; //stopwatch to keep on track of efficiency
  StopWatch.Start(); // start of the stopwatch
  TChain chain("data");
  Int_t n_entries=0;

  // all these data file have the same tac spectra

  chain.Add("rootdata/R172_0.root");
  chain.Add("rootdata/R173_0.root");
  chain.Add("rootdata/R174_0.root");
  chain.Add("rootdata/R175_0.root");
  chain.Add("rootdata/R176_0.root");
  chain.Add("rootdata/R180_0.root");
  chain.Add("rootdata/R181_0.root");
  chain.Add("rootdata/R183_0.root");
  chain.Add("rootdata/R184_0.root");
  chain.Add("rootdata/R185_0.root");
  chain.Add("rootdata/R186_0.root");
  chain.Add("rootdata/R187_0.root");
  chain.Add("rootdata/R188_0.root");
  chain.Add("rootdata/R189_0.root");


  //chain.Add("rootdata/R178_0.root"); // newest calib
  //chain.Add("rootdata/R156_0.root"); // mask
  TFile f("tchain.root","RECREATE"); // opens the root file
  Int_t eadc[100]; // allocate the space for the adc
  Int_t eampl[100]; // allocate the space for the ampl

  Int_t emult=0;// start Multiplicity variables
  n_entries = chain.GetEntries(); // number of entries
  cout << "Total Events: " << n_entries << endl;
  Int_t t=0;

  chain.SetBranchAddress("emult", &emult); // this is to direct the read variables into a virutal branch
  chain.SetBranchAddress("eadc", eadc);// this is without the & because eadc is an array
  chain.SetBranchAddress("eampl",eampl);// this is without the & because eampl is an array
  //tac histogram
  TH1* tac = new TH1F("tac","tac",5000,0,5000);
  // charged particles
  TH1* alphaenergyl = new TH1F("alphaenergyl","alphaenergyl",125,0,10);
  TH1* alphaenergylGATE = new TH1F("alphaenergylGATE","alphaenergylGATE",125,0,10);

  TH2* mapleft              = new TH2F("mapleft", "Map left", 200, 1.5, 3.5 ,200, -0.6, 0.6 );
  TH2* mapleftsingle        = new TH2F("mapleftsingle", "mapleftsingle", 200, 1.5, 3.5 ,200, -0.6, 0.6 );
  TH1* mapleftsinglex       = new TH1F("mapleftsinglex", "mapleftsinglex", 200, 1.5, 3.5);
  TH2* mapleftsingleGATE    = new TH2F("mapleftsingleGATE", "mapleftsingleGATE", 200, 1.5, 3.5 ,200, -0.6, 0.6 );
  TH1* mapleftsingleGATEx   = new TH1F("mapleftsingleGATEx", "mapleftsingleGATEx", 200, 1.5, 3.5);
  //General maps
  TH2F *ADCmap   = new TH2F("ADCmap","ADCmap",100,0,100,800,0,16);// maps of adc

  //hitogramas for gamma spectra

  TH1* labrsingle       = new TH1F("labrsingle","labrsingle",200,0,8);
  TH1* labrsingleGATE   = new TH1F("labrsingleGATE","labrsingleGATE",200,0,8);
  TH1* labrt            = new TH1F("labrt","labrt",200,0,8);


  TH1* mapLaBrthetasingle     = new TH1F("mapLaBrthetasingle","mapLaBrthetasingle",100,-30,360);
  TH1* mapLaBrthetasingleGATE = new TH1F("mapLaBrthetasingleGATE","mapLaBrthetasingleGATE",100,-30,360);
  TH2* mapLaBr                = new TH2F("mapLaBr", "mapLaBr", 100, -30, 360 ,100, -180, 30);

  TH1* mapLaBrthetat   = new TH1F("mapLaBrthetat","mapLaBrthetat",100,-30,360);
  TH1* mapLaBrthetatC1 = new TH1F("mapLaBrthetatC1","mapLaBrthetatC1",100,-30,360);

  TH1* mapLaBrtheta = new TH1F("mapLaBrtheta","mapLaBrtheta",39,-30,360);
  TH1* mapLaBrphi   = new TH1F("mapLaBrphi","mapLaBrphi",100,-180,30);

  TH1F *ceadc[100]; // hitogram array declaration for DSSD
  TH1F *adcraw[100]; // hitogram array declaration for DSSD

  TH1I *thetamap[7];
  TH1I *thetamapg[7];
  Double_t mapintegral[7]={1};
  TH1* thetamapt  = new TH1F("thetamapt","thetamapt",39,-30,360);
  TH1* thetamaptg = new TH1F("thetamaptg","thetamaptg",39,-30,360);
  for (Int_t w = 0; w < 7; w++) {
    std::ostringstream theta,thetag;
    theta<<"th"<<w;
    thetag<<"thg"<<w;
    thetamap[w]  = new TH1I(theta.str().c_str(),theta.str().c_str(),39,-30,360);
    thetamapg[w] = new TH1I(thetag.str().c_str(),thetag.str().c_str(),39,-30,360);

  }

  for(Int_t w=0;w<100;w++) { // loop over the DSSD channels
    std::ostringstream name,namee; // string to call them DSSD_(number)+"root file name
    name<<"ceadc"<<w; // add the number of the detector to the string
    namee<<"adcraw"<<w;
    adcraw[w] = new TH1F(namee.str().c_str(),namee.str().c_str(),5000,0,5000);
    ceadc[w]  = new TH1F(name.str().c_str(),name.str().c_str(),1000,0,10);// name.str().c_str() converts the string to be use in the histogram declaration
  }

  Int_t const numofh = 10  ; // number of hitograms
  TH1F *labr3h[10][numofh]; // hitogram array declaration for labr
  TH1F *labr3hc[10][numofh]; // hitogram array declaration for labr
  TH1F *labr3hcpre[10][numofh]; // hitogram array declaration for labr
  TH1F *labr3hscaled[10];
  for (Int_t i = 0; i < 10; i++) {
    std::ostringstream namescale;
    namescale<<"labrscaled"<<i;
    labr3hscaled[i] = new TH1F(namescale.str().c_str(),namescale.str().c_str(),400,0,10);// name.str().c_str() converts the string to be use in the histogram declaration
    for(Int_t w=0;w<numofh;w++) { // loop over the DSSD channels
      std::ostringstream name,namec,namecc; // string to call them DSSD_(number)+"root file name
      name<<"labr"<<i<<"_"<<w; // add the number of the detector to the string NEED TO KEEP THIS LINE AND THE NEXT ONE DUE TO CALIBRATION NECECITIRES
      labr3h[i][w] = new TH1F(name.str().c_str(),name.str().c_str(),500,150,4150);// name.str().c_str() converts the string to be use in the histogram declaration
      namec<<"labrc"<<i<<"_"<<w; // add the number of the detector to the string
      labr3hc[i][w] = new TH1F(namec.str().c_str(),namec.str().c_str(),400,0,10);// name.str().c_str() converts the string to be use in the histogram declaration
      namecc<<"labrcpre"<<i<<"_"<<w; // add the number of the detector to the string
      labr3hcpre[i][w] = new TH1F(namecc.str().c_str(),namecc.str().c_str(),400,0,10);// name.str().c_str() converts the string to be use in the histogram declaration
    }

  }
  Int_t labr[10];
  Float_t xl[16],yl[16],xr[16],yr[16];
  Float_t xle[16],yle[16],xre[16],yre[16],labre[10];
  Float_t Ethreshold=0.1;
  Float_t Eb=12.3;
  Float_t Pb=sqrt(2*(4.00260)*(Eb));
  Float_t Mc=12.0;
  Float_t Ma=3.7273;
  Float_t Qv = 0; // scattering carbon alpha

  Int_t g=0; //variable for the multihistograms
  for (Int_t i = 0; i < n_entries; i++) { // over all entries of the chain
    chain.GetEntry(i); // this one directs the attention to the "i" entry
    Int_t hitontac=0,
    tactac=0;// tac

    Int_t xlc=0,xlec=0, // these four lines are for the counters for the indexes
    ylc=0,ylec=0, // in th the position and energy arrays
    xrc=0,xrec=0,
    yrc=0,yrec=0,
    labrc=0,labrec=0, // counters for the labr arrays
    hitondl=0,hitondr=0,hitonlabr=0, /// keep track of hits on detectors
    labr_counter=0;
    Int_t labrP=0; // these counters are for the contitions for the search of peaks
    Int_t labrC1=0;
    Int_t s_lxe[16],s_lye[16];
    Int_t s_rxe[16],s_rye[16];

    for (Int_t k = 0; k < emult; k++) {
      Int_t realchan=0; // clean the real channel for the imap
      Float_t realampl=0; // clean the real amplitude
      realchan = imap(eadc[k]);
      if (eadc[k] == 80) {hitontac++; tactac = eampl[k]; tac->Fill(tactac);}
      adcraw[realchan]->Fill(eampl[k]);

    } //  end fisrt loop over multiplicty

    for (Int_t k = 0; k < emult; k++) {
      Int_t realchan=0; // clean the real channel for the imap
      Float_t realampl=0; // clean the real amplitude
      Float_t prerealampl=0;
      realchan = imap(eadc[k]);

      if (realchan>=0 && realchan < 64 ) {
        realampl= ((eampl[k]*calibp0[eadc[k]])+calibp1[eadc[k]]);
        ceadc[realchan]->Fill(realampl);
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

      if (realchan >= 64 &&  realchan <= 73) {

        prerealampl = (eampl[k]*calibp0[eadc[k]])+calibp1[eadc[k]]+(pow(eampl[k],2)*calibp2[eadc[k]])+(pow(eampl[k],3)*calibp3[eadc[k]]);
        realampl =  (pow(prerealampl,0)*calibt[realchan-64][g][0])+
        (pow(prerealampl,1)*calibt[realchan-64][g][1])+
        (pow(prerealampl,2)*calibt[realchan-64][g][2])+
        (pow(prerealampl,3)*calibt[realchan-64][g][3]);
        hitonlabr++;
        labr[labrc]=realchan;
        labre[labrec]=realampl;
        labrc++;labrec++;
        ADCmap->Fill(realchan,realampl);
        if (realampl>0.4) {
          ceadc[realchan]->Fill(realampl);
          labr3h[realchan-64][g]->Fill(eampl[k]); // NEED TO KEEP THIS LINE DUE TO CALIBRATION PROCESS
          labr3hcpre[realchan-64][g]->Fill(prerealampl);
          labr3hc[realchan-64][g]->Fill(realampl);
          if (realampl>1.0) {labr3hscaled[realchan-64]->Fill(realampl);}

          labrt->Fill(realampl);
        }
        if (realampl>5.139 && realampl<5.699) {labrP++;}
        if (realampl>4.33 && realampl<4.53) {labrC1++;}
      }



    } // second loop over multiplicty

    ////////
    TMath::Sort(int(hitondl/2.0),xle,s_lxe,1);
    TMath::Sort(int(hitondl/2.0),yle,s_lye,1);

    for (int f = 0; f < int(hitondl/2.0); f++) {
      if (abs(s_lxe[f]-s_lye[f])<Ethreshold) {
        mapleft->Fill(xl[s_lxe[f]],yl[s_lye[f]]);
      }
    }

    // single hit on left
    if (xlc==1 && ylc==1 && abs(xle[0]-yle[0])<Ethreshold && xrc==0 && yrc==0 && ((xle[0]+yle[0])/2)>0.8 ) { // one hit on detector 1
      mapleftsingle->Fill(xl[s_lxe[0]],yl[s_lye[0]]);
      alphaenergyl->Fill((xle[0]+yle[0])/2);
      mapleftsinglex->Fill(xl[s_lxe[0]]);
      if (hitontac>0 && tactac > 630 && tactac<750 && labrP>0 && hitonlabr==1) {

        mapleftsingleGATEx->Fill(xl[s_lxe[0]]);
        mapleftsingleGATE->Fill(xl[s_lxe[0]],yl[s_lye[0]]);
        alphaenergylGATE->Fill((xle[0]+yle[0])/2);
        for (Int_t e = 0; e < hitonlabr; e++) {
          if ((labr[e]-64)<7 && labre[e]>1.0) {
            thetamapg[labr[e]-64]->Fill( thetaLaBr(labr[e]) );
          }
        }
      }
    }
    // cabron

    //theta map labr toyal

    if (hitonlabr>0) {
      for (Int_t q = 0; q < hitonlabr; q++) {
        if ((labr[q]-64)<7 && labre[q]>1.0) {
          thetamap[labr[q]-64]->Fill( thetaLaBr(labr[q]) );
          mapLaBrthetat->Fill(thetaLaBr(labr[q]));
        }
      }
    }



    if (hitonlabr==1) {
      labrsingle->Fill(labre[0]);
      mapLaBrthetasingle->Fill(thetaLaBr(labr[0]));
      if (xlc==1 && ylc==1 && abs(xle[0]-yle[0])<Ethreshold && xrc==0 && yrc==0
          && ((xle[0]+yle[0])/2)>1.7 && ((xle[0]+yle[0])/2)<2.7 && hitontac>0 && tactac > 630 && tactac<750) {
        labrsingleGATE->Fill(labre[0]);
        mapLaBrthetasingleGATE->Fill(thetaLaBr(labr[0]));


      }
    }


    if (i==int(((n_entries+1)/(numofh))*(g+1))) {
      g++;
      cout << "going good: " << (i*100.0)/n_entries << " % of events processed" << endl;
    }
  }//end of loop over all events

  for (Int_t f = 0; f < 7; f++) {
    mapintegral[f]= thetamap[f]->Integral();
  }
  // for (Int_t f = 0; f < 7; f++) {
  //   cout << mapintegral[4]/mapintegral[f]<< endl;
  // }
  //detector 4 looks like the less effcient so lets calibrate the relative efficiency with it
  for (Int_t t = 0; t < 7; t++) {
    thetamapt->Add(thetamap[t],mapintegral[4]/mapintegral[t]);
    thetamaptg->Add(thetamapg[t],mapintegral[4]/mapintegral[t]);
    labr3hscaled[t]->Scale(mapintegral[4]/mapintegral[t]);
  }


  for (size_t i = 0; i < 10; i++) {
    labr3hscaled[i]->Smooth();
    for (Int_t t = 0; t < numofh; t++) {
      labr3hc[i][t]->Smooth();
      labr3hcpre[i][t]->Smooth();
    }
  }

  // for (size_t i = 0; i < 10; i++) {
  //   for (Int_t t = 0; t < numofh; t++) { // drawing the histos
  //     TCanvas * c1 = new TCanvas("c", "c", 1280, 720);
  //     TH1F *labr3hco6tco = dynamic_cast<TH1F*>(labr3h[i][t]->Rebin(4,"labr3hco6tco"));
  //     TH1F *back = dynamic_cast<TH1F*>(labr3hco6tco->ShowBackground());
  //     labr3hco6tco->Add(back,-1);
  //     labr3hco6tco->ShowPeaks(1);
  //     //labr3hco6tco->GetYaxis()->SetRangeUser(0,3500);
  //     labr3hco6tco->Draw();
  //     std::ostringstream pics;
  //     pics<<"pics75alld/hist_d"<<i<<"run"<<t<<".png";
  //     c1->SaveAs(pics.str().c_str());
  //     c1->Close();
  //   }
  // }

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
  Float_t pre_pre_pos=pre_pos+r3;
  pos = ((50.0/16)*(pre_pre_pos))-25.0;
  //cout << pos << endl;
  return pos;

}
Float_t p_alpha(Float_t Energy){
  Float_t pA = sqrt(2*(3.7273)*Energy);
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
Float_t thetaLaBr(Int_t detector){
  Float_t LO=-29.22;
  Float_t HI=29.22;
  Float_t theta;
  Int_t d = detector-64;
  if (d==0){theta = 270;}
  if(d==1) {theta = 0;}
  if(d>=2 && d<=6){theta =  (45*(d-1));}
  if(d==7){theta = 45;}
  if(d==8){theta = 135;}
  if(d==9){theta = 270;}
  float r3 = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
  return theta ;
}
Float_t phiLaBr(Int_t detector){
  Float_t LO=-29.22;
  Float_t HI=29.22;
  Float_t phi;
  Int_t d =  detector-64;
  if (d<=6) {phi = 0;}
  else {phi = -55;}
  float r3 = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
  return phi ;

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
    if (a>=40 && a<=47) {C_Chn=a  ;}   // Vertical, meaning back of detector

    if (a>=48 && a<=55) {C_Chn=a+8;}    // Horizontal, meaing front of detector
    if (a>=56 && a<=63) {C_Chn=111-a;}  // Horizontal, meaing front of detector
  }
  return C_Chn;
}

Float_t energyloss(Float_t Energy_i){
  Float_t Range_i=(0.119509)+
  (0.728771*pow(Energy_i,1))+
  (-0.157141*pow(Energy_i,2))+
  (0.111049*pow(Energy_i,3))+
  (-0.0287723*pow(Energy_i,4))+
  (0.00455411*pow(Energy_i,5))+
  (-0.000446413*pow(Energy_i,6))+
  (2.64015e-05*pow(Energy_i,7))+
  (-8.62449e-07*pow(Energy_i,8))+
  (1.19443e-08*pow(Energy_i,9));
  Float_t Range_f = Range_i + 3.45;
  Float_t Energy_f= (-0.273371)+
  (1.85138*pow(Range_f,1))+
  (-0.278392*pow(Range_f,2))+
  (0.039434*pow(Range_f,3))+
  (-0.0037989*pow(Range_f,4))+
  (0.000239383*pow(Range_f,5))+
  (-9.65087e-06*pow(Range_f,6))+
  (2.3809e-07*pow(Range_f,7))+
  (-3.24599e-09*pow(Range_f,8))+
  (1.85241e-11*pow(Range_f,9));
  return Energy_f;
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



TFile *_file0 = TFile::Open("tchain.root")
alphaenergyl->Draw()
TH1 *back = alphaenergyl->ShowBackground()
TH1 *alphaenergylco = alphaenergyl->Clone();
alphaenergylco->Add(back,-1)
alphaenergylco->GetYaxis()->SetRangeUser(0,50)
alphaenergylco->Draw()
*/
