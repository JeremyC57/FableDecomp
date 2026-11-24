// Leaf: ?GetStateGroupType@CAIStateGroup_FollowPlayer@@UBE?AW4EAIStateGroupType@@XZ
#define FABLE_RETAIL_CAISTATEGROUP_FOLLOWPLAYER_RTTI_DECLS
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_FOLLOWPLAYER
#include "engine/CAIStateGroup_FollowPlayer.h"
#pragma optimize("s",on)
EAIStateGroupType CAIStateGroup_FollowPlayer::GetStateGroupType() const
{
    return (EAIStateGroupType)2;
}
// Flatten route (flatten_pilot v4, NF324): CAIStateGroup_FollowPlayer_GetStateGroupType -> CAIStateGroup_FollowPlayer::GetStateGroupType, the RTTI-placed PDB member; class model: engine/CAIStateGroup_FollowPlayer.h opt-in FABLE_RETAIL_CAISTATEGROUP_FOLLOWPLAYER, FABLE_RETAIL_CAISTATEGROUP_FOLLOWPLAYER_RTTI_DECLS (preamble of CAIStateGroupFollowPlayer_OnActivate_0094ef70.cpp).
