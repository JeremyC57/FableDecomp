// NCreatureAction::CActionEventTriggerParticle::CActionEventTriggerParticle(long a, long b, long c) -- inlined event base (CBase(), a cleared flag byte at +4), the three words at +8/+0xc/+0x10, this class's vtable, two CCharStrings at +0x18 / +0x1c default-constructed out of line, a cleared byte at +0x21
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00695400, length 63 bytes.
// Callees: CBase::CBase @ 0x0099A2D0, CCharString::CCharString() @ 0x0099E4B0 twice. Vtable 0x0125C0D8 = NCreatureAction::CActionEventTriggerParticle (rtti-vtables.tsv). Shape: the
// three arguments are loaded into ecx/eax/edx first, the flag byte and the three words belong to an intermediate `novtable` base (see the landed CActionEventRemoveObject @ 0x006922C0:
// they are all stored before this class's vtable, around the first string's `lea ecx`), then the vtable, the two strings, the trailing byte, and `this` is returned.
// The chunk label "CEnginePrimitiveManager::CEnginePrimitiveManager" is propagated (the 0x0069xxxx module is game code).
#include "fable_string.h"

struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2D0
};

namespace NCreatureAction {
struct __declspec(novtable) CActionEventBase : CBase {   // abstract: no vtable store of its own; it owns the flag and the three words
    bool m_bTriggered;                             // +0x04
    long m_a;                                      // +0x08
    long m_b;                                      // +0x0c
    long m_c;                                      // +0x10
    CActionEventBase(long a, long b, long c) : CBase(), m_bTriggered(false), m_a(a), m_b(b), m_c(c) { }
};

struct CActionEventTriggerParticle : CActionEventBase {               // vtable 0x0125C0D8
    long        m_d;                               // +0x14 (not initialised here)
    CCharString m_name;                            // +0x18
    CCharString m_param;                           // +0x1c
    bool        m_flag0;                           // +0x20 (not initialised here)
    bool        m_flag;                            // +0x21
    virtual void v00();
    CActionEventTriggerParticle(long a, long b, long c);
};
}

NCreatureAction::CActionEventTriggerParticle::CActionEventTriggerParticle(long a, long b, long c)
    : CActionEventBase(a, b, c)
    , m_name(), m_param()
    , m_flag(false)
{
}
