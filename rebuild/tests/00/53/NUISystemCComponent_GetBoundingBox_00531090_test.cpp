// Exercise the full box implementation with shared widgets and vectors.
#define FABLE_RETAIL_DEF_CLASS_BASE
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

#include "engine/CDefPointer.h"
#include "engine/NUISystem_CDeletion.h"
#include <stdio.h>
#include <stdlib.h>
CCharString::CCharString() : PStringData(0) {}
CCharString::~CCharString() { delete PStringData; }
namespace NUISystem {
CUIInterface::~CUIInterface() {}
CObserver::CObserver() {}
CObserver::~CObserver() {}
CComponent::CComponent(long) {}
CComponent::~CComponent() {}
void CUIInterface::Update(float) { fprintf(stderr, "unexpected CUIInterface::Update\n"); abort(); }
void CUIInterface::Draw(CIEngine &, CEnginePrimitiveHandle *, long, long *, NUISystem::CComponent *) { fprintf(stderr, "unexpected CUIInterface::Draw\n"); abort(); }
bool CObserver::IsObservingEvent(EEvent) { fprintf(stderr, "unexpected CObserver::IsObservingEvent\n"); abort(); }
void CObserver::ObserveEvent(EEvent) { fprintf(stderr, "unexpected CObserver::ObserveEvent\n"); abort(); }
void CObserver::IgnoreEvent(EEvent) { fprintf(stderr, "unexpected CObserver::IgnoreEvent\n"); abort(); }
void CObserver::IgnoreAllEvents() { fprintf(stderr, "unexpected CObserver::IgnoreAllEvents\n"); abort(); }
void CObserver::ObserveAllEvents() { fprintf(stderr, "unexpected CObserver::ObserveAllEvents\n"); abort(); }
void CObserver::SetPreventObserving(bool) { fprintf(stderr, "unexpected CObserver::SetPreventObserving\n"); abort(); }
void CComponent::Update(float) { fprintf(stderr, "unexpected CComponent::Update\n"); abort(); }
void CComponent::Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*) { fprintf(stderr, "unexpected CComponent::Draw\n"); abort(); }
NUISystem::CComponent * CComponent::FindChild(CCharString) { fprintf(stderr, "unexpected CComponent::FindChild\n"); abort(); }
NUISystem::CComponent * CComponent::FindChildForward(CCharString) { fprintf(stderr, "unexpected CComponent::FindChildForward\n"); abort(); }
void CComponent::FindAllChildren(CCharString, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &) { fprintf(stderr, "unexpected CComponent::FindAllChildren\n"); abort(); }
void CComponent::FindAllChildrenOfType(NUISystem::EType, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &) { fprintf(stderr, "unexpected CComponent::FindAllChildrenOfType\n"); abort(); }
void CComponent::SetPAcceptButton(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::SetPAcceptButton\n"); abort(); }
NUISystem::CComponent * CComponent::FindChildNonRecursive(CCharString) { fprintf(stderr, "unexpected CComponent::FindChildNonRecursive\n"); abort(); }
NUISystem::CComponent * CComponent::FindFirstChildOfType(NUISystem::EType) { fprintf(stderr, "unexpected CComponent::FindFirstChildOfType\n"); abort(); }
bool CComponent::HasChild(NUISystem::CComponent *) const { fprintf(stderr, "unexpected CComponent::HasChild\n"); abort(); }
void CComponent::SetZoom(const C2DVector &) { fprintf(stderr, "unexpected CComponent::SetZoom\n"); abort(); }
C2DVector CComponent::GetZoom() const { fprintf(stderr, "unexpected CComponent::GetZoom\n"); abort(); }
void CComponent::SetColour(const CRGBColour &) { fprintf(stderr, "unexpected CComponent::SetColour\n"); abort(); }
CRGBColour CComponent::GetColour() const { fprintf(stderr, "unexpected CComponent::GetColour\n"); abort(); }
void CComponent::SetPosition(const C2DVector &) { fprintf(stderr, "unexpected CComponent::SetPosition\n"); abort(); }
C2DVector CComponent::GetPosition() const { fprintf(stderr, "unexpected CComponent::GetPosition\n"); abort(); }
C2DVector CComponent::GetParentPosition() const { fprintf(stderr, "unexpected CComponent::GetParentPosition\n"); abort(); }
void CComponent::SetParentPosition(C2DVector &) { fprintf(stderr, "unexpected CComponent::SetParentPosition\n"); abort(); }
C2DVector CComponent::GetParentZoom() const { fprintf(stderr, "unexpected CComponent::GetParentZoom\n"); abort(); }
void CComponent::SetParentZoom(C2DVector &) { fprintf(stderr, "unexpected CComponent::SetParentZoom\n"); abort(); }
CRGBColour CComponent::GetParentColour() const { fprintf(stderr, "unexpected CComponent::GetParentColour\n"); abort(); }
void CComponent::SetParentColour(CRGBColour &) { fprintf(stderr, "unexpected CComponent::SetParentColour\n"); abort(); }
C2DVector CComponent::GetSize() const { fprintf(stderr, "unexpected CComponent::GetSize\n"); abort(); }
const C2DVector & CComponent::GetRenderPosition() const { fprintf(stderr, "unexpected CComponent::GetRenderPosition\n"); abort(); }
const C2DVector & CComponent::GetRenderZoom() const { fprintf(stderr, "unexpected CComponent::GetRenderZoom\n"); abort(); }
const CRGBColour & CComponent::GetRenderColour() const { fprintf(stderr, "unexpected CComponent::GetRenderColour\n"); abort(); }
void CComponent::SetRenderPosition(C2DVector &) { fprintf(stderr, "unexpected CComponent::SetRenderPosition\n"); abort(); }
void CComponent::SetRenderZoom(C2DVector &) { fprintf(stderr, "unexpected CComponent::SetRenderZoom\n"); abort(); }
void CComponent::SetRenderColour(CRGBColour &) { fprintf(stderr, "unexpected CComponent::SetRenderColour\n"); abort(); }
void CComponent::ChangePosition(const C2DVector &, float, bool) { fprintf(stderr, "unexpected CComponent::ChangePosition\n"); abort(); }
void CComponent::ChangePositionDelta(const C2DVector &, float, bool) { fprintf(stderr, "unexpected CComponent::ChangePositionDelta\n"); abort(); }
void CComponent::ChangeTargetPositionDelta(const C2DVector &, float, bool) { fprintf(stderr, "unexpected CComponent::ChangeTargetPositionDelta\n"); abort(); }
void CComponent::UpdatePosition(float) { fprintf(stderr, "unexpected CComponent::UpdatePosition\n"); abort(); }
void CComponent::ChangeZoom(const C2DVector &, float, bool) { fprintf(stderr, "unexpected CComponent::ChangeZoom\n"); abort(); }
void CComponent::ChangeZoomDelta(const C2DVector &, float, bool) { fprintf(stderr, "unexpected CComponent::ChangeZoomDelta\n"); abort(); }
void CComponent::UpdateZoom(float) { fprintf(stderr, "unexpected CComponent::UpdateZoom\n"); abort(); }
void CComponent::ChangeColour(const CRGBColour &, float, bool) { fprintf(stderr, "unexpected CComponent::ChangeColour\n"); abort(); }
void CComponent::ChangeColourDelta(const CRGBColour &, float, bool) { fprintf(stderr, "unexpected CComponent::ChangeColourDelta\n"); abort(); }
void CComponent::UpdateColour(float) { fprintf(stderr, "unexpected CComponent::UpdateColour\n"); abort(); }
void CComponent::ResetState() { fprintf(stderr, "unexpected CComponent::ResetState\n"); abort(); }
void CComponent::ForceState(unsigned long, bool) { fprintf(stderr, "unexpected CComponent::ForceState\n"); abort(); }
void CComponent::Initialise() { fprintf(stderr, "unexpected CComponent::Initialise\n"); abort(); }
bool CComponent::HasState(unsigned long) { fprintf(stderr, "unexpected CComponent::HasState\n"); abort(); }
bool CComponent::HasStateInChildren(unsigned long) { fprintf(stderr, "unexpected CComponent::HasStateInChildren\n"); abort(); }
void CComponent::ChangeState(unsigned long) { fprintf(stderr, "unexpected CComponent::ChangeState\n"); abort(); }
void CComponent::ChangeState(unsigned long, float) { fprintf(stderr, "unexpected CComponent::ChangeState\n"); abort(); }
void CComponent::ChangeState(NUISystem::CUIState &) { fprintf(stderr, "unexpected CComponent::ChangeState\n"); abort(); }
bool CComponent::ChangedState() { fprintf(stderr, "unexpected CComponent::ChangedState\n"); abort(); }
NUISystem::CUIState * CComponent::GetCurrentState() { fprintf(stderr, "unexpected CComponent::GetCurrentState\n"); abort(); }
void CComponent::SetParent(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::SetParent\n"); abort(); }
NUISystem::CComponent * CComponent::GetParent() const { fprintf(stderr, "unexpected CComponent::GetParent\n"); abort(); }
std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > & CComponent::GetChildren() { fprintf(stderr, "unexpected CComponent::GetChildren\n"); abort(); }
float CComponent::GetUpdateTime() const { fprintf(stderr, "unexpected CComponent::GetUpdateTime\n"); abort(); }
C2DVector CComponent::GetTargetPosition() const { fprintf(stderr, "unexpected CComponent::GetTargetPosition\n"); abort(); }
CRGBColour CComponent::GetTargetColour() const { fprintf(stderr, "unexpected CComponent::GetTargetColour\n"); abort(); }
C2DVector CComponent::GetTargetZoom() const { fprintf(stderr, "unexpected CComponent::GetTargetZoom\n"); abort(); }
void CComponent::ProcessChangeState() { fprintf(stderr, "unexpected CComponent::ProcessChangeState\n"); abort(); }
void CComponent::AddChild(CCountedPointer<NUISystem::CComponent> &) { fprintf(stderr, "unexpected CComponent::AddChild\n"); abort(); }
void CComponent::AddChildDynamically(CCountedPointer<NUISystem::CComponent> &) { fprintf(stderr, "unexpected CComponent::AddChildDynamically\n"); abort(); }
void CComponent::AddChildFront(CCountedPointer<NUISystem::CComponent> &) { fprintf(stderr, "unexpected CComponent::AddChildFront\n"); abort(); }
void CComponent::SetDeletion(NUISystem::CDeletion) { fprintf(stderr, "unexpected CComponent::SetDeletion\n"); abort(); }
NUISystem::CDeletion & CComponent::GetDeletion() { fprintf(stderr, "unexpected CComponent::GetDeletion\n"); abort(); }
void CComponent::RemoveChild(unsigned long) { fprintf(stderr, "unexpected CComponent::RemoveChild\n"); abort(); }
NUISystem::EType CComponent::GetType() const { fprintf(stderr, "unexpected CComponent::GetType\n"); abort(); }
void CComponent::AddAction(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddAction\n"); abort(); }
void CComponent::AddActionOnSelected(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnSelected\n"); abort(); }
void CComponent::AddActionOnBack(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnBack\n"); abort(); }
void CComponent::AddPreAction(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddPreAction\n"); abort(); }
void CComponent::AddActionOnInactive(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnInactive\n"); abort(); }
void CComponent::AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnLeftClicked\n"); abort(); }
void CComponent::AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnLeftHeld\n"); abort(); }
void CComponent::AddActionOnRightClicked(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnRightClicked\n"); abort(); }
void CComponent::AddActionOnDropped(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnDropped\n"); abort(); }
void CComponent::AddActionOnDroppedNowhere(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnDroppedNowhere\n"); abort(); }
void CComponent::AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnDraggedUp\n"); abort(); }
void CComponent::AddActionOnDraggedDown(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnDraggedDown\n"); abort(); }
void CComponent::AddActionOnHovered(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnHovered\n"); abort(); }
void CComponent::AddActionOnUnhovered(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnUnhovered\n"); abort(); }
void CComponent::AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnLeftUnclicked\n"); abort(); }
void CComponent::Die() { fprintf(stderr, "unexpected CComponent::Die\n"); abort(); }
std::map<unsigned long,NUISystem::CUIState,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,NUISystem::CUIState> > > & CComponent::GetStates() { fprintf(stderr, "unexpected CComponent::GetStates\n"); abort(); }
void CComponent::SetName(const CCharString &) { fprintf(stderr, "unexpected CComponent::SetName\n"); abort(); }
CCharString CComponent::GetName() { fprintf(stderr, "unexpected CComponent::GetName\n"); abort(); }
NUISystem::CComponent * CComponent::FindComponent(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::FindComponent\n"); abort(); }
unsigned long CComponent::GetCurrentStateIndex() { fprintf(stderr, "unexpected CComponent::GetCurrentStateIndex\n"); abort(); }
unsigned long CComponent::GetTargetStateIndex() const { fprintf(stderr, "unexpected CComponent::GetTargetStateIndex\n"); abort(); }
float CComponent::CanScroll(bool, C2DBoxF &) { fprintf(stderr, "unexpected CComponent::CanScroll\n"); abort(); }
void CComponent::FakeChildDying(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::FakeChildDying\n"); abort(); }
void CComponent::SetParentViewport(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::SetParentViewport\n"); abort(); }
void CComponent::ClearHandle() { fprintf(stderr, "unexpected CComponent::ClearHandle\n"); abort(); }
void CComponent::MakeUnavailable() { fprintf(stderr, "unexpected CComponent::MakeUnavailable\n"); abort(); }
void CComponent::MakeUnavailableDynamically() { fprintf(stderr, "unexpected CComponent::MakeUnavailableDynamically\n"); abort(); }
void CComponent::MakeAvailable() { fprintf(stderr, "unexpected CComponent::MakeAvailable\n"); abort(); }
void CComponent::MakeAvailableDynamically() { fprintf(stderr, "unexpected CComponent::MakeAvailableDynamically\n"); abort(); }
void CComponent::AddActionIAmParamOf(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionIAmParamOf\n"); abort(); }
NUISystem::CComponent * CComponent::FindFirstChildOfTypeNonRecursive(NUISystem::EType) { fprintf(stderr, "unexpected CComponent::FindFirstChildOfTypeNonRecursive\n"); abort(); }
bool CComponent::IsAllowedToScroll() const { fprintf(stderr, "unexpected CComponent::IsAllowedToScroll\n"); abort(); }
NUISystem::CComponent * CComponent::GetParentViewport() { fprintf(stderr, "unexpected CComponent::GetParentViewport\n"); abort(); }
bool CComponent::IsBastardChild() const { fprintf(stderr, "unexpected CComponent::IsBastardChild\n"); abort(); }
bool CComponent::IsIndependent() const { fprintf(stderr, "unexpected CComponent::IsIndependent\n"); abort(); }
bool CComponent::IsPositionIndependent() const { fprintf(stderr, "unexpected CComponent::IsPositionIndependent\n"); abort(); }
bool CComponent::IsZoomIndependent() const { fprintf(stderr, "unexpected CComponent::IsZoomIndependent\n"); abort(); }
bool CComponent::IsLayerIndependent() const { fprintf(stderr, "unexpected CComponent::IsLayerIndependent\n"); abort(); }
bool CComponent::IsDrawFromViewport() const { fprintf(stderr, "unexpected CComponent::IsDrawFromViewport\n"); abort(); }
bool CComponent::IsPositionIsCenter() const { fprintf(stderr, "unexpected CComponent::IsPositionIsCenter\n"); abort(); }
long CComponent::GetDefIndex() const { fprintf(stderr, "unexpected CComponent::GetDefIndex\n"); abort(); }
CDefPointer<NUISystem::CUIDef const > CComponent::LoadAndReturnDef() const { fprintf(stderr, "unexpected CComponent::LoadAndReturnDef\n"); abort(); }
void CComponent::OnObserverRemove() { fprintf(stderr, "unexpected CComponent::OnObserverRemove\n"); abort(); }
C2DVector CComponent::CanMove(const C2DVector &) { fprintf(stderr, "unexpected CComponent::CanMove\n"); abort(); }
C2DVector CComponent::GetDynamicSize() const { fprintf(stderr, "unexpected CComponent::GetDynamicSize\n"); abort(); }
C2DVector CComponent::GetTargetRenderPosition() const { fprintf(stderr, "unexpected CComponent::GetTargetRenderPosition\n"); abort(); }
bool CComponent::IsPCStyle() const { fprintf(stderr, "unexpected CComponent::IsPCStyle\n"); abort(); }
void CComponent::SetRelativeParentPosition(C2DVector &) { fprintf(stderr, "unexpected CComponent::SetRelativeParentPosition\n"); abort(); }
void CComponent::SetRelativeParentZoom(C2DVector &) { fprintf(stderr, "unexpected CComponent::SetRelativeParentZoom\n"); abort(); }
bool CComponent::IsUsingRelativeZoom() const { fprintf(stderr, "unexpected CComponent::IsUsingRelativeZoom\n"); abort(); }
bool CComponent::IsUsingRelativePosition() const { fprintf(stderr, "unexpected CComponent::IsUsingRelativePosition\n"); abort(); }
C2DVector CComponent::GetRelativeRenderPosition() { fprintf(stderr, "unexpected CComponent::GetRelativeRenderPosition\n"); abort(); }
C2DVector CComponent::GetRelativeRenderZoom() { fprintf(stderr, "unexpected CComponent::GetRelativeRenderZoom\n"); abort(); }
void CComponent::SetRelativeRenderPosition(C2DVector) { fprintf(stderr, "unexpected CComponent::SetRelativeRenderPosition\n"); abort(); }
C2DVector CComponent::SetRelativeRenderZoom(C2DVector) { fprintf(stderr, "unexpected CComponent::SetRelativeRenderZoom\n"); abort(); }
C2DVector CComponent::GetTargetRelativeRenderPosition() const { fprintf(stderr, "unexpected CComponent::GetTargetRelativeRenderPosition\n"); abort(); }
C2DVector CComponent::GetTargetRelativeRenderZoom() const { fprintf(stderr, "unexpected CComponent::GetTargetRelativeRenderZoom\n"); abort(); }
void CComponent::AddPositionChild(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::AddPositionChild\n"); abort(); }
void CComponent::SetPositionParent(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::SetPositionParent\n"); abort(); }
NUISystem::CComponent * CComponent::GetPositionParent() { fprintf(stderr, "unexpected CComponent::GetPositionParent\n"); abort(); }
void CComponent::PositionChildDying(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::PositionChildDying\n"); abort(); }
void CComponent::PositionParentDying(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::PositionParentDying\n"); abort(); }
long CComponent::GetRenderLayer() const { fprintf(stderr, "unexpected CComponent::GetRenderLayer\n"); abort(); }
void CComponent::AddActionOnDesctruction(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnDesctruction\n"); abort(); }
void CComponent::ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > *) { fprintf(stderr, "unexpected CComponent::ExecuteActions\n"); abort(); }
bool CComponent::IsClickable() const { fprintf(stderr, "unexpected CComponent::IsClickable\n"); abort(); }
void CComponent::RemoveFromHoveredList() { fprintf(stderr, "unexpected CComponent::RemoveFromHoveredList\n"); abort(); }
void CComponent::ProcessEvent(EEvent) { fprintf(stderr, "unexpected CComponent::ProcessEvent\n"); abort(); }

class FixtureWidget : public CComponent {
public:
    FixtureWidget(const C2DVector& p, const C2DVector& size, EType type, bool centered)
        : CComponent(0), position(p), dynamic_size(size), kind(type), center(centered) {}
    C2DVector GetPosition() const { return position; }
    C2DVector GetDynamicSize() const { return dynamic_size; }
    EType GetType() const { return kind; }
    bool IsPositionIsCenter() const { return center; }
    void Append(FixtureWidget* child) { Children.push_back(CCountedPointer<CComponent>(child)); }
    void Include(unsigned long child) { ShapeChildren.push_back(child); }
    void ResetSelection() { ShapeChildren.clear(); }
private:
    C2DVector position, dynamic_size;
    EType kind;
    bool center;
};
}

static bool HasBox(const NUISystem::CComponent& c, float left, float top, float right, float bottom)
{
    const C2DBoxF b = c.GetBoundingBox();
    return b.TLX == left && b.TLY == top && b.BRX == right && b.BRY == bottom;
}

int main()
{
    using namespace NUISystem;
    FixtureWidget container(C2DVector(0, 0), C2DVector(0, 0), UI_TYPE_COMPOSITE, false);
    if (!HasBox(container, 0, 0, 0, 0)) return 1;
    container.Append(new FixtureWidget(C2DVector(10, 10), C2DVector(4, 4), UI_TYPE_MESH, false));
    if (!HasBox(container, 10, 10, 14, 14)) return 2;
    container.Append(new FixtureWidget(C2DVector(2, 20), C2DVector(6, 2), UI_TYPE_MESH, true));
    if (!HasBox(container, 2, 10, 14, 22)) return 3;
    container.Append(new FixtureWidget(C2DVector(-2, 5), C2DVector(6, 4), UI_TYPE_SPRITE, true));
    if (!HasBox(container, -5, 3, 14, 22)) return 4;
    container.Include(1);
    container.Include(2);
    if (!HasBox(container, -5, 3, 8, 22)) return 5;
    container.ResetSelection();
    container.Include(2);
    if (!HasBox(container, -2, 5, 4, 9)) return 6;
    container.ResetSelection();
    container.Include(10);
    if (!HasBox(container, 0, 0, 0, 0)) return 7;
    puts("00531090_TEST PASS");
    return 0;
}