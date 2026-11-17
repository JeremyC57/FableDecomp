// NUISystem::CList::GetDataFromDef() -- copy the list definition into the list: three behaviour flags, the input delay, the position offset (stored, mirrored for use) and the 0..255 alpha offset, then sort the children into scrolling (with a visible flag) and non-scrolling lists by the definition's non-scrolling index table
// Leaf: ?GetDataFromDef@CList@NUISystem@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053822B, length 377 bytes; size-optimised frame-pointer TU
//   (`#pragma optimize("s", on)`, callees use `push ebp ; mov ebp,esp` too).
// Callees: the component's virtual slot 108 LoadAndReturnDef() const (CDefPointer by hidden pointer), std::find over
//   the definition's unsigned-long index vector @ 0x0053C8F8 (`__fastcall`, value by reference, ret 4; manifest
//   "_Umove<EWeaponClass*>" is propagated), vector<CComponent*>::push_back @ 0x00486024 (manifest "Vector_PushBack"),
//   vector<bool>::push_back @ 0x0053CB4F (manifest "deque_push_back_or_allocate"), the definition's virtual slot 1
//   (delete at refcount zero). Float pool: 1/255 @ 0x01231724.
// Layout: Ego_r.pdb NUISystem::CList (576 bytes) less 0x30..0x3C bytes of debug-only container words: retail Children
//   (component +0xB0, 8-byte counted pointers), behaviour flags byte +0x12E, ScrollingChildren +0x164, NonScrollingChildren
//   +0x170, PositionOffset +0x188, PositionOffsetToUse +0x190, AlphaOffset +0x198, AlphaOffsetToUse +0x19C,
//   ScrollingChildrenVisibility (vector<bool>) +0x1C8, InputDelay +0x1EC, PDefPositionOffset +0x1F0, PDefAlphaOffset
//   +0x1F8 -- now engine/NUISystem_CList.h (FABLE_RETAIL_CLIST). The list def is packed
//   (PositionOffset at the unaligned +0x142), as in the landed CDynamicList::GetDataFromDef 0x00541390.
// Shape: the visible noinline STLport find / push_back bodies (header opt-ins below) let VC7.1 enregister the unsigned index (homed in
//   [ebp-0Ch] only around the find, sharing the word with the child temporaries and the alpha fild temp);
//   `AlphaOffset = AlphaOffsetToUse = ...` stores the second member first.
// NF66 (round 7): ported onto engine/NUISystem_CList.h / NUISystem_CUIDef.h (FABLE_RETAIL_CLIST + FABLE_RETAIL_CUIDEF, the define
//   block of the landed same-TU rows 0x005379F5 / 0x0053A0A1): the local CListDef / CDefPointer / find / ChildPointer / vector
//   models and their stand-in same-TU bodies deleted. The def fields are CUIDef's (Wrapping +0x140, Inverted +0x141, PositionOffset
//   +0x142, AlphaOffset +0x14a, Scrolling +0x14b, InputDelay +0x184, NonScrollingChildren +0x94); the flags byte is
//   CComponent::FlagsSetTwo (+0x12e: bit 3 IsWrapping, bit 4 IsScrolling, bit 5 IsInverted, the landed 0x0053CE32/41/50). The
//   same-TU STLport bodies come from FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE (push_back<CComponent*> 0x00486024,
//   push_back<bool> 0x0053CB4F) and FABLE_RETAIL_STLP_FIND_NOINLINE_BODY_FORWARDER (find 0x0053C8F8 -> __find 0x00535F5A).
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CLIST_GETDATAFROMDEF
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY_FORWARDER
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CList.h"
#include "stlport/fable_algorithm.h"

namespace NUISystem {

void CList::GetDataFromDef()
{
    CDefPointer<const CUIDef> def = LoadAndReturnDef();
    if (def->Wrapping)
        FlagsSetTwo |= 8;
    if (def->Scrolling)
        FlagsSetTwo |= 0x10;
    if (def->Inverted)
        FlagsSetTwo |= 0x20;
    InputDelay = def->InputDelay;
    PDefPositionOffset = def->PositionOffset;
    PDefAlphaOffset = def->AlphaOffset;
    PositionOffsetToUse = def->PositionOffset;
    PositionOffset = def->PositionOffset;
    PositionOffsetToUse.X = -PositionOffsetToUse.X;
    AlphaOffset = AlphaOffsetToUse = def->AlphaOffset * (1.0f / 255.0f);
    for (unsigned long i = 0; i < Children.size(); ++i) {
        if (std::find(def->NonScrollingChildren.begin(), def->NonScrollingChildren.end(), i) == def->NonScrollingChildren.end()) {
            ScrollingChildren.push_back(Children[i].Get());
            ScrollingChildrenVisibility.push_back(true);
        } else {
            NonScrollingChildren.push_back(Children[i].Get());
        }
    }
}
}
