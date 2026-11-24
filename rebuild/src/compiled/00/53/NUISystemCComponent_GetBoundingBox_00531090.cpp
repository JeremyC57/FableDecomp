// NUISystem::CComponent::GetBoundingBox
// Leaf: ?GetBoundingBox@CComponent@NUISystem@@UBE?AVC2DBoxF@@XZ
// Flags: /Gr
// Bracket: grouping the centered bottom extent reproduces the store at 0x00531431.
// The uncentered additions also retain the retail position and size evaluation order.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#include "engine/NUISystem_CComponent.h"
#include "engine/C2DBoxF.h"
#include "stlport/fable_algorithm.h"


C2DBoxF NUISystem::CComponent::GetBoundingBox() const
{
    C2DVector min(0.0f, 0.0f);
    C2DVector max(0.0f, 0.0f);
    if (Children.empty())
        return C2DBoxF(min, max);

    std::vector<unsigned long> shape_children(ShapeChildren);
    long first_child = -1;
    if (shape_children.empty())
        first_child = 0;

    for (unsigned long count = 0; count < Children.size() && !shape_children.empty(); ++count) {
        if (std::find(shape_children.begin(), shape_children.end(), count) != shape_children.end()) {
            first_child = count;
            break;
        }
    }
    if (first_child == -1)
        return C2DBoxF(min, max);

    min = Children[first_child]->GetPosition();
    max = min + Children[first_child]->GetDynamicSize();
    for (unsigned long count = 0; count < Children.size(); ++count) {
        if (!shape_children.empty() && std::find(shape_children.begin(), shape_children.end(), count) == shape_children.end())
            continue;
        if (count == static_cast<unsigned long>(first_child))
            continue;
        if (Children[count]->GetType() == UI_TYPE_SPRITE && Children[count]->IsPositionIsCenter()) {
            min.X = std::min(Children[count]->GetPosition().X - Children[count]->GetDynamicSize().X / 2.0f, min.X);
            min.Y = std::min(Children[count]->GetPosition().Y - Children[count]->GetDynamicSize().Y / 2.0f, min.Y);
        } else {
            min.X = std::min(Children[count]->GetPosition().X, min.X);
            min.Y = std::min(Children[count]->GetPosition().Y, min.Y);
        }
        if (Children[count]->GetType() == UI_TYPE_SPRITE && Children[count]->IsPositionIsCenter()) {
            max.X = std::max(Children[count]->GetDynamicSize().X / 2.0f + Children[count]->GetPosition().X, max.X);
            max.Y = std::max((Children[count]->GetDynamicSize().Y / 2.0f) + Children[count]->GetPosition().Y, max.Y);
        } else {
            max.X = std::max(Children[count]->GetPosition().X + Children[count]->GetDynamicSize().X, max.X);
            max.Y = std::max(Children[count]->GetDynamicSize().Y + Children[count]->GetPosition().Y, max.Y);
        }
    }
    C2DBoxF result(min, max);
    return result;
}