// CCreatureAction_ScorpionChargeInto::CCreatureAction_ScorpionChargeInto(CThingCreatureBase& creature, CThing& target, float charge_distance) --
// base CCombatActionBase(creature, target, 150, GetSoundPair(creature, "SWORD_SWING")), an empty PObstruction, then ChargeDistance
// Leaf: ??0CCreatureAction_ScorpionChargeInto@@QAE@AAVCThingCreatureBase@@AAVCThing@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4E20, length 114 bytes (+ int3 padding), `ret 0Ch`. Vtable 0x01280ED4.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::GetSoundPair @ 0x008561A0, CCombatActionBase::
//   CCombatActionBase(CThingCreatureBase&, CThing&, long, const CSoundPair&) @ 0x00857F30, CCharString::~CCharString @ 0x0099EAE0,
//   CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10.
// Shape: after the vtable store the CIntelligentPointer member at +0x118 is built (base ctor, then its vtable 0x01238C6C), and the float is
//   stored to +0x114 last (a body assignment: scalars stored after member constructors are body code).
// NF39 (2026-11-16): moved off its local CCombatActionBase / CCharString / CSoundPair / class models (pointer arguments, a `long` third argument)
//   onto the shared engine/CCreatureAction_ScorpionChargeInto.h (opt-in FABLE_RETAIL_CCREATUREACTION_SCORPIONCHARGEINTO, which names 0x008C4E20:
//   ChargeDistance float +0x114, PObstruction +0x118) and engine/CCombatActionBase.h (SOUNDCTOR: GetSoundPair; new TARGETCTOR: the Ego_r.pdb
//   (creature, target, priority, sound) constructor 0x00857F30). Evidence: Ego_r.pdb ??0CCreatureAction_ScorpionChargeInto@@QAE@AAVCThingCreatureBase@@AAVCThing@@M@Z
//   (references, float stored as ChargeDistance); CAIStateGroup_ScorpionKing::UpdateCombat 0x0097BDA0 already binds it.
// Defines: the landed 0x0097BDA0 UpdateCombat preamble + SOUNDCTOR + TARGETCTOR.
#define FABLE_RETAIL_CCOMBATACTIONBASE_ALLOWSCOMBINATIONANIMS  // NF264 (q12750): CCombatActionBase::AllowsCombinationAnims override, so emitted tables bind slot 66 = 0x0062EA10 as retail
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
#define FABLE_RETAIL_CAISTATEGROUP_SCORPIONKING
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONCHARGEINTO
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONUNBLOCKABLELEFTTURNATTACK
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONUNBLOCKABLERIGHTTURNATTACK
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONPREPARETAILPLUNGE
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
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroup_ScorpionKing.h"
#include "engine/CCreatureAction_ScorpionChargeInto.h"
#include "engine/CCreatureAction_ScorpionUnblockableLeftTurnAttack.h"
#include "engine/CCreatureAction_ScorpionUnblockableRightTurnAttack.h"
#include "engine/CCreatureAction_ScorpionPrepareTailPlunge.h"
#include "engine/CTCBoss.h"
#include "engine/CTCPhysicsBase.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAIBrain.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CScorpionKingBattleDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventCreatureCharging.h"
#include "engine/CTCPhysicalShieldSpell.h"
#include "engine/CTCDPhysicalObstruction.h"
#include "engine/NCreatureMode.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"

CCreatureAction_ScorpionChargeInto::CCreatureAction_ScorpionChargeInto(CThingCreatureBase& creature, CThing& target, float charge_distance)
    : CCombatActionBase(creature, target, 150, GetSoundPair(creature, "SWORD_SWING"))
{
    ChargeDistance = charge_distance;
}
