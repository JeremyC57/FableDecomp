// NUISystem::CList::DoRecomputeOffsets() -- recompute every scrolling child's state positions and alphas from the list's offsets and snapshot their states
// Grade: DIFFER(1111v1111) -- one store pair swapped: retail stores PositionOffsetToUse.Y (0x0053C36E) before the fstp of the negated X (0x0053C374); fchs gives the opposite order, `* -1` gives retail's order but an fmul (57 spellings)
// Leaf: ?DoRecomputeOffsets@CList@NUISystem@@MAEXXZ
// Flags: /O1 /Oy-
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053C332, 1111 bytes, plain `ret`; unaligned entry right after 0x0053C32F `leave ; ret 4`: a size-optimised
//   frame-pointer TU (xor/inc for 1, `and [mem],0`, leave) -> /O1 /Oy-, as the landed CList rows (optimize("s")).
// Callees: CList::InitialiseOffsets (slot 156, +0x270), vector<map<unsigned long,CUIState> >::erase(first,last) 0x0053CD69 (clear),
//   CComponent::GetStates (slot 82, +0x148), map<unsigned long,CUIState>::operator[] 0x0042D5B1, __ftol2 0x00BFEA70, CComponent::ChangeColour
//   (slot 38, +0x98), CComponent::ChangePosition (slot 31, +0x7C), vector<map<..> >::push_back 0x0053CFC2.
// Shape: the five position keys and the two change keys are loop locals hoisted by the guard + do-while (retail initialises them once,
//   after the `i < size()` test); the alpha keys are temporaries sharing one slot; the Y step goes through a float local (retail
//   `fld Next.Y ; fadd ToUse.Y`); the alpha test is `!(a >= 1) && !(a >= 0.999)` (test ah,1); the C2DVector reference gives the lea.
// Next idea: another negation spelling that keeps fchs but lets the scheduler sink the fstp (a by-reference inline setter, or
//   negating through C2DVector::operator- on a copy).
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
    C2DVector& to_use = PositionOffsetToUse;
    to_use = PDefPositionOffset;
    to_use.X = -to_use.X;
    PositionOffset = PDefPositionOffset;
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
