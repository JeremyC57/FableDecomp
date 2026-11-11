// CThingFilter_IsNot::operator()(const CThing& thing) const -- true for every thing except the one the filter excludes
// Leaf: ??RCThingFilter_IsNot@@QBE_NABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A76A0, 14 bytes: `mov ecx,[ecx] ; mov edx,[esp+4] ; xor eax,eax ; cmp ecx,edx ;
//   setne al ; ret 4`.
// Name fix (naming agent NF21, 2026-11-11): Ego_r.pdb and FableWin.pdb spell ??RCThingFilter_IsNot@@QBE_NABVCThing@@@Z (Ego_r
//   CThingFilter_IsNot { CThing* Thing; }, 4 bytes); the landed GetPFirstThing / GetPBestThing<CFilter_Bind2/3<CThingFilter_IsNot,..> >
//   rows call this address as the IsNot filter. The row was the generated stand-in S::op(void*).
// Layout: the shared engine/CThingFilter_IsNot.h (the excluded thing held by reference at +0x00).
class CThing;
#include "engine/CThingFilter_IsNot.h"

bool CThingFilter_IsNot::operator()(const CThing& thing) const
{
    return &Thing != &thing;
}
