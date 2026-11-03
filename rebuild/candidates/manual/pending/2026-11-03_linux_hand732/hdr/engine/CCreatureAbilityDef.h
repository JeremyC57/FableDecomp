#ifndef FABLE_ENGINE_CCREATUREABILITYDEF_H
#define FABLE_ENGINE_CCREATUREABILITYDEF_H
// New shared header (chunk 732, row 0x006AA720 CThingPlayerCreature::SetPlayerCreatureActionForMelee): the retail CCreatureAbilityDef.
// RTTI .?AVCCreatureAbilityDef@@ : CParentDefClassBase (ghidra_out/rtti_bases.tsv); Ego_r.pdb sizeof 0x44 with its one member
// ECreatureAbility Type at +0x40, retail +0x3C (ghidra_out/def_schema.json retail_off 60: the retail CParentDefClassBase is 4 bytes
// shorter, its CVectorMap is 0x10). SetPlayerCreatureActionForMelee compares Type with 1 (the flourish ability) and 7 (the
// block-counter / break-block ability). The enumerator names are not on disk, so ECreatureAbility stays an opaque enum here.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CCreatureAbilityDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
enum ECreatureAbility;
class CCreatureAbilityDef : public CParentDefClassBase {
public:
    ECreatureAbility Type;                                   // +0x3c
    static void CheckRetailLayout() { FABLE_STATIC_ASSERT(offsetof(CCreatureAbilityDef, Type) == 0x3c); }
};
#endif
#endif
