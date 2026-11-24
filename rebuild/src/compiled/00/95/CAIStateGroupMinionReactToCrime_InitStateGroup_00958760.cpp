// Leaf: ?InitStateGroup@CAIStateGroup_MinionReactToCrime@@UAEXABVCWorld@@AAVCThingAICreature@@@Z
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CAIStateGroup_MinionReactToCrime.h (class-model switch FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIME; was ?InitStateGroup@CAIStateGroup_MinionReactToCrime_Methods@@QAEXABVCWorld@@AAVCThingAICreature@@@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent CAIStateGroupMinionReactToCrime_OnReplace_00958b20.cpp.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING_FORCEOPENCOUNT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SETDEEDMASK
#define FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIME
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CAIStateGroup_MinionReactToCrime.h"
#include "engine/CAIBrain.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/ThingAlive.h"
#include "engine/CDefClassBase.h"
class CWorld; class CThingAICreature;
void CAIStateGroup_MinionReactToCrime::InitStateGroup(const CWorld&, CThingAICreature&){ GuardTold=0; OpinionDeedHandle=0; AllowDuplicateReactToCrime=0; }
