// Leaf: ?GetAnimExtraFlags@CCreatureAction_PutGoodsDown@@UBEKXZ
// CCreatureAction_PutGoodsDown::GetAnimExtraFlags() const -- returns 8.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   @CCreatureAction_PutGoodsDown_GetAnimExtraFlags@4 becomes the virtual member on engine/CCreatureAction_PutGoodsDown.h
//   (FABLE_RETAIL_CCREATUREACTION_PUTGOODSDOWN_VIRTUAL_MODEL, Needs FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_CCREATUREACTIONBASE). NF427
//   G1 (CONST shape).
#define FABLE_RETAIL_CCREATUREACTION_PUTGOODSDOWN_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureAction_PutGoodsDown.h"
unsigned long CCreatureAction_PutGoodsDown::GetAnimExtraFlags() const
{
    return 8;
}
