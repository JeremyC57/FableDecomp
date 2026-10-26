#pragma optimize("s",on)
#include "rebuild_abi.h"
// CTCMapwho::OnDie @ 0x005c281c
class CTCMapwho;
extern "C" void FABLE_FASTCALL FableH1_5c281c_0065688f(CTCMapwho *self);
class CTCMapwho { public: char dummy; void OnDie(); };
void CTCMapwho::OnDie(){
    if (!this) return;
    FableH1_5c281c_0065688f(this);
    operator delete(this);
}