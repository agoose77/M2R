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

type in terminal, -l option is to NOT show the root image at start
$ root -l
root[0] .x M2RV2.C++ // this compiles the for improved results
when asked, input the MIDAS file you wish to convert

To load the root file you just create,type.
The data is stored in this way, making it very fast to read, it is very similar to MIDAS output, and it keeps
the file size small.
$ root  yourfile.root
root [1] data->Show(0)
======> EVENT:0
 emult           = 3
 time            = 1
 eadc            = 43,
                  63, 80
 eampl           = 326,
                  379, 398
root [2] data->Show(1)
======> EVENT:1
 emult           = 2
 time            = 2
 eadc            = 42,
                  57
 eampl           = 865,
                  912
root [3] data->Show(5)
======> EVENT:5
 emult           = 1
 time            = 6
 eadc            = 66
 eampl           = 71



*/


#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

#include "TFile.h"      //Write root files
#include "TTree.h"      // Make trees
#include "TObject.h"    // Write objects
#include <TH1.h>        //  1D histograms
#include <TStopwatch.h>  // stopwatch for time
#include <sstream>      // for ostringstream

//Colours for format and visuals
#define RESET   "\033[0m"
#define BLUE    "\033[34m"	/* Blue */
#define WHITE   "\033[37m"	/* White */
#define GREEN   "\033[32m"	/* Green */
#define YELLOW  "\033[33m"	/* Yellow */
#define RED     "\033[31m"	/* Red */
Int_t imap(Int_t); // change the maping of thin detectors

