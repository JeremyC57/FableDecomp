// CTCVillage::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCVillage@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00661248, thiscall, `ret`.
// Identity: slot 23 of CTCVillage's vtable 0x0125A9AC (defined here: the slot holds this body in 1 vtable(s), all of CTCVillage or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCVillage@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCVillage.h (opt-ins as in the landed row CTCVillage_GetIsHeroEnemyOfGuards_00658604.cpp).
#define FABLE_RETAIL_CTCVILLAGE_VSLOTS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_APOLOGISE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCVillage.h"
#include "engine/CWorld.h"
bool CTCVillage::IsComponentSerialisable() const
{
    return true;
}
