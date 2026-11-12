// CTCHeroStats::GetInterfaceType() const -- the component's interface type, TCI_HERO_STATS (4)
// Leaf: ?GetInterfaceType@CTCHeroStats@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005814A8, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCHeroStats's vtable 0x0124F70C (the slot holds this body in 1 vtable(s), owner CTCHeroStats); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCHeroStats@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCHeroStats::vfunc_21 (rebuild/src/compiled/00/58/CTCHeroStats_vfunc21_005814a8.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCHEROSTATS_VSLOTS3
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroStats.h"
ETCInterfaceType CTCHeroStats::GetInterfaceType() const
{
    return StatGetInterfaceType();
}
