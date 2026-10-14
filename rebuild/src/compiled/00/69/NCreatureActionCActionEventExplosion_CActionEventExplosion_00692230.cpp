// NCreatureAction::CActionEventExplosion::CActionEventExplosion() -- inlined intermediate base (CBase(), a cleared flag byte at +4), own vtable, a CCharString at +0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692230, length 30 bytes.
// Callees: CBase::CBase @ 0x0099A2D0, CCharString::CCharString() @ 0x0099E4B0 (null data pointer,
// bumps the string-instance counter). This class's vtable is 0x0125C154 (rtti-vtables.tsv:
// NCreatureAction::CActionEventExplosion).
// Shape: the byte store at +4 sits BEFORE this class's vtable store. An init-list member of this
// class would sink the vtable store below it only partially (VC7.1 schedules the vtable store
// between such stores), so the flag belongs to an inlined intermediate base constructor whose
// own vtable store is dead (abstract: `__declspec(novtable)`, GOTCHAS). Then the vtable, then
// the name member at +0x14 is default-constructed out of line and `this` is returned.
#include "fable_string.h"

struct CBase {
    virtual void v00();
    CBase();                                // 0x0099A2D0
};

namespace NCreatureAction {
struct __declspec(novtable) CActionEventBase : CBase {   // abstract: no vtable store of its own
    bool m_bTriggered;                      // +0x04
    CActionEventBase() : CBase(), m_bTriggered(false) { }
};

struct CActionEventExplosion : CActionEventBase {              // vtable 0x0125C154
    unsigned char _pad[0x0c];               // +0x08..+0x14 (not initialised here)
    CCharString   m_name;                   // +0x14
    virtual void v00();
    CActionEventExplosion();
};
}

NCreatureAction::CActionEventExplosion::CActionEventExplosion()
    : CActionEventBase()
    , m_name()
{
}