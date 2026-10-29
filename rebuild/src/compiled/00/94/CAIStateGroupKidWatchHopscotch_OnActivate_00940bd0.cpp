#include "rebuild_abi.h"
extern "C" void FABLE_FASTCALL Act_940bd0_A();
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
struct C { char p[0x30]; int StartFrame; char p2[0x3c-0x34]; int ActivationCount; void OnActivate(); };
void C::OnActivate(){ this->ActivationCount++; Act_940bd0_A(); this->StartFrame = CWorld::GetFrame(); }