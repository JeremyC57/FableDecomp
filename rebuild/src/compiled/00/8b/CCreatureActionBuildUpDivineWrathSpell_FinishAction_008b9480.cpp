// Leaf: ?FinishAction@CCreatureAction_BuildUpDivineWrathSpell@@UAEXXZ
// CCreatureAction_BuildUpDivineWrathSpell::FinishAction() -- forwards to CCombatActionBase::FinishAction (0x00859A20; retail tail-jumps to it).
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + Ego_r/FableWin name set; the landed stand-in
//   _CCreatureAction_BuildUpDivineWrathSpell_FinishAction becomes the virtual member on
//   engine/CCreatureAction_BuildUpDivineWrathSpell.h (FABLE_RETAIL_CCREATUREACTION_BUILDUPDIVINEWRATHSPELL_VIRTUAL_MODEL, Needs
//   FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR). NF427 G2 (JMP
//   shape); callee: the target's RTTI-port label = its landed self.
#define FABLE_RETAIL_CCREATUREACTION_BUILDUPDIVINEWRATHSPELL_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_BuildUpDivineWrathSpell.h"
void CCreatureAction_BuildUpDivineWrathSpell::FinishAction()
{
    CCombatActionBase::FinishAction();
}
