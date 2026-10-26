// NCreatureAction::CActionEventTriggerParticle::CActionEventTriggerParticle() -- the CBase base constructor, the flag byte at +4 cleared, this class's vtable, then the two CCharString members at +0x18 and +0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692120, length 48 bytes.
// Callees: CBase::CBase @ 0x0099A2D0, CCharString::CCharString @ 0x0099E4B0 twice. Vtable 0x0125C0D8 = NCreatureAction::CActionEventTriggerParticle (rtti-vtables.tsv; the chunk label
// "CFireheartMinigameDef" is propagated). Shape: the byte clear sits BEFORE the vtable store: it belongs to an intermediate (novtable) base whose inline constructor
// runs between CBase and the derived vtable store (a plain member initialiser of the derived class is scheduled after the vtable store); `this` is returned.
class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
};
namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2D0
}; } using N_0099a2d0::CBase;

namespace NCreatureAction {
struct __declspec(novtable) CActionEventBase_00692120 : CBase {   // the intermediate base: its inline constructor clears the flag byte at +4 (stored before the derived vtable)
    bool CActionEventBase_flag;                    // +0x04
    CActionEventBase_00692120() : CBase(), CActionEventBase_flag(false) { }
};
struct CActionEventTriggerParticle : CActionEventBase_00692120 {                 // vtable 0x0125C0D8
    unsigned char _pad08[0x10];                   // +0x08 .. +0x18 (not initialised)
    CCharString   m_name;                          // +0x18
    CCharString   m_text;                          // +0x1c
    virtual void v00();
    CActionEventTriggerParticle();
};
}

NCreatureAction::CActionEventTriggerParticle::CActionEventTriggerParticle()
    : CActionEventBase_00692120()
{
}