void M2RV2() {

//cout<<BLUE<<" (_______)(_)   (_)(_______)(_____ \\ | | / _____)/ _____)(_______)"<<"\n";
//cout<<BLUE<<"  _        _______  _______  _____) )| |( (____ ( (____   _______ "<<"\n";
//cout<<BLUE<<" | |      |  ___  ||  ___  ||  __  / | | \\____ \\ \\____ \\ |  ___  |"<<"\n";
//cout<<BLUE<<" | |_____ | |   | || |   | || |  \\ \\ | | _____) )_____) )| |   | |"<<"\n";
//cout<<BLUE<<"  \\______)|_|   |_||_|   |_||_|   |_||_|(______/(______/ |_|   |_|"<<"\n"<<RESET;

cout<<"                    "<<BLUE<<"   _______  ______   ______  "<<"\n";
cout<<"                    "<<BLUE<<"  (_______)(_____ \\ (_____ \\ "<<"\n";
cout<<"                    "<<BLUE<<"   _  _  _   ____) ) _____) )"<<"\n";
cout<<"                    "<<BLUE<<"  | ||_|| | / ____/ |  __  / "<<"\n";
cout<<"                    "<<BLUE<<"  | |   | || (_____ | |  \\ \\ "<<"\n";
cout<<"                    "<<BLUE<<"  |_|   |_||_______)|_|   |_|"<<"\n"<<RESET;



  TDatime now; // date and time
  now.Print(); // print date and time
  string mystring; //string to store the name of the file
  cout <<GREEN<< "Enter the name of the MIDAS file you want to convert to root" << "\n" << RESET;
  cin >> mystring; // user input file name
  //mystring = "R175_0";
  cout << GREEN <<"Now analysing: " << mystring << RESET<< endl;
  TStopwatch StopWatch; //stopwatch to keep on track of efficiency
  StopWatch.Start(); // start of the stopwatch

  std::ifstream is (mystring.c_str(), std::ifstream::binary); //MIDAS binary file source

  // These strings are just to name the new root file the same way as the MIDAS file.
  string rootext(".root");
  string RF = mystring+rootext;

if (is) { // this if is just to check if the file exists.
  TFile f(RF.c_str(),"RECREATE"); //ROOT file, RF.c_str() makes the new file to have the ORIGNAL.root as name
  TTree *data = new TTree("data","Tree data"); //Tree to store the Data in a tree with the same name as the original run
  // data collectors variables
  char buffer[4]; //the file will be read to here in charactes safe number is 3
  Int_t pre_event [4]; // make them integers

  //Event Count, Block Count, auxiliar variables
  Int_t Ecount=0;
  Int_t Bcount=0;
  Int_t a;
  //Data variables
  Int_t adc,bin_slot,adc_low,adc_high;
  Int_t ampl,Event;
  Int_t adc_num=0;
  Int_t detectors=0;
  Int_t hitslabr3,hitsdssd;
  // Multiplicity
  Int_t Mult=0;
  Int_t emult=0;
  ///NUMBER OT DETECTORS TO ALLOCATE
  Int_t eadc[200];
  Int_t eampl[200];
  //Branches to store the data
  data->Branch("emult", &emult, "emult/I");  // Multiplicity of the event
  data->Branch("eadc", eadc, "eadc[emult]/I"); // Branch with the adc number
  data->Branch("eampl",eampl,"eampl[emult]/I"); // amplitude number
  data->Branch("hitsdssd", &hitsdssd, "hitsdssd/I");  // Multiplicity of the event
  data->Branch("hitslabr3", &hitslabr3, "hitslabr3/I");  // Multiplicity of the event

  // Histograms
  TH1I *h1 = new TH1I("Hits", "Number of detectos involved in each event", 100, 0, 100); //Distribution of #of active detectos in each event
  TH1I *hadc[200];
  for(int w=0;w<200;w++) {
		std::ostringstream name;
		name<<"hadc"<<w;
    name<< mystring;
		hadc[w] = new TH1I(name.str().c_str(),name.str().c_str(),5000,0,5000);
	}

  // get length of file:
  is.seekg (0, is.end); // go to the end of file
  int data_length = is.tellg(); // how long is the file in Bytes
  is.seekg (0, is.beg); // position to the start of file
  cout <<BLUE<< "Reading " << data_length << " characters... \n"<<RESET;

                          /////////////////////
                          //Reading the  data//
                          /////////////////////
    while (is) {  // while the data file is not at EOF
        //read 4 bytes at the time
        is.read(buffer,4);
        for (Int_t i = 0; i <= 3; i++) {pre_event[i]=(unsigned char)buffer[i];} // turn read chars into ints
          //check if Block, start of header
          if(pre_event[0]==69 && pre_event[1]==66){Bcount++;} //block counter
          //check if event
          if (pre_event[0]==255 &&  pre_event[1]==255){
            hitsdssd=0;
            hitslabr3=0;
            Mult=0;
            Ecount++; // event counter
            a=int(((pre_event[2]*256)+pre_event[3])/4)-1; // How many detectos were involved in the hit
            h1->Fill(a);
            emult=a;
            // this loop reads all the channels and amplitudes involved in an event
            if (Ecount< 5) {
              printf("Event: %i Channels: %i \n",Ecount, a );
            }
            for (Int_t k = 0; k < a; k++) {
                      is.read(buffer,4); // read the event
                      for (int r = 0; r <= 3; r++) {pre_event[r]=(unsigned char)buffer[r];}// turn read chars into ints
                      adc_num = int(32*(pre_event[1]-1)+pre_event[0]); // Get ADC num
                      ampl = int((256*(pre_event[2]))+pre_event[3]); // Get Amplitude
                      eadc[k]=imap(adc_num); // saves the adc number in each hit in the event
                      eampl[k]=ampl; // saves the ampl number in each hit in the event
                      hadc[imap(adc_num)]->Fill(ampl); // hitograms for quick view
                      if (adc_num<=63) {hitsdssd++;} // only for this experiment 2015
                      if (adc_num>63 && adc_num<=73) {hitslabr3++;}
            }
                      if (Ecount< 3) {
                        printf("Multiplicity is: %i\n",emult );
                        for (Int_t y = 0; y < emult; y++) {
                          printf("adc: %i ampl: %i\n",eadc[y],eampl[y]);
                        }
                      }
            Mult=adc_num+1; // Just to know the number of the detectors involved
            if (Mult>detectors) {detectors=Mult;}
            data->Fill(); // Data dumping into the Branch
         }
    }
  is.close(); //close the file
  ///smooth Histograms
 for(int ii=0;ii<detectors;ii++) {
    hadc[ii]->Smooth();
  }
  ///Delete unused Histograms
  for(int ii=detectors;ii<200;ii++) {delete hadc[ii];}

  f.Write("",TObject::kOverwrite); // DO NOT forget to actually write the ROOT File
  cout <<YELLOW<< "Events: " << Ecount << "\n";
  cout <<YELLOW<< "Blocks: " << Bcount << "\n";
  cout <<YELLOW << "Channels on the experiment: " << detectors << "\n"; //Plus one because index starts at 0
  StopWatch.Stop(); // Measure time for efficiency
  cout << GREEN << "Total Real Time: "<< StopWatch.RealTime() << "s" << endl;
}
else{cout<< "error opening the file\n";}
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
    if (a>=32 && a<=39) {C_Chn=a+8;}    // Vertical, meaning back of detector
    if (a>=40 && a<=47) {C_Chn=79-a;}   // Vertical, meaning back of detector
    if (a>=48 && a<=55) {C_Chn=a+8;}    // Horizontal, meaing front of detector
    if (a>=56 && a<=63) {C_Chn=111-a;}  // Horizontal, meaing front of detector
  }
  return C_Chn;
}
