// CTCTalk::GetCurrentSpeechDuration @ 006e2840
// fild [ecx+0x14] ; fidiv [g_divisor] ; ret
// => (float)member / (int global) -> fidiv keeps divisor as int

// NF166 (round 10, q11679 / q11691 / q11725): 0x01375550 is ?ConstantFPS@CMainGameComponent@@1JA (protected static long,
//   engine/CMainGameComponent.h), read through the inline public accessor GetConstantFPS() (Ego_r + FableWin
//   ?GetConstantFPS@CMainGameComponent@@SIJXZ) as retail code outside the class does; was the row-local stand-in g_speechDurationDivisor.
#include "engine/CMainGameComponent.h"

class CTCTalk
{ public:
    char pad0[0x14];
    int  m_speechTicks;   // +0x14

    float GetCurrentSpeechDuration();
};

float CTCTalk::GetCurrentSpeechDuration()
{
    return (float)m_speechTicks / CMainGameComponent::GetConstantFPS();
}