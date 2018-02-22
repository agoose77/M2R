
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

#include "TFile.h"
#include "TTree.h"
#include "TObject.h"


void M2R() {

  TDatime now; // date and time
  now.Print(); // print date and time

  TStopwatch StopWatch; //stopwatch to keep on track of efficiency
  StopWatch.Start(); // start of the stopwatch

  std::ifstream is ("R177_0", std::ifstream::binary); //MIDAS binary file source

if (is) { // this if is just to check if the file exists.
  TFile f("data.root","UPDATE"); //ROOT file
  TTree *data = new TTree("data","Tree data"); //Tree to store the Data
  data->Write("data.root",TObject::kOverwrite); // Making the root file

  // data collectors variables
  char buffer[3]; //the file will be read to here in charactes
  Int_t pre_event [4]; // make them integers

  //Event Count, Block Count, auxiliar variables
  Int_t Ecount=0;
  Int_t Bcount=0;
  Int_t h=0;
  Int_t a=0;
  Int_t blu=0;
  //Data variables
  Int_t adc,bin_slot,adc_low,adc_high;
  Int_t ampl,adc_num,Event,detectors;


  // Multiplicity
  Int_t Mult;
  ///NUMBER OT DETECTORS TO ALLOCATE
  Float_t Z1[200];
  memset(Z1, 0, sizeof(Z1));
  //Branches to store the data
  data->Branch("M_part", &Mult, "Mult/I");
  data->Branch("Z_event", Z1, "Z1[Mult]/F");
  // Histograms
  TH1F *h1 = new TH1F("Hits", "Number of detectos involved in each event", 200, 0, 200);
  // get length of file:
  is.seekg (0, is.end); // go to the end of file
  int data_length = is.tellg(); // how long is the file in Bytes
  is.seekg (0, is.beg); // position to the start of file
  cout << "Reading " << data_length << " characters... \n";

    ////////////////////////////
    //Reading the actual data///
    ////////////////////////////
    while (is) {  // while the data file is readable

        //read 4 bytes at the time
        is.read(buffer,4);
        for (size_t i = 0; i < 4; i++) {pre_event[i]=(unsignedchar)buffer[i];} // turn them into ints
          //check if Block, start of header
          if(pre_event[0]==69 && pre_event[1]==66){Bcount++;} //block counter
          //check if event
          if (pre_event[0]==255 &&  pre_event[1]==255){
            Ecount++; // event counter
            a=int(((pre_event[2]*256)+pre_event[3])/4)-1; // How many detectos were involved in the hit
            h1->Fill(a);
            // this loop reads all the channels and amplitudes involved in an event
            if (Ecount< 10) {
              printf("Event: %i Channels: %i \n",Ecount, a );
            }
            for (size_t k = 0; k < a; k++) {
                    blu=k; //dumie variable for the loop
                    is.read(buffer,4); // read the event
                    for (size_t r = 0; r < 4; r++) {pre_event[r]=(unsignedchar)buffer[r];} //Char to Int
                    adc_num = 32*(pre_event[1]-1)+pre_event[0]; // Get ADC num
                    ampl = (256*(pre_event[2]))+pre_event[3]; // Get Amplitude
                    //Event=Ecount; // number of Event
                    Z1[adc_num]=ampl;
                    }
          // The way the data is stored(in order, from the chanel zero to //
          // the last channel with hit on it) allows to have the ///////////
          //multiplicity of the events to be equal to the last ADC number///
          Mult=adc_num;
          if (Mult>detectors) {detectors=Mult;} // How to know how many detectors were involved in the experiment if the quantity is know this step is removable

        //Just to check Everything is going well
        //  if (Ecount<10) {
        //    printf("Event: %i\n",Ecount );
        //   for (size_t i = 0; i < Mult; i++){if (Z1[i]!=0) {printf("Channel: %i Amplitude: %i\n",i, Z1[i]);}}
        //    }
          data->Fill(); // Data filling
          memset(Z1, 0, sizeof(Z1)); // reset array to zero
         }
    }
  is.close(); //close the file
  f.Write("",TObject::kOverwrite); // DO NOT forget to actually write the ROOT File
  printf("Events %i\n",Ecount );
  printf("Blocks %i\n",Bcount );
  printf("Channels on the experiment: %i \n", detectors );
  StopWatch.Stop(); // Measure efficiency
  TDatime now1; // time and date
  now1.Print(); // time and date
  cout << "Total Real Time: "<< StopWatch.RealTime() << "s" << endl;
}
else{cout<< "error opening the file\n";}
}
