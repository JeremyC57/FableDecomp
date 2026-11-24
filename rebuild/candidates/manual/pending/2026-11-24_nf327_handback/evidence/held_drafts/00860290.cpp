// CCreatureAction_GetUpFromSitting::Clone() const -- new CCreatureAction_GetUpFromSitting(*this): the implicit copy constructor inlined (base copy out of line, own vtable, members copied one register each)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00860290, length 71 bytes. Vtable slot 11 (Clone).
// Callees: operator new @ 0x00BFEA1A (0x11c bytes, null-checked), CCombatActionBase::CCombatActionBase(const CCombatActionBase&)
// @ 0x0062F3B0. Vtable 0x01276C2C (rtti-vtables.tsv). SittingMode / RemoveOnPerformAction are the Ego_r.pdb names.
// Retail CCombatActionBase is 0x114 bytes (PDB 0x12c), so the members sit at +0x114.. (PDB +0x12c..) (the shared headers model them now).
// Leaf: ?Clone@CCreatureAction_GetUpFromSitting@@UBEPAVCCreatureActionBase@@XZ
// Clone port (NF323, 2026-11-24; NF317 Q3 / NF320 Q1): defined on the real class of engine/CCreatureAction_GetUpFromSitting.h (opt-in
//   FABLE_RETAIL_CCREATUREACTION_GETUPFROMSITTING_RTTI_DECLS: Clone public as both PDBs spell it, plus the missing overrides) over the retail
//   engine/CCombatActionBase.h branch (FABLE_RETAIL_KRAKEN_ROAR, landed definer 0x00924B90) instead of the local one-slot stand-in
//   (was ?Clone@CCreatureAction_GetUpFromSitting@@QBEPAVCCreatureActionBase@@XZ). The inline implicit copy constructor stores the class vftable, so this TU emits
//   ??_7CCreatureAction_GetUpFromSitting@@6B@ from the header model, slot for slot the retail table 0x01276C2C.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR
#define FABLE_RETAIL_KRAKEN_ROAR
#define FABLE_RETAIL_CCREATUREACTION_GETUPFROMSITTING_RTTI_DECLS
#include "engine/CCreatureActionBase.h"
#include "engine/CSoundPair.h"
class CAnimationEntry;
#include "engine/CCreatureAction_GetUpFromSitting.h"

CCreatureActionBase* CCreatureAction_GetUpFromSitting::Clone() const
{
    return (CCreatureActionBase*)new CCreatureAction_GetUpFromSitting(*this);
}
