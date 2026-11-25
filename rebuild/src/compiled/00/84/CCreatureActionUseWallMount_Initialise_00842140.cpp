// Leaf: ?Initialise@CCreatureAction_UseWallMount@@UAEXXZ
// CCreatureAction_UseWallMount::Initialise() -- forwards to CCreatureActionBase::Initialise (0x00694D10; retail tail-jumps to it).
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + Ego_r/FableWin name set; the landed stand-in
//   _CCreatureAction_UseWallMount_Initialise becomes the virtual member on engine/CCreatureAction_UseWallMount.h
//   (FABLE_RETAIL_CCREATUREACTION_USEWALLMOUNT_VIRTUAL_MODEL, Needs FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_CCREATUREACTIONBASE). NF427
//   G2 (JMP shape); callee: the target's RTTI-port label = its landed self.
#define FABLE_RETAIL_CCREATUREACTION_USEWALLMOUNT_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureAction_UseWallMount.h"
void CCreatureAction_UseWallMount::Initialise()
{
    CCreatureActionBase::Initialise();
}
