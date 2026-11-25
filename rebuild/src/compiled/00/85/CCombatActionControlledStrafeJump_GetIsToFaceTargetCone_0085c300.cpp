// Leaf: ?GetIsToFaceTargetCone@CCombatAction_ControlledStrafeJump@@UBEMXZ
// CCombatAction_ControlledStrafeJump::GetIsToFaceTargetCone() const -- returns 0.99722224f (retail loads the pooled float literal at 0x01276AB8).
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + Ego_r/FableWin name set; the landed stand-in
//   _CCombatAction_ControlledStrafeJump_GetIsToFaceTargetCone becomes the virtual member on engine/CCombatAction_ControlledStrafeJump.h
//   (FABLE_RETAIL_CCOMBATACTION_CONTROLLEDSTRAFEJUMP_VIRTUAL_MODEL, Needs FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE,
//   FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR). NF427 G1 (CONST shape).
#define FABLE_RETAIL_CCOMBATACTION_CONTROLLEDSTRAFEJUMP_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCombatAction_ControlledStrafeJump.h"
float CCombatAction_ControlledStrafeJump::GetIsToFaceTargetCone() const
{
    return 0.99722224f;
}
