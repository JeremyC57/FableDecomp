// Leaf: ?GetTimeToInitiate@CAIStateGroup_BanditGossip@@MBEJXZ
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_GOSSIP
#include "engine/C3DVector.h"
#include "engine/CAIStateGroup_BanditGossip.h"
long CAIStateGroup_BanditGossip::GetTimeToInitiate() const
{
    return 100;
}
// Flatten route (flatten_pilot v4, NF324): CAIStateGroup_BanditGossip_GetTimeToInitiate -> CAIStateGroup_BanditGossip::GetTimeToInitiate, the RTTI-placed PDB member; class model: engine/CAIStateGroup_BanditGossip.h under FABLE_RETAIL_CAISTATEGROUP_GOSSIP (preamble of CAIStateGroupMinionGossip_IsTimingCritical_008fd3dc.cpp).
