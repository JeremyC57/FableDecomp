// Leaf: ?IsCombatStateGroup@CAIStateGroup_HoverWander@@UBE_NXZ
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE
#define FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_MOVETOPOSANDFACEPOS
#define FABLE_RETAIL_CAISTATEGROUP_HOVERWANDER
#include <math.h>
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CVectorMap.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CAIStateGroup_HoverWander.h"
bool CAIStateGroup_HoverWander::IsCombatStateGroup() const
{
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CAIStateGroup_HoverWander_IsCombatStateGroup -> CAIStateGroup_HoverWander::IsCombatStateGroup, the RTTI-placed PDB member; class model: engine/CAIStateGroup_HoverWander.h under FABLE_RETAIL_CAISTATEGROUP_HOVERWANDER (preamble of CAIStateGroupHoverWander_Update_00959bf0.cpp).
