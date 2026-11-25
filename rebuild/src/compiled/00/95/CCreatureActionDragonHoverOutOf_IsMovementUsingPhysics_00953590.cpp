// Leaf: ?IsMovementUsingPhysics@CCreatureAction_DragonHoverOutOf@@UBE_NXZ
// CCreatureAction_DragonHoverOutOf::IsMovementUsingPhysics() const -- returns false.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   @CCreatureAction_DragonHoverOutOf_IsMovementUsingPhysics@4 becomes the virtual member on engine/CCreatureAction_DragonHoverOutOf.h
//   (FABLE_RETAIL_CCREATUREACTION_DRAGONHOVEROUTOF_VIRTUAL_MODEL, Needs FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE,
//   FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR). NF427 G1 (CONST shape).
#define FABLE_RETAIL_CCREATUREACTION_DRAGONHOVEROUTOF_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_DragonHoverOutOf.h"
bool CCreatureAction_DragonHoverOutOf::IsMovementUsingPhysics() const
{
    return false;
}
