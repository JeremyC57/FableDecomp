// NUISystem::CList::AddScrollingChildAndSetOffsets(CCountedPointer<CComponent>&) -- add a scrolling child at the
//   list's next scrolling-child position / alpha: write the position into the child's states 1, 4, 5 (and 0, 3 when the
//   list def asks for it) and the alpha byte (clamped: 0, 255 or alpha * 255) into states 1, 4, 5; advance the next position by the
//   position offset and the alpha by the alpha offset, bouncing both offsets when the alpha leaves [.., 0.999); then add the
//   child, append it to the scrolling children (visible) and record its child index as a shape child
// Leaf: ?AddScrollingChildAndSetOffsets@CList@NUISystem@@UAEXAAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Retail: Fable.exe @ 0x0053AD07, 952 bytes, `ret 4`; /Os + optimize("y", off) TU (as ComputeScrollBarPosition 0x0053B0BF).
// Callees: map operator[] 0x0042D5B1, CComponent::AddChild 0x00533B50, vector push_backs 0x00486024 (CComponent*),
//   0x0053CB4F (bool), 0x005178A0 (unsigned long), _ftol 0x00BFEA70. 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 0.999f 0x012462AC,
//   255.0f 0x01230014.
// Layout: the ComputeScrollBarPosition view, plus retail = Ego_r.pdb CList - 0x3c from PositionOffset on (12-byte
//   vectors): PositionOffsetToUse +0x190, AlphaOffsetToUse +0x19c, NextScrollingChildAlpha +0x1a0,
//   NextScrollingChildPosition +0x1a4; CComponent Children +0xb0 (8-byte CCountedPointer elements), ShapeChildren +0xe4.
// NF66 (round 7): ported onto engine/NUISystem_CList.h (FABLE_RETAIL_CLIST, the define block of the landed same-TU rows
//   0x005379F5 / 0x0053BEAC): the local CComponent / CComponentDef / CDefPointer / CCountedPointer / CList and the three vector
//   stand-ins deleted. The visibility and shape-child push_backs now bind the STLport spellings ?push_back@?$vector@_NV?$allocator@_N@std@@@std@@QAEXAB_N@Z
//   (0x0053CB4F) and ?push_back@?$vector@KV?$allocator@K@std@@@std@@QAEXABK@Z (0x005178A0); the def flag at +0x221 is CUIDef::PCStyle
//   (LoadAndReturnDef, slot 108); the self is the PDB virtual (Ego_r 0x005B42E1 / FableWin 0x0219C4D4, UAE, non-const reference).
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CList.h"

namespace NUISystem {

void CList::AddScrollingChildAndSetOffsets(CCountedPointer<CComponent>& child)
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
    CComponent::AddChild(child);
    ScrollingChildren.push_back(child.operator->());
    ScrollingChildrenVisibility.push_back(true);
    ShapeChildren.push_back(Children.size() - 1);
}

}