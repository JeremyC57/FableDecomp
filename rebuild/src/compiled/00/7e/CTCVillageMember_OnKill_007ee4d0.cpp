// CTCVillageMember::OnKill() -- a dying village member is removed from its village (through the village's TCI_VILLAGE component), updating the village camera points
// Leaf: ?OnKill@CTCVillageMember@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EE4D0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 14 of CTCVillageMember's vtable 0x0124007C (the slot holds this body in 1 vtable(s), owner CTCVillageMember); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCVillageMember@@UAEXXZ; `ret` matches the signature.
// Was: CTCVillageMember::vfunc_14 (rebuild/src/compiled/00/7e/CTCVillageMember_vfunc14_007ee4d0.cpp).
#define FABLE_RETAIL_CTCVILLAGE_VSLOTS3
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCVILLAGE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
void CTCVillageMember::OnKill()
{
    if (PVillage.Get()) {
        CTCVillage* village;
        PVillage.Get()->GetTC(TCI_VILLAGE, &village);
        village->RemoveThingAndUpdateCameraPoints(ParentThing);
    }
}
