// Retail 0x007fd3d0  CAIStateGroup_BuyFromSeller::BoughtSomething
// __fastcall void(this, float amount)
//   fild [0x1375550]        ; int global
//   fmul [esp+arg]          ; * float amount
//   call _ftol              ; (long) truncate
//   mov [this+0x24], eax

// NF166 (round 10, q11679 / q11691 / q11725): 0x01375550 is ?ConstantFPS@CMainGameComponent@@1JA (protected static long,
//   engine/CMainGameComponent.h), read through the inline public accessor GetConstantFPS() (Ego_r + FableWin
//   ?GetConstantFPS@CMainGameComponent@@SIJXZ) as retail code outside the class does; was the row-local stand-in g_BuyFromSeller_Scale.
#include "engine/CMainGameComponent.h"

struct CAIStateGroup_BuyFromSeller
{
    char pad[0x24];
    int  field_24;

    void BoughtSomething(float amount);
};


void CAIStateGroup_BuyFromSeller::BoughtSomething(float amount)
{
    this->field_24 = (int)((float)CMainGameComponent::GetConstantFPS() * amount);
}