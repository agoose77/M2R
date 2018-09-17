#include "TFile.h"      //Write root files
#include "TTree.h"      // Make trees
#include "TObject.h"    // Write objects
#include "TCanvas.h"    // TCanvas
#include "TH1.h"        //  1D histograms
#include "TH2.h"        //  2D histograms
#include "TStopwatch.h"  // stopwatch for time
#include "TChain.h"     // Tchain to process several root files at the time
#include "TGraph.h"     // TGraph include
#include <sstream>      // for ostringstream
#include <stdlib.h>     // srand, rand
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <math.h>       // sqrt

void energy_loss(){
  FILE *out_eloss; // file declaration
  Int_t fread_dump_energyloss,elos_i=0; // dump fscan result, vaiable for the array with the enegy loss
  Float_t eloss_E[1501];
  Float_t eloss_R[1501];
  Float_t d2=1,d3=0; // clean variables
  out_eloss=fopen("elossbueno.rft","r"); // opens the calib file with the name typed
    if (out_eloss!=NULL) { // only works for actual files
      while(!feof(out_eloss))  { // reads until is over
        fread_dump_energyloss=fscanf(out_eloss,"%f %f",&d2,&d3);
        eloss_E[elos_i]=d2; // multiply
        eloss_R[elos_i]=d3; // add
        //cout << elos_i << "\t" << eloss_E[elos_i] << "\t" << eloss_R[elos_i] << endl;
        elos_i++;
      }
    } else {
        cout << "Eloss file not found" << endl;
    }
    TGraph* EvsR = new TGraph(elos_i,eloss_E,eloss_R);
    TGraph* RvsE = new TGraph(elos_i,eloss_R,eloss_E);
    //EvsR->Draw("AC*");
    //EvsR->SetTitle("EvsR");
    RvsE->Draw("AC");
    RvsE->SetTitle("RvsE");
}// end of program
/*Minimizer is Linear parameters for the pol9 for the energy loss
Chi2                      =   0.00017463
NDf                       =          942
p0                        =      0.16721   +/-   0.00114064
p1                        =     0.597041   +/-   0.00389508
p2                        =   -0.0371422   +/-   0.0051133
p3                        =    0.0609571   +/-   0.00346335
p4                        =    -0.018266   +/-   0.00136089
p5                        =   0.00355555   +/-   0.000327089
p6                        = -0.000447084   +/-   4.87327e-05
p7                        =  3.49208e-05   +/-   4.38673e-06
p8                        = -1.53884e-06   +/-   2.18361e-07
p9                        =  2.92149e-08   +/-   4.61182e-09

*/
