// Leaf: ?IsTimingCritical@CAIStateGroup_StealStealableItem@@UBE_NXZ
#define FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM_RTTI_DECLS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM
#define FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM_ISRUNNABLE
#define FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM_FINDPSTEALABLEITEM
#define FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM_FINDPESCAPEPOINT
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_StealStealableItem.h"
bool CAIStateGroup_StealStealableItem::IsTimingCritical() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CAIStateGroup_StealStealableItem_IsTimingCritical -> CAIStateGroup_StealStealableItem::IsTimingCritical, the RTTI-placed PDB member; class model: engine/CAIStateGroup_StealStealableItem.h opt-in FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM, FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM_RTTI_DECLS (preamble of CAIStateGroupStealStealableItem_IsRunnable_00964da0.cpp).
