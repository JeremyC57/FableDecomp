// NUISystem::CList::DoRecomputeOffsets() -- recompute every scrolling child's state positions and alphas from the list's offsets and snapshot their states
// Leaf: ?DoRecomputeOffsets@CList@NUISystem@@MAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053C332, 1111 bytes, plain `ret`; unaligned entry right after 0x0053C32F `leave ; ret 4`: the
//   size-optimised frame-pointer CList TU (xor/inc for 1, `and [mem],0`, leave) -> optimize("s") + optimize("y", off), as the landed CList
//   rows (equivalently /O1 /Oy-, which also matches).
// Callees: CList::InitialiseOffsets (slot 156, +0x270), vector<map<unsigned long,CUIState> >::erase(first,last) 0x0053CD69 (clear),
//   CComponent::GetStates (slot 82, +0x148), map<unsigned long,CUIState>::operator[] 0x0042D5B1, __ftol2 0x00BFEA70, CComponent::ChangeColour
//   (slot 38, +0x98), CComponent::ChangePosition (slot 31, +0x7C), vector<map<..> >::push_back 0x0053CFC2.
// Shape: the offset set-up is the GetDataFromDef 0x0053822B order -- copy the def offset into PositionOffsetToUse, copy it into
//   PositionOffset, THEN negate PositionOffsetToUse.X -- which gives retail's `mov [ToUse.Y]` before the `fstp [ToUse.X]` (negating
//   before the PositionOffset copy, or through a C2DVector reference local, sinks the fstp ahead of the Y store). The five position
//   keys and the two change keys are loop locals hoisted by the guard + do-while (retail initialises them once, after the `i < size()`
//   test); the alpha keys are temporaries sharing one slot; the Y step goes through a float local (retail `fld Next.Y ; fadd ToUse.Y`);
//   the alpha test is `!(a >= 1) && !(a >= 0.999)` (test ah,1).
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CList.h"
namespace NUISystem {
void CList::DoRecomputeOffsets()
{
    InitialiseOffsets(ScrollingChildren.size());
    PositionOffsetToUse = PDefPositionOffset;
    PositionOffset = PDefPositionOffset;
    PositionOffsetToUse.X = -PositionOffsetToUse.X;
    AlphaOffset = AlphaOffsetToUse = PDefAlphaOffset * (1.0f / 255.0f);
    ScrollingChildrenStates.clear();
    unsigned long i = 0;
    if (i < ScrollingChildren.size()) {
    unsigned long state1 = 1, state4 = 4, state5 = 5, state0 = 0, state6 = 6;
    unsigned long colourState = 1, positionState = 1;
    do {
        CComponent* child = ScrollingChildren[i];
        child->GetStates()[state1].Position = NextScrollingChildPosition;
        child->GetStates()[state4].Position = NextScrollingChildPosition;
        child->GetStates()[state5].Position = NextScrollingChildPosition;
        child->GetStates()[state0].Position = NextScrollingChildPosition;
        child->GetStates()[state6].Position = NextScrollingChildPosition;
        if (NextScrollingChildAlpha < 0.0f) {
            child->GetStates()[1].Colour.A = 0;
            child->GetStates()[4].Colour.A = 0;
            child->GetStates()[5].Colour.A = 0;
        } else if (NextScrollingChildAlpha > 1.0f) {
            child->GetStates()[1].Colour.A = 255;
            child->GetStates()[4].Colour.A = 255;
            child->GetStates()[5].Colour.A = 255;
        } else {
            child->GetStates()[1].Colour.A = (unsigned char)(NextScrollingChildAlpha * 255.0f);
            child->GetStates()[4].Colour.A = (unsigned char)(NextScrollingChildAlpha * 255.0f);
            child->GetStates()[5].Colour.A = (unsigned char)(NextScrollingChildAlpha * 255.0f);
        }
        float y = NextScrollingChildPosition.Y + PositionOffsetToUse.Y;
        NextScrollingChildPosition.Y = y;
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
        child->ChangeColour(child->GetStates()[colourState].Colour, 0.0f, false);
        child->ChangePosition(child->GetStates()[positionState].Position, 0.0f, false);
        ScrollingChildrenStates.push_back(ScrollingChildren[i]->GetStates());
    } while (++i < ScrollingChildren.size());
    }
}
}
