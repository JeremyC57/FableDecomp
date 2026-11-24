// CCreatureAction_PickUpJug::Clone() -- new CCreatureAction_PickUpJug(*this): the implicit copy constructor inlined over the out-of-line CCreatureAction_PickUpThingAtTable copy constructor, then this class's vtable (0x012963A4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009680D0, length 47 bytes.
// Callees: operator new @ 0x00BFEA1A (0xb4 bytes, null-checked), CCreatureAction_PickUpThingAtTable::CCreatureAction_PickUpThingAtTable(const CCreatureAction_PickUpThingAtTable&) @ 0x0084C9A0.
// Shape: PickUpJug adds no data to PickUpThingAtTable (sizeof 0xb4), so its copy constructor is the base copy plus the vtable replacement.
// Leaf: ?Clone@CCreatureAction_PickUpJug@@UBEPAVCCreatureActionBase@@XZ
// Clone port (NF327, 2026-11-24; NF323 Q1 / Q2, NF320 Q1): defined on the real class of engine/CCreatureAction_PickUpJug.h (opt-in
//   FABLE_RETAIL_CCREATUREACTION_PICKUPJUG_VIRTUAL_MODEL) over engine/CCreatureAction_Tankard.h, whose add-only opt-in
//   FABLE_RETAIL_CCREATUREACTION_PICKUPTHINGATTABLE_COPYCTOR_DECL declares the base copy constructor retail calls out of line (FableWin.pdb
//   ??0CCreatureAction_PickUpThingAtTable@@QAE@ABV0@@Z + the landed row 0x0084C9A0), instead of the local one-slot stand-in model
//   (was ?Clone@CCreatureAction_PickUpJug@@QAEPAVCCreatureActionBase@@XZ). The inline implicit copy constructor stores the class vftable,
//   so this TU emits ??_7CCreatureAction_PickUpJug@@6B@ from the header model, slot for slot the retail table.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTION_PICKUPTHINGATTABLE_COPYCTOR_DECL
#define FABLE_RETAIL_CCREATUREACTION_PICKUPJUG_VIRTUAL_MODEL
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_PickUpJug.h"

CCreatureActionBase* CCreatureAction_PickUpJug::Clone() const
{
    return new CCreatureAction_PickUpJug(*this);
}
