// CCreatureAction_ScorpionChargeInto::~CCreatureAction_ScorpionChargeInto() -- the implicit destructor's own COMDAT: destroy PObstruction (CIntelligentPointer<CThing>, dtor 0x00A01C10), then
// tail-jump to ~CCombatActionBase 0x0062E900 (no vptr store)
// Leaf: ??1CCreatureAction_ScorpionChargeInto@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097C900.
// NF39 (2026-11-16): moved off its `DestVal_118_A` stand-in (catalog label "DestVal...CRegisteredMusicEntry", propagated) onto the shared
//   engine/CCreatureAction_ScorpionChargeInto.h (the implicit destructor: a user-declared one stores the vptr first, retail does not). Evidence: FableWin.pdb name set ??1CCreatureAction_ScorpionChargeInto@@UAE@XZ; Ego_r.pdb PObstruction +0x118 (header); UpdateCombat 0x0097BDA0 destroys its ChargeInto temporary with it (call at 0x0097C351, already bound).
// Defines: the landed caller 0097bda0 preamble.
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
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventCreatureCharging.h"
#include "engine/CTCPhysicalShieldSpell.h"
#include "engine/CTCDPhysicalObstruction.h"
#include "engine/NCreatureMode.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"

// The implicit destructor (the class declares none) emitted as its own COMDAT by these emitters (not part of the graded body), as the
//   landed map rows 0x009AC130 / 0x006C8270 do: Use_ generates it, Kill_ calls it out of line (qualified: no virtual dispatch).
void Use_0097c900(CCreatureAction_ScorpionChargeInto* object) { object->CCreatureAction_ScorpionChargeInto::~CCreatureAction_ScorpionChargeInto(); }
void Kill_0097c900(CCreatureAction_ScorpionChargeInto* object) { object->CCreatureAction_ScorpionChargeInto::~CCreatureAction_ScorpionChargeInto(); }
