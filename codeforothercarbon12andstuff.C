// Float_t alpha1 = p_alpha((xle[0]+yle[0])/2);
// Float_t alpha1_comp[3]={
//   alpha1*sin(xl[0])*cos(yl[0]),
//   alpha1*sin(yl[0]),
//   alpha1*cos(xl[0])*cos(yl[0])};
//   Float_t Pc_sqrt=(alpha1_comp[0]*alpha1_comp[0])+
//                   (alpha1_comp[1]*alpha1_comp[1])+
//                   ((alpha1_comp[2]-Pb)*(alpha1_comp[2]-Pb));
//   Float_t Ec=Pc_sqrt/(2*Mc);
//   Float_t ExC12 = Eb + Qv - Ec - ((xle[0]+yle[0])/2);
//   Ex_C12_scatleft->Fill(ExC12);

    // TMath::Sort(int(hitondr/2.0),xre,s_rxe,1);
    // TMath::Sort(int(hitondr/2.0),yre,s_rye,1);

// for (int f = 0; f < int(hitondr/2.0); f++) {
//   if (abs(s_rxe[f]-s_rye[f])<Ethreshold) {
//     mapright->Fill(xr[s_rxe[f]],yr[s_rye[f]]);
//   }
// }


//// single hit on right
// if (xrc==1 && yrc==1 && abs(xre[0]-yre[0])<Ethreshold && labrP>0 && xlc==0 && ylc==0 && ((xre[0]+yre[0])/2)>0.8) { // one hit on detector 1
//   alphaenergyr->Fill((xre[0]+yre[0])/2);
//   maprightsingle->Fill(xr[s_rxe[0]],yr[s_rye[0]]);
//
//   Float_t alpha1 = p_alpha((xre[0]+yre[0])/2);
//   Float_t alpha1_comp[3]={
//     alpha1*sin(xr[0])*cos(yr[0]),
//     alpha1*sin(yr[0]),
//     alpha1*cos(xr[0])*cos(yr[0])};
//     Float_t Pc_sqrt=(alpha1_comp[0]*alpha1_comp[0])+
//                     (alpha1_comp[1]*alpha1_comp[1])+
//                     ((alpha1_comp[2]-Pb)*(alpha1_comp[2]-Pb));
//     Float_t Ec=Pc_sqrt/(2*Mc);
//     Float_t ExC12 = Eb + Qv - Ec - ((xre[0]+yre[0])/2);
//     Ex_C12_scatright->Fill(ExC12);
//   }
    ////////
// //one hit in each detector
// if (xlc==1 && ylc==1 && abs(xle[0]-yle[0])<Ethreshold &&
//     xrc==1 && yrc==1 && abs(xre[0]-yre[0])<Ethreshold && labrP>0) {
//   Float_t alphal = p_alpha((xle[0]+yle[0])/2);
//   Float_t alphal_comp[3]={
//     alphal*sin(xl[0])*cos(yl[0]),
//     alphal*sin(yl[0]),
//     alphal*cos(xl[0])*cos(yl[0])};
//     Float_t Pcl_sqrt=(alphal_comp[0]*alphal_comp[0])+
//     (alphal_comp[1]*alphal_comp[1])+
//     ((alphal_comp[2])*(alphal_comp[2]));
//   Float_t alphar = p_alpha((xre[0]+yre[0])/2);
//   Float_t alphar_comp[3]={
//     alphar*sin(xr[0])*cos(yr[0]),
//     alphar*sin(yr[0]),
//     alphar*cos(xr[0])*cos(yl[0])};
//     Float_t Pcr_sqrt=(alphar_comp[0]*alphar_comp[0])+
//     (alphar_comp[1]*alphar_comp[1])+
//     (alphar_comp[2]*alphar_comp[2]);
//   Float_t alpharecos[3]={0};
//   for (size_t v = 0; v < 3; v++) {alpharecos[v]= -alphar_comp[v]-alphal_comp[v];}
//   Float_t Psqrt_alpharecos=0;
//   for (size_t g = 0; g < 3; g++) { Psqrt_alpharecos=+pow(alpharecos[g],2);}
//   Float_t E_alpharecos=0;
//   E_alpharecos = Psqrt_alpharecos/(2*Ma);
//   Float_t Ec_recos= (Pcr_sqrt+Pcl_sqrt+Psqrt_alpharecos)/(2*Mc);
//   Float_t ExcplusQ = E_alpharecos+((xre[0]+yre[0])/2)+((xle[0]+yle[0])/2)-Ec_recos;
//   ExplusQ_C12r_hitrightleft->Fill(ExcplusQ);
// }


