// CCombatAction_WillHealInto::CCombatAction_WillHealInto(CThingCreatureBase& creature, long spell_level) -- base
//   CCombatActionBase(creature, 150, GetSoundPair(creature, "SWORD_SWING")), SpellLevel at +0x114
// Leaf: ??0CCombatAction_WillHealInto@@QAE@AAVCThingCreatureBase@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00956870, length 90 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("SWORD_SWING" @ 0x0127AC80, -1),
// CCombatActionBase::GetSoundPair(const CThingCreatureBase&, const CCharString&) @ 0x008561A0 (__fastcall: CSoundPair returned through the
// hidden pointer in ecx, the creature in edx, the string reference pushed), CCombatActionBase::CCombatActionBase
// (CThingCreatureBase&, long, const CSoundPair&) @ 0x00858030, CCharString::~CCharString @ 0x0099EAE0.
// Vtable 0x012948AC = CCombatAction_WillHealInto (rtti-vtables.tsv).
// Shape: the string argument is a CONVERSION temporary (the literal converts through CCharString(const char*, long = -1)), alive for
// the whole initialiser expression (destroyed after the base call); the sound pair result is built at [esp+8] and its address is pushed
// as the reference argument, then the vtable store and the member store from the second argument.
// Names (agentNF250, 2026-11-22, CODEXP27 L100): was ??0CCombatAction_WillHealInto@@QAE@PAVCThingCreatureBase@@J@Z from row-local
//   models. Ego_r.pdb and FableWin.pdb S_PUB32 both spell ??0CCombatAction_WillHealInto@@QAE@AAVCThingCreatureBase@@J@Z; the landed
//   caller 0x00955620 already binds it. Now on the shared engine/CCombatAction_WillHealInto.h (SpellLevel, size 0x118) and
//   engine/CCombatActionBase.h (SOUNDCTOR).
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
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatAction_WillHealInto.h"

CCombatAction_WillHealInto::CCombatAction_WillHealInto(CThingCreatureBase& creature, long spell_level)
    : CCombatActionBase(creature, 150, GetSoundPair(creature, "SWORD_SWING"))
    , SpellLevel(spell_level)
{
}
