// 00531b40 NUISystem::CComponent::PositionChildDying
// Leaf: ?PositionChildDying@CComponent@NUISystem@@UAEXPAV12@@Z
// Flags: /Gr
#define FABLE_RETAIL_COBSERVER_DESTRUCTOR
#define FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27
#define FABLE_RETAIL_NUISYSTEM_CCOMPONENT_MEMBERS
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_TREE_ITERATOR_TRAITS
#define FABLE_RETAIL_STLP_FIND_INPUT
#define FABLE_RETAIL_UI_SET_ERASE_POSITION_PASS30
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#include "engine/NUISystem_CComponent.h"
#include "stlport/fable_algorithm.h"
namespace NUISystem {
void CComponent::PositionChildDying(CComponent* child)
{
    std::set<CComponent*>::iterator it = std::find(PositionChildren.begin(), PositionChildren.end(), child);
    if (it != PositionChildren.end())
        PositionChildren.erase(it);
}
}
