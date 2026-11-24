// Leaf: ?GetTimeToRespond@CAIStateGroup_MinionGossip@@MBEJXZ
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_GOSSIP
#include "engine/C3DVector.h"
#include "engine/CAIStateGroup_MinionGossip.h"
long CAIStateGroup_MinionGossip::GetTimeToRespond() const
{
    return 60;
}
// Flatten route (flatten_pilot v4, NF324): CAIStateGroup_MinionGossip_GetTimeToRespond -> CAIStateGroup_MinionGossip::GetTimeToRespond, the RTTI-placed PDB member; class model: engine/CAIStateGroup_MinionGossip.h under FABLE_RETAIL_CAISTATEGROUP_GOSSIP (preamble of CAIStateGroupMinionGossip_IsTimingCritical_008fd3dc.cpp).
