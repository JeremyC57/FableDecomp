// CCreatureAction_DragonRearUpFull::~CCreatureAction_DragonRearUpFull() -- the implicit destructor's own COMDAT: destroy PShot (CIntelligentPointer<CThing>, dtor 0x00A01C10), then
// tail-jump to ~CCombatActionBase 0x0062E900 (no vptr store)
// Leaf: ??1CCreatureAction_DragonRearUpFull@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009531A0.
// NF39 (2026-11-16): moved off its `DestVal_114_D` stand-in (catalog label "DestVal...CRegisteredMusicEntry", propagated) onto the shared
//   engine/CCreatureAction_DragonRearUpFull.h (the implicit destructor: a user-declared one stores the vptr first, retail does not). Evidence: FableWin.pdb name set ??1CCreatureAction_DragonRearUpFull@@UAE@XZ; Ego_r.pdb PShot +0x114 (header); UpdateGround 0x00952890 destroys its RearUpFull temporaries with it (already bound).
// Defines: the landed caller 00952890 preamble.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CJACKDRAGONDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCREATUREACTION_SNAPDRAGON
#define FABLE_RETAIL_CCREATUREACTION_SNAPDRAGONFAST
#define FABLE_RETAIL_CCREATUREACTION_DRAGONREARUPFULL
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON_UPDATEGROUND
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBOSS_GENERICCOUNTER
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CJackDragonDef.h"
#include "engine/CCreatureAction_SnapDragon.h"
#include "engine/CCreatureAction_SnapDragonFast.h"
#include "engine/CCreatureAction_DragonRearUpFull.h"
#include "engine/CCreatureAction_DragonRoar.h"
#include "engine/CAIStateGroup_JackDragon.h"
#include "engine/CWorld.h"
#include "engine/CTCBoss.h"
#include "engine/CMainGameComponent.h"
#include "engine/ThingAlive.h"

// The implicit destructor (the class declares none) emitted as its own COMDAT by these emitters (not part of the graded body), as the
//   landed map rows 0x009AC130 / 0x006C8270 do: Use_ generates it, Kill_ calls it out of line (qualified: no virtual dispatch).
void Use_009531a0(CCreatureAction_DragonRearUpFull* object) { object->CCreatureAction_DragonRearUpFull::~CCreatureAction_DragonRearUpFull(); }
void Kill_009531a0(CCreatureAction_DragonRearUpFull* object) { object->CCreatureAction_DragonRearUpFull::~CCreatureAction_DragonRearUpFull(); }
