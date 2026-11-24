// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `b114` -> `Landed`.
// Leaf: ?IsToEvadeAllHits@CCreatureAction_LeapLong@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_LEAPLONG_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_LeapLong.h"  // retyped onto the PDB layout; byte parity re-verified
bool CCreatureAction_LeapLong::IsToEvadeAllHits() const
{
    CCreatureAction_LeapLong* action = const_cast<CCreatureAction_LeapLong*>(this); return action->Landed == 0; }
// Flatten route (flatten_pilot v5, NF329): CCreatureAction_LeapLong_IsToEvadeAllHits -> CCreatureAction_LeapLong::IsToEvadeAllHits, the RTTI-placed PDB member; class model: engine/CCreatureAction_LeapLong.h opt-in FABLE_RETAIL_CCREATUREACTION_LEAPLONG_VIRTUAL_MODEL, FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR.
