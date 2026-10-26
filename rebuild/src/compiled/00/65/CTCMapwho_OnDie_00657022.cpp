#pragma optimize("s",on)
#include "rebuild_abi.h"
// CTCMapwho::OnDie @ 0x00657022
class CTCMapwho;
extern "C" void FABLE_FASTCALL FableH1_657022(CTCMapwho *self);
class CTCMapwho { public: char dummy; void OnDie(); };
void CTCMapwho::OnDie(){
    if (this) {
        FableH1_657022(this);
        operator delete(this);
    }
}