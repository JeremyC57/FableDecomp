#pragma optimize("s",on)
#include "rebuild_abi.h"
// CTCMapwho::OnDie @ 0x005c281c
struct CTCMapwho;
extern "C" void FABLE_FASTCALL FableH1_5c281c_0065688f(CTCMapwho *self);
extern "C" void FABLE_CDECL FableH2_5c281c(CTCMapwho *self);
struct CTCMapwho { char dummy; void OnDie(); };
void CTCMapwho::OnDie(){
    if (!this) return;
    FableH1_5c281c_0065688f(this);
    FableH2_5c281c(this);
}