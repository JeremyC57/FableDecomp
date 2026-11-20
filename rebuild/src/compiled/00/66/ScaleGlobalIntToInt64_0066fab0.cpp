// NF166 (round 10, q11679 / q11691 / q11725): 0x01375550 is ?ConstantFPS@CMainGameComponent@@1JA (protected static long,
//   engine/CMainGameComponent.h), read through the inline public accessor GetConstantFPS() (Ego_r + FableWin
//   ?GetConstantFPS@CMainGameComponent@@SIJXZ) as retail code outside the class does; was the row-local stand-in ScaleGlobalIntToInt64_0066fab0_Value.
#include "engine/CMainGameComponent.h"
extern "C" float ScaleGlobalIntToInt64_0066fab0_Scale;

extern "C" __int64 __cdecl ScaleGlobalIntToInt64_0066fab0()
{
    return (__int64)(
        (float)CMainGameComponent::GetConstantFPS() *
        ScaleGlobalIntToInt64_0066fab0_Scale);
}
