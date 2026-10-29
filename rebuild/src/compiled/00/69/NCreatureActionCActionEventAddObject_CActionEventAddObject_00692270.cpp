// NCreatureAction::CActionEventAddObject::CActionEventAddObject() -- the CBase base constructor, the flag byte at +4 cleared, this class's vtable, then the two CCharString members at +0x14 and +0x18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692270, length 48 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0, CCharString::CCharString @ 0x0099E4B0 twice. Vtable 0x0125C178 = NCreatureAction::CActionEventAddObject (rtti-vtables.tsv; the chunk label
// "CFireheartMinigameDef" is propagated). Shape: the byte clear sits BEFORE the vtable store: it belongs to an intermediate (novtable) base whose inline constructor
// runs between CBase and the derived vtable store (a plain member initialiser of the derived class is scheduled after the vtable store); `this` is returned.
class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
};
class CBaseClass {
public:
    virtual void v00();
    CBaseClass();                                       // 0x0099A2D0
};

namespace NCreatureAction {
struct __declspec(novtable) CActionEventBase_00692270 : CBaseClass {   // the intermediate base: its inline constructor clears the flag byte at +4 (stored before the derived vtable)
    bool CActionEventBase_flag;                    // +0x04
    CActionEventBase_00692270() : CBaseClass(), CActionEventBase_flag(false) { }
};
struct CActionEventAddObject : CActionEventBase_00692270 {                 // vtable 0x0125C178
    unsigned char _pad08[0xc];                   // +0x08 .. +0x14 (not initialised)
    CCharString   m_name;                          // +0x14
    CCharString   m_text;                          // +0x18
    virtual void v00();
    CActionEventAddObject();
};
}

NCreatureAction::CActionEventAddObject::CActionEventAddObject()
    : CActionEventBase_00692270()
{
}