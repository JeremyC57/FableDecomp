// NCreatureAction::CActionEventAddObject::CActionEventAddObject(long a, long b, long c) -- inlined event base (CBase(), a cleared flag byte at +4), the three words at +8/+0xc/+0x10, this class's vtable, two CCharStrings at +0x14 / +0x18 default-constructed out of line, a cleared byte at +0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00695690, length 63 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0, CCharString::CCharString() @ 0x0099E4B0 twice. Vtable 0x0125C178 = NCreatureAction::CActionEventAddObject (rtti-vtables.tsv). Shape: the
// three arguments are loaded into ecx/eax/edx first, the flag byte and the three words belong to an intermediate `novtable` base (see the landed CActionEventRemoveObject @ 0x006922C0:
// they are all stored before this class's vtable, around the first string's `lea ecx`), then the vtable, the two strings, the trailing byte, and `this` is returned.
// The chunk label "CEnginePrimitiveManager::CEnginePrimitiveManager" is propagated (the 0x0069xxxx module is game code).
#include "fable_string.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

namespace NCreatureAction {
struct __declspec(novtable) CActionEventBase : CBaseClass {   // abstract: no vtable store of its own; it owns the flag and the three words
    bool m_bTriggered;                             // +0x04
    long m_a;                                      // +0x08
    long m_b;                                      // +0x0c
    long m_c;                                      // +0x10
    CActionEventBase(long a, long b, long c) : CBaseClass(), m_bTriggered(false), m_a(a), m_b(b), m_c(c) { }
};

struct CActionEventAddObject : CActionEventBase {               // vtable 0x0125C178
    CCharString m_name;                            // +0x14
    CCharString m_param;                           // +0x18
    bool        m_flag;                            // +0x1c
    virtual void v00();
    CActionEventAddObject(long a, long b, long c);
};
}

NCreatureAction::CActionEventAddObject::CActionEventAddObject(long a, long b, long c)
    : CActionEventBase(a, b, c)
    , m_name(), m_param()
    , m_flag(false)
{
}