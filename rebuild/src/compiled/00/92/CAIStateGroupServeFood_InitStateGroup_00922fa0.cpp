#include "rebuild_abi.h"
struct C {
    char a[0x14]; int CarriedFoodItems; char b[0x1c-0x18]; int Seat; char c[0x44-0x20]; int ServeActionPoint;
    void InitStateGroup(int p1, int p2);
};
void C::InitStateGroup(int p1, int p2){ this->CarriedFoodItems=0; this->Seat=-1; this->ServeActionPoint=-1; }