// CCreatureAction_PickUpTankard::CCreatureAction_PickUpTankard(CThingCreatureBase& creature, CThing& tankard) -- base
//   CCreatureAction_PickUpThingAtTable(creature, tankard, "DRINK_AT_TABLE_INTO", MODE_DRINKING_FROM_TANKARD), own vtable (0x01292D6C)
// Leaf: ??0CCreatureAction_PickUpTankard@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00947B70, length 64 bytes, `ret 8`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCreatureAction_PickUpThingAtTable::CCreatureAction_PickUpThingAtTable(
//   CThingCreatureBase&, CThing&, const CCharString&, NCreatureMode::EMode) @ 0x008440C0, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the animation name comes from an inline helper returning it by value, so it is built in the one-word frame before any argument
//   is pushed and its address re-derived for the call (a bare temporary argument pushes the trailing constant first and reuses the
//   constructor's eax). Class name from rtti-vtables.tsv (the chunk label is propagated).
// Names (agentNF250, 2026-11-22, CODEXP27 L89): was ??0CCreatureAction_PickUpTankard@@QAE@PAVCThingCreatureBase@@PAVCThing@@@Z from
//   row-local models. FableWin.pdb S_PUB32 spells ??0CCreatureAction_PickUpTankard@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z (Ego_r.pdb
//   keeps only the copy constructor), and the landed caller 0x00948880 (CAIStateGroup's tankard drink, retail call into this address)
//   already binds it; `ret 8` = two arguments. Now on the shared engine/CCreatureAction_Tankard.h (FABLE_RETAIL_DRINK_FIXTURE_PASS27,
//   the caller's opt-in); 39 = NCreatureMode::MODE_DRINKING_FROM_TANKARD (engine/NCreatureMode.h).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_PUMPCOMBATACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBOSS_STRIKES
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCPHYSICALSHIELDSPELL_ALLOWREACTIONS
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_CREATE
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DRINK_FIXTURE_PASS27
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/NCreatureMode.h"
#include "engine/CCreatureAction_Tankard.h"

static inline CCharString AnimationName() { return CCharString("DRINK_AT_TABLE_INTO", -1); }

CCreatureAction_PickUpTankard::CCreatureAction_PickUpTankard(CThingCreatureBase& creature, CThing& tankard)
    : CCreatureAction_PickUpThingAtTable(creature, tankard, AnimationName(), NCreatureMode::MODE_DRINKING_FROM_TANKARD)
{
}
