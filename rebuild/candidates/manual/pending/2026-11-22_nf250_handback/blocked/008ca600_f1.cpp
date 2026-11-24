// CCreatureAction_NymphSummonOutOf::CCreatureAction_NymphSummonOutOf(CThingCreatureBase& creature) -- combat action with an unnamed sound
//   pair (priority 150), animation "SUMMON_CREATURES_OUTOF"
// Leaf: ??0CCreatureAction_NymphSummonOutOf@@QAE@AAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CA600, length 116 bytes, `ret 4`. Vtable 0x01282354.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::GetSoundPair (__fastcall) @ 0x008561A0,
//          CCombatActionBase::CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&) @ 0x00858030,
//          CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0.
// Shape: the sound name is a NULL pointer converted implicitly (`push -1 ; push 0`), the hidden-pointer CSoundPair
//   feeds the base constructor, the string dies after it; then a scoped named CCharString for the animation (its
//   constructor pushes are scheduled above the vtable store, GOTCHAS family wave) and the setter call.
// Identity: the stored vtable 0x01282354 is CCreatureAction_NymphSummonOutOf (rtti-vtables.tsv). No member of its own is touched.
// Names (agentNF250, 2026-11-22, CODEXP27 L74): was ??0CCreatureAction_NymphSummonOutOf@@QAE@PAVCThingCreatureBase@@@Z from row-local
//   models. Ego_r.pdb and FableWin.pdb S_PUB32 both spell ??0CCreatureAction_NymphSummonOutOf@@QAE@AAVCThingCreatureBase@@@Z (`ret 4`:
//   one argument); the landed caller 0x0091C6A0 already binds it. Now on the shared engine/CCreatureAction_NymphSummonOutOf.h
//   (FABLE_RETAIL_NYMPH_ATTACK_PASS27, the caller's opt-in) and engine/CCombatActionBase.h (SOUNDCTOR + SETCOMBATANIMATION).
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
#define FABLE_RETAIL_CCOMBATACTIONBASE_SETCOMBATANIMATION
#define FABLE_RETAIL_NYMPH_ATTACK_PASS27
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_NymphSummonOutOf.h"

CCreatureAction_NymphSummonOutOf::CCreatureAction_NymphSummonOutOf(CThingCreatureBase& creature)
    : CCombatActionBase(creature, 150, GetSoundPair(creature, (const char*)0))
{
    {
        CCharString name("SUMMON_CREATURES_OUTOF", -1);
        SetCombatAnimation(name);
    }
}