// //// Double hit in left detector, carbon reconstruction
// if (xlc==2 && ylc==2 && abs(xle[0]-yle[0])<Ethreshold &&
//     abs(xle[1]-yle[1])<Ethreshold && labrP>0 ) {
//   Float_t alphal0 = p_alpha((xle[0]+yle[0])/2);
//   Float_t alphal0_comp[3]={
//     alphal0*sin(xl[0])*cos(yl[0]),
//     alphal0*sin(yl[0]),
//     alphal0*cos(xl[0])*cos(yl[0])};
//     Float_t Pcl0_sqrt=(alphal0_comp[0]*alphal0_comp[0])+
//     (alphal0_comp[1]*alphal0_comp[1])+
//     ((alphal0_comp[2])*(alphal0_comp[2]));
//
//   Float_t alphal1 = p_alpha((xle[1]+yle[1])/2);
//   Float_t alphal1_comp[3]={
//     alphal1*sin(xl[1])*cos(yl[1]),
//     alphal1*sin(yl[1]),
//     alphal1*cos(xl[1])*cos(yl[1])};
//     Float_t Pcl1_sqrt=(alphal1_comp[0]*alphal1_comp[0])+
//     (alphal1_comp[1]*alphal1_comp[1])+
//     ((alphal1_comp[2])*(alphal1_comp[2]));
//
//   Float_t alpharecos[3]={0};
//   for (size_t v = 0; v < 3; v++) {alpharecos[v]= -alphal0_comp[v]-alphal1_comp[v];}
//   Float_t Psqrt_alpharecos=0;
//   for (size_t g = 0; g < 3; g++) { Psqrt_alpharecos=+pow(alpharecos[g],2);}
//   Float_t E_alpharecos=0;
//   E_alpharecos = Psqrt_alpharecos/(2*Ma);
//   Float_t Ec_recos= (Pcl0_sqrt+Pcl1_sqrt+Psqrt_alpharecos)/(2*Mc);
//   Float_t ExcplusQ = E_alpharecos+((xle[1]+yle[1])/2)+((xle[0]+yle[0])/2)-Ec_recos;
//   ExplusQ_C12r_doublehitleft->Fill(ExcplusQ);
// }
// ////
//
// /// Double hit on mapright
// if (xrc==2 && yrc==2 && abs(xre[0]-yre[0])<Ethreshold &&
//     abs(xre[1]-yre[1])<Ethreshold && labrP>0 ) {
//   //cout << "flag" << endl;
//   Float_t alphar0 = p_alpha((xre[0]+yre[0])/2);
//   Float_t alphar0_comp[3]={
//     alphar0*sin(xr[0])*cos(yr[0]),
//     alphar0*sin(yr[0]),
//     alphar0*cos(xr[0])*cos(yr[0])};
//     Float_t Pcr0_sqrt=(alphar0_comp[0]*alphar0_comp[0])+
//     (alphar0_comp[1]*alphar0_comp[1])+
//     ((alphar0_comp[2])*(alphar0_comp[2]));
//
//   Float_t alphar1 = p_alpha((xre[1]+yre[1])/2);
//   Float_t alphar1_comp[3]={
//     alphar1*sin(xr[1])*cos(yr[1]),
//     alphar1*sin(yr[1]),
//     alphar1*cos(xr[1])*cos(yr[1])};
//     Float_t Pcr1_sqrt=(alphar1_comp[0]*alphar1_comp[0])+
//     (alphar1_comp[1]*alphar1_comp[1])+
//     ((alphar1_comp[2])*(alphar1_comp[2]));
//
//   Float_t alpharecos[3]={0};
//   for (size_t v = 0; v < 3; v++) {alpharecos[v]= -alphar0_comp[v]-alphar1_comp[v];}
//   Float_t Psqrt_alpharecos=0;
//   for (size_t g = 0; g < 3; g++) { Psqrt_alpharecos=+pow(alpharecos[g],2);}
//   Float_t E_alpharecos=0;
//   E_alpharecos = Psqrt_alpharecos/(2*Ma);
//   Float_t Ec_recos= (Pcr0_sqrt+Pcr1_sqrt+Psqrt_alpharecos)/(2*Mc);
//   Float_t ExcplusQ = E_alpharecos+((xre[1]+yre[1])/2)+((xre[0]+yre[0])/2)-Ec_recos;
//   ExplusQ_C12r_doublehitright->Fill(ExcplusQ);
// }
// ///

// single hit on right
// if (xrc==1 && yrc==1 && abs(xre[0]-yre[0])<Ethreshold && labrP>0) { // one hit on detector 1
//   alphaenergyr->Fill((xre[0]+yre[0])/2);
//   Float_t alpha1 = p_alpha((xre[0]+yre[0])/2);
//   Float_t alpha1_comp[3]={
//     alpha1*sin(xr[0])*cos(yr[0]),
//     alpha1*sin(yr[0]),
//     alpha1*cos(xr[0])*cos(yr[0])};
//     Float_t Pc_sqrt=(alpha1_comp[0]*alpha1_comp[0])+
//                     (alpha1_comp[1]*alpha1_comp[1])+
//                     ((alpha1_comp[2]-Pb)*(alpha1_comp[2]-Pb));
//     Float_t Ec=Pc_sqrt/(2*Mc);
//     Float_t ExC12 = Eb + Qv - Ec - ((xre[0]+yre[0])/2);
//     Ex_C12_scatright->Fill(ExC12);
//   }
//     ////////
