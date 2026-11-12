// CTCPerceiveThing::OnKill() -- resets the perception state, then the base kill path
// Leaf: ?OnKill@CTCPerceiveThing@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00744F70, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 14 of CTCPerceiveThing's vtable 0x012688CC (the slot holds this body in 1 vtable(s), owner CTCPerceiveThing); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCPerceiveThing@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCPerceiveThing::vfunc_14 (rebuild/src/compiled/00/74/global_Destval_00744f70.cpp).
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPerceiveThing.h"
void CTCPerceiveThing::OnKill()
{
    ResetPerceptionVariables();
    CTCBase::OnKill();
}
