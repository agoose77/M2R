#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include <TRandom.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <Riostream.h>

void render16(char *name="NOTHING", char *rootFile="render16.root")
{

 printf("The argument is %s\n", name);
 printf("The second argument is %s\n", rootFile);

TFile *f = new TFile(rootFile,"UPDATE");

TTree *render16 = new TTree("render16","Tree data");

render16->Write("render16",TObject::kOverwrite);


	Int_t d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15, d16;

	Int_t dect1,dect2,dect3,dect4,dect5,dect6,dect7,dect8,dect9,dect10,dect11,dect12,dect13,dect14,dect15,dect16;

	Double_t Adect1,Adect2,Adect3,Adect4,Adect5,Adect6,Adect7,Adect8,Adect9,Adect10,Adect11,Adect12,Adect13,Adect14,Adect15,Adect16,max;

	Double_t xarribatodos,yarribatodos, abajotodos;

  Double_t xt,yt;

	Int_t BIASmax, BIASmin;

	BIASmax=5000;

	BIASmin=50;

//Posiciones de los Detectores
  Double_t Adetvec[16];

  Double_t posdetxvec[16] = {79.35, 49.35, 34.35, 2.5, 0., 2.5, 34.35, 49.35, 79.35, 109.35, 124.35, 156.35, 158.7, 156.2, 124.35, 109.35};

  Double_t posdetyvec[16] = {0., 0., 0., 2.5, 35.0, 67.5, 70., 70., 70., 70., 70., 67.5, 35., 2.5, 0., 0.};


render16->Branch("ADetector1render16",&Adect1,"Adect1/D");
render16->Branch("ADetector2render16",&Adect2,"Adect2/D");
render16->Branch("ADetector3render16",&Adect3,"Adect3/D");
render16->Branch("ADetector4render16",&Adect4,"Adect4/D");
render16->Branch("ADetector5render16",&Adect5,"Adect5/D");
render16->Branch("ADetector6render16",&Adect6,"Adect6/D");
render16->Branch("ADetector7render16",&Adect7,"Adect7/D");
render16->Branch("ADetector8render16",&Adect8,"Adect8/D");
render16->Branch("ADetector9render16",&Adect9,"Adect9/D");
render16->Branch("ADetector10render16",&Adect10,"Adect10/D");
render16->Branch("ADetector11render16",&Adect11,"Adect11/D");
render16->Branch("ADetector12render16",&Adect12,"Adect12/D");
render16->Branch("ADetector13render16",&Adect13,"Adect13/D");
render16->Branch("ADetector14render16",&Adect14,"Adect14/D");
render16->Branch("ADetector15render16",&Adect15,"Adect15/D");
render16->Branch("ADetector16render16",&Adect16,"Adect16/D");


render16->Branch("xtrender16",&xt,"xt/D");
render16->Branch("ytrender16",&yt,"yt/D");

Int_t cuentasIFS;

render16->Branch("cuentasIFS",&cuentasIFS,"cuentasIFS/I");

FILE *out;
out=fopen(name,"r");
printf("opening %s\n",name);
int count = 0;
int count1 = 0;
int count2 = 0;
while(!feof(out))  {

	//fscanf(out,"%d %d",&d1ext, &d2ext);
	fscanf(out,"%d %d %d %d %d %d %d %d",&d1,&d2,&d3,&d4,&d5,&d6,&d7,&d8);
	fscanf(out,"%d %d %d %d %d %d %d %d",&d9,&d10,&d11,&d12,&d13,&d14,&d15,&d16);
	count+=1;
	count1=0;

  Adetvec[0]=d1;
  Adetvec[1]=d2;
  Adetvec[2]=d3;
  Adetvec[3]=d4;
  Adetvec[4]=d5;
  Adetvec[5]=d6;
  Adetvec[6]=d7;
  Adetvec[7]=d8;
  Adetvec[8]=d9;
  Adetvec[9]=d10;
  Adetvec[10]=d11;
  Adetvec[11]=d12;
  Adetvec[12]=d13;
  Adetvec[13]=d14;
  Adetvec[14]=d15;
  Adetvec[15]=d16;

// MONDE detectors

	for (int i=0; i<16; i++){
            Adetvec[i]=0;
        }

		dect1=0;
		Adect1=0;
	if (d1<BIASmax && d1>BIASmin) {


		count1+=1;
		dect1=d1;
		Adect1=(1.0*dect1)+0;
		Adetvec[0] = Adect1;


	}

	  dect2=0;
		Adect2=0;
	if (d2<BIASmax && d2>BIASmin) {

		count1+=1;
		dect2=d2;
		Adect2=(1.0*dect2)+0;
		Adetvec[1] = Adect2;

	}
		dect3=0;
		Adect3=0;
	if (d3<BIASmax && d3>BIASmin) {

		count1+=1;
		dect3=d3;
		Adect3=(1.0*dect3)+0;
		Adetvec[2] = Adect3;

	}

		dect4=0;
		Adect4=0;
	if (d4<BIASmax && d4>BIASmin) {

		count1+=1;
		dect4=d4;
		Adect4=(1.0*dect4)-0;
		Adetvec[3] = Adect4;



	}
		dect5=0;
		Adect5=0;
	if (d5<BIASmax && d5>BIASmin) {

		count1+=1;
		dect5=d5;
		Adect5=(1.0*dect5)+0;
		Adetvec[4] = Adect5;



	}
		dect6=0;
		Adect6=0;
	if (d6<BIASmax && d6>BIASmin) {

		count1+=1;
		dect6=d6;
		Adect6=(1.0*dect6)-0;
		Adetvec[5] = Adect6;


	}
		dect7=0;
		Adect7=0;
	if (d7<BIASmax && d7>BIASmin) {
		count1+=1;
		dect7=d7;
		Adect7=(1.0*dect7)+0;
		Adetvec[6] = Adect7;

	}
		dect8=0;
		Adect8=0;
	if (d8<BIASmax && d8>BIASmin) {
		count1+=1;
		dect8=d8;
		Adect8=(1.0*dect8)+0;
		Adetvec[7] = Adect8;

	}

		dect9=0;
		Adect9=0;
	if (d9<BIASmax && d9>BIASmin) {
		count1+=1;
		dect9=d9;
		Adect9=(1.0*dect9)+0;
		Adetvec[8] = Adect9;



	}
		dect10=0;
		Adect10=0;
	if (d10<BIASmax && d10>BIASmin) {
		count1+=1;
		dect10=d10;
		Adect10=(1.0*dect10)+0;
		Adetvec[9] = Adect10;



	}
		dect11=0;
		Adect11=0;
	if (d11<BIASmax && d11>BIASmin) {
		count1+=1;
		dect11=d11;
		Adect11=(1.0*dect11)+0;
		Adetvec[10] = Adect11;


	}
		dect12=0;
		Adect12=0;
	if (d12<BIASmax && d12>BIASmin) {
		count1+=1;
		dect12=d12;
		Adect12=(1.0*dect12)+0;
		Adetvec[11] = Adect12;



	}
		dect13=0;
		Adect13=0;
	if (d13<BIASmax && d13>BIASmin) {
		count1+=1;
		dect13=d13;
		Adect13=(1.0*dect13)+0;
		Adetvec[12] = Adect13;
	}
		dect14=0;
		Adect14=0;
	if (d14<BIASmax && d14>BIASmin) {
		count1+=1;
		dect14=d14;
		Adect14=(1.0*dect14)+0;
		Adetvec[13] = Adect14;

	}
		dect15=0;
		Adect15=0;
	if (d15<BIASmax && d15>BIASmin) {
		count1+=1;
		dect15=d15;
		Adect15=(1.0*dect15)+0;
		Adetvec[14] = Adect15;


	}
		dect16=0;
		Adect16=0;

	if (d16<BIASmax && d16>BIASmin) {
		count1+=1;
		dect16=d16;
		Adect16=(1.0*dect16)+0;
		Adetvec[15] = Adect16;
	}


	cuentasIFS=count1;

	xarribatodos=0;
	yarribatodos=0;
	abajotodos=0;
	xt=0;
	yt=0;


// Anger Position


if(cuentasIFS>=2){

	for (int i=0; i<16; i++){

				xarribatodos += posdetxvec[i]*Adetvec[i];
				yarribatodos += posdetyvec[i]*Adetvec[i];
				abajotodos += Adetvec[i];

			}

			xt= (xarribatodos/abajotodos);
			yt= (yarribatodos/abajotodos);

	}

render16->Fill();




}

cout << "\nThe Number of Events is: " << count <<endl;

fclose(out);

f->Write("",TObject::kOverwrite);


}
