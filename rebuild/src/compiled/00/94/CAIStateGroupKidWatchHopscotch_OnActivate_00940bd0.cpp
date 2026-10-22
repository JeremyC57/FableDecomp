#include "rebuild_abi.h"
extern "C" void FABLE_FASTCALL Act_940bd0_A();
extern "C" int FABLE_FASTCALL Act_940bd0_B();
struct C { char p[0x30]; int StartFrame; char p2[0x3c-0x34]; int ActivationCount; void OnActivate(); };
void C::OnActivate(){ this->ActivationCount++; Act_940bd0_A(); this->StartFrame = Act_940bd0_B(); }