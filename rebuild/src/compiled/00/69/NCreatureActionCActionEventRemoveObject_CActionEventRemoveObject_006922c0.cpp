// NCreatureAction::CActionEventRemoveObject::CActionEventRemoveObject() -- inlined intermediate base (CBase(), a cleared flag byte at +4), own vtable, a CCharString at +0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006922C0, length 30 bytes.
// Callees: CBase::CBase @ 0x0099A2D0, CCharString::CCharString() @ 0x0099E4B0 (null data pointer,
// bumps the string-instance counter). This class's vtable is 0x0125C1A0 (rtti-vtables.tsv:
// NCreatureAction::CActionEventRemoveObject).
// Shape: the byte store at +4 sits BEFORE this class's vtable store. An init-list member of this
// class would sink the vtable store below it only partially (VC7.1 schedules the vtable store
// between such stores), so the flag belongs to an inlined intermediate base constructor whose
// own vtable store is dead (abstract: `__declspec(novtable)`, GOTCHAS). Then the vtable, then
// the name member at +0x14 is default-constructed out of line and `this` is returned.
#include "fable_string.h"

namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                                // 0x0099A2D0
}; } using N_0099a2d0::CBase;

namespace NCreatureAction {
struct __declspec(novtable) CActionEventBase : CBase {   // abstract: no vtable store of its own
    bool m_bTriggered;                      // +0x04
    CActionEventBase() : CBase(), m_bTriggered(false) { }
};

struct CActionEventRemoveObject : CActionEventBase {              // vtable 0x0125C1A0
    unsigned char _pad[0x0c];               // +0x08..+0x14 (not initialised here)
    CCharString   m_name;                   // +0x14
    virtual void v00();
    CActionEventRemoveObject();
};
}

NCreatureAction::CActionEventRemoveObject::CActionEventRemoveObject()
    : CActionEventBase()
    , m_name()
{
}