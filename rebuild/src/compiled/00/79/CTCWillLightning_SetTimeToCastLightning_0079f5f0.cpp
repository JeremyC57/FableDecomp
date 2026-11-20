// CTCWillLightning::SetTimeToCastLightning @ 0x0079f5f0
// void __fastcall (CTCWillLightning*, float)
//   [ecx+0x5b] = 1;
//   [ecx+0x1c] = (int)( (float)g_int * param );

// NF166 (round 10, q11679 / q11691 / q11725): 0x01375550 is ?ConstantFPS@CMainGameComponent@@1JA (protected static long,
//   engine/CMainGameComponent.h), read through the inline public accessor GetConstantFPS() (Ego_r + FableWin
//   ?GetConstantFPS@CMainGameComponent@@SIJXZ) as retail code outside the class does; was the row-local stand-in g_lightningCastTicks.
#include "engine/CMainGameComponent.h"

class CTCWillLightning { public:
    char pad00[0x1c];
    int   m_timeToCast;   // +0x1c
    char pad20[0x5b - 0x20];
    char m_castFlag;      // +0x5b

    void SetTimeToCastLightning(float scale);
};

void CTCWillLightning::SetTimeToCastLightning(float scale)
{
    this->m_castFlag = 1;
    this->m_timeToCast = (int)((float)CMainGameComponent::GetConstantFPS() * scale);
}