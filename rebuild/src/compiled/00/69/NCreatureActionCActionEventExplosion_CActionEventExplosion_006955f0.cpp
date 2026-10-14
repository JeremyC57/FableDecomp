// NCreatureAction::CActionEventExplosion::CActionEventExplosion(long, long, long) -- inlined intermediate base taking
// the three arguments, own vtable, a CCharString at +0x14, then -1 / false / false.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006955F0, length 68 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CBase::CBase @ 0x0099A2D0, CCharString::CCharString() @ 0x0099E4B0. Vtable 0x0125C154 (rtti-vtables:
// NCreatureAction::CActionEventExplosion; same class as the landed default constructor @ 0x00692230). The manifest
// label "CAIStateGroup_OpinionReactionRoot ctor" is propagated and rejected.
// Shape: the flag byte at +4 and the three argument stores (+8, +0xc, +0x10) all sit BEFORE this class's vtable
// store, so they belong to an inlined `__declspec(novtable)` intermediate base constructor (its own vtable store is
// dead); then the vtable, the name member default-constructed out of line, and the body: +0x18 = -1, +0x1c and
// +0x1d cleared.
struct CCharString {
    void* m_pStringData;
    CCharString();                              // 0x0099E4B0
};

struct CBase {
    virtual void v00();
    CBase();                                    // 0x0099A2D0
};

namespace NCreatureAction {
struct __declspec(novtable) CActionEventBase : CBase {   // abstract: no vtable store of its own
    bool m_bTriggered;                          // +0x04
    long m_actionId;                            // +0x08
    long m_eventType;                           // +0x0c
    long m_param;                               // +0x10
    CActionEventBase(long actionId, long eventType, long param)
        : CBase(), m_bTriggered(false), m_actionId(actionId), m_eventType(eventType), m_param(param) { }
};

struct CActionEventExplosion : CActionEventBase {          // vtable 0x0125C154
    CCharString m_name;                         // +0x14
    long m_index;                               // +0x18
    bool m_flag1c;                              // +0x1c
    bool m_flag1d;                              // +0x1d
    virtual void v00();
    CActionEventExplosion(long actionId, long eventType, long param);
};
}

NCreatureAction::CActionEventExplosion::CActionEventExplosion(long actionId, long eventType, long param)
    : CActionEventBase(actionId, eventType, param)
    , m_name()
{
    m_index = -1;
    m_flag1c = false;
    m_flag1d = false;
}