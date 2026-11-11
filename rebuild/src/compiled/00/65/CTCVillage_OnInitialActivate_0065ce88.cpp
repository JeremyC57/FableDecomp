// CTCVillage::OnInitialActivate() -- a village restored in limbo re-applies its limbo state on activation
// Leaf: ?OnInitialActivate@CTCVillage@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065CE88, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 6 of CTCVillage's vtable 0x0125A9AC (the slot holds this body in 1 vtable(s), owner CTCVillage); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnInitialActivate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnInitialActivate@CTCVillage@@UAEXXZ; `ret` matches the signature.
// Was: CTCVillage::vfunc_6 (rebuild/src/compiled/00/65/CTCVillage_OnInitialActivate_0065ce88.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCVILLAGE_VSLOTS3
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCVillage.h"
void CTCVillage::OnInitialActivate()
{
    if (Limbo)
        SetVillageLimbo(Limbo);
}
