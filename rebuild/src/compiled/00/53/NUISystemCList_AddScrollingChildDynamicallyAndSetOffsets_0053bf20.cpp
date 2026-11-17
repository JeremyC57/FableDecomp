// NUISystem::CList::AddScrollingChildDynamicallyAndSetOffsets(const CCountedPointer<CComponent>& child) -- add a scrolling child at run time at the list's next scrolling-child position / alpha: write the position into the child's states 1, 4, 5 (and 0, 3 when the list def asks for it) and the clamped alpha byte into states 1, 4, 5, advance the next position / alpha (bouncing the offsets), then the base dynamic add, the scrolling-children, visibility and scroll-layout records, and -- while the list is live (slot 113) -- focus the child on the front end for scroll types 3 / 5 and record its child index as a shape child
// Leaf: ?AddScrollingChildDynamicallyAndSetOffsets@CList@NUISystem@@UAEXAAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Retail: Fable.exe @ 0x0053BF20, 1042 bytes, `ret 4`; /Os + optimize("y", off) TU.
// Callees: map operator[] 0x0042D5B1, CChangingStateComponent::AddChildDynamically 0x0052CAB0 (direct base call), vector
//   push_backs 0x00486024 (CComponent*), 0x0053CB4F (bool), 0x005178A0 (unsigned long), the scroll layout's AddChild
//   0x0053CFC2, NUISystem::CManager::GetInstance 0x0041E5F2 (+ slot 2), _ftol 0x00BFEA70. 0.0f 0x0122DEDC,
//   1.0f 0x0122DED8, 0.999f 0x012462AC, 255.0f 0x01230014.
// Layout: the landed sibling NUISystem::CList::AddScrollingChildAndSetOffsets 0x0053AD07 (its whole first half is this
//   function's), plus the landed CList::AddScrollingChildDynamically 0x0053BEAC (+0x17c scroll layout); the scroll type
//   +0x148 is descriptive. The child's event-listener base sits at +4 (`neg/sbb/and` null-checked conversion for the
//   front-end argument, unchecked `lea ecx,[edi+4]` for its own slot-3 call).
// NF66 (round 7): ported onto engine/NUISystem_CList.h exactly as its landed sibling 0x0053AD07 (same define block, plus
//   engine/NUISystem_CManager.h): the local CComponent / CComponentDef / CDefPointer / CCountedPointer / CList / vector / scroll-layout
//   stand-ins deleted. Names from the header: the scroll layout is ScrollingChildrenStates (push_back 0x0053CFC2), IsLive is CComponent
//   slot 113 IsPCStyle, ScrollType +0x148 is CChangingStateComponent::TargetState, SetFocusListener is CObservable slot 2 AddObserver,
//   HandleEvent is CObserver slot 3 ObserveEvent(UI_EVENT_MOUSE_MOVEMENT = 0x19). The self is the PDB virtual (UAE, non-const reference).
#include "fable_compiler.h"
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"

namespace NUISystem {

void CList::AddScrollingChildDynamicallyAndSetOffsets(CCountedPointer<CComponent>& child)
{
    CComponent* c;
    c = child.operator->();
    c->GetStates()[1].Position = NextScrollingChildPosition;
    c = child.operator->();
    c->GetStates()[4].Position = NextScrollingChildPosition;
    c = child.operator->();
    c->GetStates()[5].Position = NextScrollingChildPosition;
    if (LoadAndReturnDef()->PCStyle) {
        c = child.operator->();
        c->GetStates()[0].Position = NextScrollingChildPosition;
        c = child.operator->();
        c->GetStates()[3].Position = NextScrollingChildPosition;
    }
    if (NextScrollingChildAlpha < 0.0f) {
        child->GetStates()[1].Colour.A = 0;
        c = child.operator->();
        c->GetStates()[4].Colour.A = 0;
        c = child.operator->();
        c->GetStates()[5].Colour.A = 0;
    } else if (NextScrollingChildAlpha > 1.0f) {
        child->GetStates()[1].Colour.A = 0xff;
        c = child.operator->();
        c->GetStates()[4].Colour.A = 0xff;
        c = child.operator->();
        c->GetStates()[5].Colour.A = 0xff;
    } else {
        child->GetStates()[1].Colour.A = (unsigned char)(NextScrollingChildAlpha * 255.0f);
        child->GetStates()[4].Colour.A = (unsigned char)(NextScrollingChildAlpha * 255.0f);
        child->GetStates()[5].Colour.A = (unsigned char)(NextScrollingChildAlpha * 255.0f);
    }
    NextScrollingChildPosition.Y += PositionOffsetToUse.Y;
    float alpha = AlphaOffsetToUse + NextScrollingChildAlpha;
    if (!(alpha >= 1.0f) && !(alpha >= 0.999f)) {
        NextScrollingChildAlpha = alpha;
        NextScrollingChildPosition.X += PositionOffsetToUse.X;
    } else {
        AlphaOffsetToUse = -AlphaOffsetToUse;
        PositionOffsetToUse.X = -PositionOffsetToUse.X;
        if (NextScrollingChildAlpha < 1.0f) {
            NextScrollingChildPosition.X = 0.0f;
            NextScrollingChildAlpha = 1.0f;
        } else {
            NextScrollingChildPosition.X += PositionOffsetToUse.X;
            NextScrollingChildAlpha += AlphaOffsetToUse;
        }
    }
    CChangingStateComponent::AddChildDynamically(child);
    ScrollingChildren.push_back(child.operator->());
    ScrollingChildrenVisibility.push_back(true);
    ScrollingChildrenStates.push_back(child->GetStates());
    if (IsPCStyle()) {
        if (TargetState == 3 || TargetState == 5) {
            CManager::GetInstance()->AddObserver(child.operator->());
            child->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        }
        ShapeChildren.push_back(Children.size() - 1);
    }
}

}