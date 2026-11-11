// CTCPhysicalShieldSpell::OnKill() -- the kill path: FinalCleanup
// Leaf: ?OnKill@CTCPhysicalShieldSpell@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079EC70, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 14 of CTCPhysicalShieldSpell's vtable 0x0126C53C (the slot holds this body in 1 vtable(s), owner CTCPhysicalShieldSpell); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCPhysicalShieldSpell@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCPhysicalShieldSpell::vfunc_14 (rebuild/src/compiled/00/79/gapscan_sub0079ec70_0079ec70.cpp).
#define FABLE_RETAIL_BRIAR_ROSE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CTCPhysicalShieldSpell.h"
void CTCPhysicalShieldSpell::OnKill()
{
    FinalCleanup();
}
