#pragma optimize("s", on)
// CQuickAccessItemButtonGuiDef::Copy(const CDefClassBase*) -- the virtual definition copy: the base class's Copy, then ButtonGraphic, Position and Offset taken from the source definition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046EF87, length 49 bytes.
// Callees: CParentDefClassBase::operator= @ 0x00431ED0 (??4CParentDefClassBase@@QAEAAV0@ABV0@@Z: the def chain has no
// CParentDefClassBase::Copy; slot 19 is pure there), called with the source pointer (the landed CObjectFamilyDef::Copy
// @ 0x004611D6 calls it the same way).
// Vtable 0x012360A4 slot 19 is this body (rtti-vtables.tsv). Layout as the copy constructor @ 0x0046EE2D:
// ButtonGraphic +0x3c, Position +0x40, Offset +0x48 (PDB +0x40/+0x44/+0x4c over a 4-byte smaller retail base).
// Shape: a size-optimised TU; the qualified base call is direct, every dword moves through eax and nothing is
// returned (eax is left holding the last word: a `void` virtual, not an operator=).
class C2DVector { public:
    float X, Y;
};


// Layout: the def chain is the retail one from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase, vtable 0x01230CBC,
//   0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98, operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

class CQuickAccessItemButtonGuiDef : public CParentDefClassBase { public:   // vtable 0x012360A4, retail sizeof 0x50
    long      ButtonGraphic;                    // +0x3c
    C2DVector Position;                         // +0x40
    C2DVector Offset;                           // +0x48
    virtual void Copy(const CDefClassBase* other);
};

void CQuickAccessItemButtonGuiDef::Copy(const CDefClassBase* other)
{
    const CQuickAccessItemButtonGuiDef* source = static_cast<const CQuickAccessItemButtonGuiDef*>(other);
    CParentDefClassBase::operator=(*source);
    ButtonGraphic = source->ButtonGraphic;
    Position = source->Position;
    Offset = source->Offset;
}