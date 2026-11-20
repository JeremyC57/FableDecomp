// IS_THING_ALIVE<const CThing*>(const CThing* const& thing) -- the thing exists and its dead bit (+0x91 bit 0) is clear
// Leaf: ??$IS_THING_ALIVE@PBVCThing@@@@YI_NABQBVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00436380, 21 bytes (`mov eax,[ecx] ; test ; je ; test byte ptr [eax+91h],1 ; jne ; mov al,1 ; ret`).
// Identity (NF196, 2026-11-20, q6447): the IS_THING_ALIVE<const CThing*> instance, FableWin.pdb 0x01AE3C10
//   ??$IS_THING_ALIVE@PBVCThing@@@@YI_NABQBVCThing@@@Z (Ego_r.pdb inlines it). Byte-identical to the CThing* instance 0x0040DCE0, but a
//   separate COMDAT: 0x007BDAC7 calls both from one function (the const target at 0x00436380, the hit thing at 0x0040DCE0), and every
//   landed caller (0x0057D5A0 0x005B4EBA 0x0064A1AE 0x0065A852 0x007BBE34 0x007BC38A 0x007BC6FA 0x007BC8FC 0x007BDAC7) binds this spelling.
//   Was the stand-in ?IS_THING_ALIVE@@YI_NPBUThingRef@@@Z on a row-local CThing model; now on the shared engine/CThing.h, in the shape of
//   the landed 0x0040DCE0 row.
#include "engine/CThing.h"
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    if (thing != 0 && thing->IsDead() == 0)
        return true;
    return false;
}
template bool __fastcall IS_THING_ALIVE<const CThing*>(const CThing* const& thing);
