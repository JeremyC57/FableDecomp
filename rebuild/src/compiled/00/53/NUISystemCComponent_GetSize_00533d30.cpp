// 00533d30 NUISystem::CComponent::GetSize
// Flags:
// Leaf: ?GetSize@CComponent@NUISystem@@UBE?AVC2DVector@@XZ
#define FABLE_RETAIL_STLP_CUISTATE_MAP_GETSIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_CUISTATE_MAP_OWNERS_PASS28
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_NUISYSTEM_CCOMPONENT_MEMBERS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
#include "engine/NUISystem_CUIState.h"
#include "engine/NUISystem_CComponent.h"
#include "stlport/fable_algorithm.h"
namespace NUISystem {
C2DVector CComponent::GetSize() const
{
    C2DVector min(0, 0);
    C2DVector max(0, 0);
    if (Children.empty())
        return min;
    std::vector<unsigned long> shape_children = ShapeChildren;
    long first_child = -1;
    if (shape_children.empty())
        first_child = 0;
    if (0) {} else for (unsigned long count = 0; count < Children.size(); ++count) {
        if (shape_children.empty())
            break;
        if (std::find(shape_children.begin(), shape_children.end(), count) != shape_children.end()) {
            first_child = count;
            break;
        }
    }
    if (first_child == -1)
        return max - min;
    min.X = Children[first_child]->GetStates()[UI_STATE_INACTIVE].Position.X;
    min.Y = Children[first_child]->GetStates()[UI_STATE_INACTIVE].Position.Y;
    max = min + Children[first_child]->GetSize();
    if (0) {} else for (unsigned long count = 0; count < Children.size(); ++count) {
        if (!shape_children.empty() && std::find(shape_children.begin(), shape_children.end(), count) == shape_children.end())
            continue;
        if (count == first_child)
            continue;
        if (Children[count]->GetType() == UI_TYPE_SPRITE && Children[count]->IsPositionIsCenter()) {
            if (Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X - Children[count]->GetSize().X / 2.0f < min.X) {
                min.X = Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X - Children[count]->GetSize().X / 2.0f;
            }
            if (Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y - Children[count]->GetSize().Y / 2.0f < min.Y) {
                min.Y = Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y - Children[count]->GetSize().Y / 2.0f;
            }
        } else {
            if (Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X < min.X)
                min.X = Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X;
            if (Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y < min.Y)
                min.Y = Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y;
        }
        if (Children[count]->GetType() == UI_TYPE_SPRITE && Children[count]->IsPositionIsCenter()) {
            if (Children[count]->GetSize().X / 2.0f + Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X > max.X) {
                max.X = Children[count]->GetSize().X / 2.0f + Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X;
            }
            if (Children[count]->GetSize().Y / 2.0f + Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y > max.Y) {
                max.Y = Children[count]->GetSize().Y / 2.0f + Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y;
            }
        } else {
            if (Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X + Children[count]->GetSize().X > max.X) {
                max.X = Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X + Children[count]->GetSize().X;
            }
            if (Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y + Children[count]->GetSize().Y > max.Y) {
                max.Y = Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y + Children[count]->GetSize().Y;
            }
        }
    }
    return max - min;
}
}