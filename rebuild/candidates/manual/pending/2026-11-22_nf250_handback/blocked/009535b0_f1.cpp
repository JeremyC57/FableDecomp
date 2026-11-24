// CCreatureAction_DragonFireBeamInto::CCreatureAction_DragonFireBeamInto(CThingCreatureBase& creature, CThing& target, float time) --
//   combat action on a target with the SWORD_SWING sound pair (priority 250); the beam timer comes from the argument
// Leaf: ??0CCreatureAction_DragonFireBeamInto@@QAE@AAVCThingCreatureBase@@AAVCThing@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009535B0, length 97 bytes, `ret 0Ch` (three arguments).
// Identity: the stored vtable 0x01293E54 is CCreatureAction_DragonFireBeamInto (rtti-vtables.tsv). Ego_r.pdb gives the class one float,
//   Timer (PDB +0x12c, retail +0x114: the retail CCombatActionBase is 0x114 bytes).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::GetSoundPair (__fastcall) @ 0x008561A0,
//          CCombatActionBase::CCombatActionBase(CThingCreatureBase&, CThing&, long, const CSoundPair&) @ 0x00857F30,
//          CCharString::~CCharString @ 0x0099EAE0.
// Shape: the "SWORD_SWING" literal converts implicitly to a CCharString temporary (re-addressed with `lea`), the
//   CSoundPair (user constructor -> returned through the hidden pointer in ecx) lands in a frame temporary that is
//   passed by reference to the base constructor; the string dies after the base call, then the vtable store and the
//   Timer initialiser (a float copied through edx from the third argument slot).
// Names (agentNF250, 2026-11-22, CODEXP27 L92): was ??0CCreatureAction_DragonFireBeamInto@@QAE@PAVCThingCreatureBase@@PAVCThing@@M@Z from
//   row-local models. Ego_r.pdb and FableWin.pdb S_PUB32 both spell ??0CCreatureAction_DragonFireBeamInto@@QAE@AAVCThingCreatureBase@@AAVCThing@@M@Z;
//   the landed caller 0x00951FB0 (CAIStateGroup_JackDragon::UpdateHover) already binds it. Now on the shared
//   engine/CCreatureAction_DragonFireBeamInto.h (FABLE_RETAIL_JACKDRAGON_HOVER_PASS27, the caller's opt-in) and engine/CCombatActionBase.h.
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
#define FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_DragonFireBeamInto.h"

CCreatureAction_DragonFireBeamInto::CCreatureAction_DragonFireBeamInto(CThingCreatureBase& creature, CThing& target, float time)
    : CCombatActionBase(creature, target, 250, GetSoundPair(creature, "SWORD_SWING"))
    , Timer(time)
{
}
