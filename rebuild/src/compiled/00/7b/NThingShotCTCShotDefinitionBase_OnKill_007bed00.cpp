// NThingShot::CTCShotDefinitionBase::OnKill() -- the kill path is the remove path (virtual OnRemove, slot 2)
// Leaf: ?OnKill@CTCShotDefinitionBase@NThingShot@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BED00, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 14 of NThingShot::CTCShotDefinitionBase's vtable 0x0126E1A4 (the slot holds this body in 14 vtable(s), owner NThingShot::CTCShotDefinitionBase); the Xbox 360 Test PDB virtual order (third source) aligned on 25 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCShotDefinitionBase@NThingShot@@UAEXXZ; no `ret` to check (NORET).
// Was: NThingShot::CTCShotDefinitionThrow::vfunc_14 (rebuild/src/compiled/00/7b/NThingShotCTCShotDefinitionThrow_vfunc14_007bed00.cpp).
#define FABLE_RETAIL_NTHINGSHOT_CTCSHOTDEFINITIONBASE_VSLOTS3
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CThing.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
namespace NThingShot {
void CTCShotDefinitionBase::OnKill()
{
    OnRemove();
}
}
