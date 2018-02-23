/***
 *    ________ ______  __________________ _______
 *    ___  __ \___  / / /__  ___/___  __ \___    |
 *    __  /_/ /__  /_/ / _____ \ __  /_/ /__  /| |
 *    _  ____/ _  __  /  ____/ / _  _, _/ _  ___ |
 *    /_/      /_/ /_/   /____/  /_/ |_|  /_/  |_|
 *
 */
/*
to use this script:
Requisites: root >5.32

to get root in ubuntu:
$ sudo apt-get install root-system

type in terminal, -l option is to NOT show the root image
$ root -l
root[0] .x M2R.C
when asked, input the MIDAS file you wish to convert
*/

#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

#include "TFile.h"      //Write root files
#include "TTree.h"      // Make trees
#include "TObject.h"    // Write objects

//Colours for format and visuals
#define RESET   "\033[0m"
#define BLUE    "\033[34m"	/* Blue */
#define WHITE   "\033[37m"	/* White */
#define GREEN   "\033[32m"	/* Green */
#define YELLOW  "\033[33m"	/* Yellow */
#define RED     "\033[31m"	/* Red */

void M2R() {

cout<<BLUE<<"  _______  _     _  _______  ______   _   ______  ______  _______ "<<"\n";
cout<<BLUE<<" (_______)(_)   (_)(_______)(_____ \\ | | / _____)/ _____)(_______)"<<"\n";
cout<<BLUE<<"  _        _______  _______  _____) )| |( (____ ( (____   _______ "<<"\n";
cout<<BLUE<<" | |      |  ___  ||  ___  ||  __  / | | \\____ \\ \\____ \\ |  ___  |"<<"\n";
cout<<BLUE<<" | |_____ | |   | || |   | || |  \\ \\ | | _____) )_____) )| |   | |"<<"\n";
cout<<BLUE<<"  \\______)|_|   |_||_|   |_||_|   |_||_|(______/(______/ |_|   |_|"<<"\n"<<RESET;


  TDatime now; // date and time
  now.Print(); // print date and time
  string mystring;
  cout <<GREEN<< "Enter the name of the MIDAS file you want to convert to root" << "\n" << RESET;
  cin >> mystring;

  TStopwatch StopWatch; //stopwatch to keep on track of efficiency
  StopWatch.Start(); // start of the stopwatch

  std::ifstream is (mystring.c_str(), std::ifstream::binary); //MIDAS binary file source

  // This string are just to name the new root file the same way as the MIDAS file.
  string rootext(".root"); //
  string RF = mystring +rootext;

if (is) { // this if is just to check if the file exists.
  TFile f(RF.c_str(),"UPDATE"); //ROOT file, RF.c_str() makes the new file to have the ORIGNAL.root as name
  TTree *data = new TTree(mystring.c_str():,"Tree data"); //Tree to store the Data in a tree with the same name as the original run
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
  Float_t Z1[200]; // 200 is more than enough to allocate, you can add more if required.
  memset(Z1, 0, sizeof(Z1)); // Set Z1 to zero
  //Branches to store the data
  data->Branch("M_part", &Mult, "Mult/I"); // This makes variable lenght branch, Mult comes from multiplicity
  data->Branch("Z_event", Z1, "Z1[Mult]/F");
  // Histograms
  TH1F *h1 = new TH1F("Hits", "Number of detectos involved in each event", 200, 0, 200); //Distribution of #of active detectos in each event
  // get length of file:
  is.seekg (0, is.end); // go to the end of file
  int data_length = is.tellg(); // how long is the file in Bytes
  is.seekg (0, is.beg); // position to the start of file
  cout <<RED<< "Reading " << data_length << " characters... \n"<<RESET;

    ////////////////////////////
    //Reading the actual data///
    ////////////////////////////

    while (is) {  // while the data file is not at EOF

        //read 4 bytes at the time
        is.read(buffer,4);
        for (size_t i = 0; i < 4; i++) {pre_event[i]=(unsignedchar)buffer[i];} // turn read chars into ints
          //check if Block, start of header
          if(pre_event[0]==69 && pre_event[1]==66){Bcount++;} //block counter
          //check if event
          if (pre_event[0]==255 &&  pre_event[1]==255){
            Ecount++; // event counter
            a=int(((pre_event[2]*256)+pre_event[3])/4)-1; // How many detectos were involved in the hit
            h1->Fill(a);
            // this loop reads all the channels and amplitudes involved in an event
            if (Ecount< 5) {printf("Event: %i Channels: %i \n",Ecount, a );}

            for (size_t k = 0; k < a; k++) {
                    blu=k; //dumie variable for the loop
                    is.read(buffer,4); // read the event
                    for (size_t r = 0; r < 4; r++) {pre_event[r]=(unsignedchar)buffer[r];} //Char to Int
                    adc_num = 32*(pre_event[1]-1)+pre_event[0]; // Get ADC num
                    ampl = (256*(pre_event[2]))+pre_event[3]; // Get Amplitude
                    //Event=Ecount; // number of Event
                    Z1[adc_num]=ampl;
                    }
          /*The way the data is stored(in order, from the chanel zero to
          the last channel with hit on it) allows to have the
          multiplicity of the events to be equal to the last ADC number*/
          Mult=adc_num;

          // How many detectors were involved in the experiment if the quantity is known this step is removable
          if (Mult>detectors) {detectors=Mult;}

          data->Fill(); // Data dumping into the Branch
          memset(Z1, 0, sizeof(Z1)); // reset array to zero
         }
    }
  is.close(); //close the file
  f.Write("",TObject::kOverwrite); // DO NOT forget to actually write the ROOT File
  cout <<YELLOW<< "Events" << Ecount << "\n";
  cout <<YELLOW<< "Blocks" << Bcount << "\n";
  cout <<YELLOW << "Channels on the experiment: " << detectors+1 << "\n"; //Plus one because index starts at 0
  StopWatch.Stop(); // Measure time for efficiency
  cout << GREEN << "Total Real Time: "<< StopWatch.RealTime() << "s" << endl;
}
else{cout<< "error opening the file\n";}
}
