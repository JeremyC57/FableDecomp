#include "rebuild_abi.h"
#include <cstdio>
struct C {
    char a[0x14]; int CarriedFoodItems; char b[0x1c-0x18]; int Seat; char c[0x44-0x20]; int ServeActionPoint;
    void InitStateGroup(int world, int creature);
};
void C::InitStateGroup(int world, int creature){ this->CarriedFoodItems=0; this->Seat=-1; this->ServeActionPoint=-1; }
int main(){ C o; o.InitStateGroup(1,2);
 if(o.CarriedFoodItems!=0||o.Seat!=-1||o.ServeActionPoint!=-1){std::printf("B1");return 1;}
 std::printf("RB_922fa0_OK\n"); return 0; }