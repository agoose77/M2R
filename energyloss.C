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
