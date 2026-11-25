// Leaf: ?Initialise@CCreatureAction_CastWillFireballSpell@@UAEXXZ
// CCreatureAction_CastWillFireballSpell::Initialise() -- forwards to CCombatActionBase::Initialise (0x00857520; retail tail-jumps to it).
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + Ego_r/FableWin name set; the landed stand-in
//   _CCreatureAction_CastWillFireballSpell_Initialise becomes the virtual member on engine/CCreatureAction_CastWillFireballSpell.h
//   (FABLE_RETAIL_CCREATUREACTION_CASTWILLFIREBALLSPELL_VIRTUAL_MODEL, Needs FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE,
//   FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR). NF427 G2 (JMP shape); callee: the target's RTTI-port label = its
//   landed self.
#define FABLE_RETAIL_CCREATUREACTION_CASTWILLFIREBALLSPELL_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_CastWillFireballSpell.h"
void CCreatureAction_CastWillFireballSpell::Initialise()
{
    CCombatActionBase::Initialise();
}
