#include "rebuild_abi.h"
#include <cstdio>
extern "C" void FABLE_FASTCALL Act_940bd0_A(){}
static int bv=0; extern "C" int FABLE_FASTCALL Act_940bd0_B(){ return bv; }
struct C { char p[0x30]; int StartFrame; char p2[0x3c-0x34]; int ActivationCount; void OnActivate(); };
void C::OnActivate(){ this->ActivationCount++; Act_940bd0_A(); this->StartFrame = Act_940bd0_B(); }
int main(){ C o; o.ActivationCount=1; o.StartFrame=0; bv=7; o.OnActivate(); if(o.ActivationCount!=2||o.StartFrame!=7){std::printf("B1");return 1;} std::printf("RB_940bd0_OK\n"); return 0; }