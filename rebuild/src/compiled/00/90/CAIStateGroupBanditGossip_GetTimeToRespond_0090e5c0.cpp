// Leaf: ?GetTimeToRespond@CAIStateGroup_BanditGossip@@MBEJXZ
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_GOSSIP
#include "engine/C3DVector.h"
#include "engine/CAIStateGroup_BanditGossip.h"
long CAIStateGroup_BanditGossip::GetTimeToRespond() const
{
    return 90;
}
// Flatten route (flatten_pilot v4, NF324): CAIStateGroup_BanditGossip_GetTimeToRespond -> CAIStateGroup_BanditGossip::GetTimeToRespond, the RTTI-placed PDB member; class model: engine/CAIStateGroup_BanditGossip.h under FABLE_RETAIL_CAISTATEGROUP_GOSSIP (preamble of CAIStateGroupMinionGossip_IsTimingCritical_008fd3dc.cpp).
