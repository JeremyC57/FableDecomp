// CCreatureAction_JackScream::CCreatureAction_JackScream(const CCreatureAction_JackScream& other) -- copy constructor: CCombatActionBase copy, own vtable, add-ref copy of PModeCountedPointer
// Leaf: ??0CCreatureAction_JackScream
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009775F0, length 57 bytes.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0.
// Vtable: 0x01297CB4 (CCreatureAction_JackScream, rtti-vtables.tsv). Called from the landed Clone
// @ 0x009775C0 (`new T(*this)`, sizeof 0x11c).
// Shape: `other` in edi pushed for the base copy constructor; the derived vtable store; then the one
// derived member, PModeCountedPointer (CCountedPointer<NCreatureMode::CModeJackOfBladesScream>, PDB
// name), is copied by its inline copy constructor with the count block add-ref'd in place.
// Layout: retail places the counted pointer at +0x114 (engine/CCreatureAction_JackScream.h, from the
// PDB, has it at +0x12c with sizeof 0x134: the retail CCombatActionBase is 0x114 bytes), so the header
// disagrees with the bytes and a local model with the retail offsets is used.
// The manifest label "CScriptGameResourceObjectScriptedThingBase" is propagated (the vtable names the class).
struct CCPPointerInfo {
    long m_count;                                     // +0x00 shared reference count
};

struct CModeJackOfBladesScream;

struct CCountedPointer_CModeJackOfBladesScream {
    CModeJackOfBladesScream* Data;
    CCPPointerInfo*          Info;
    CCountedPointer_CModeJackOfBladesScream(const CCountedPointer_CModeJackOfBladesScream& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _body[0x114 - 4];                   // retail sizeof(CCombatActionBase) = 0x114
    CCombatActionBase(const CCombatActionBase& other);         // 0x0062F3B0
};

struct CCreatureAction_JackScream : CCombatActionBase {
    CCountedPointer_CModeJackOfBladesScream PModeCountedPointer;   // +0x114
    virtual void v00();
    CCreatureAction_JackScream(const CCreatureAction_JackScream& other);
};

CCreatureAction_JackScream::CCreatureAction_JackScream(const CCreatureAction_JackScream& other)
    : CCombatActionBase(other), PModeCountedPointer(other.PModeCountedPointer)
{
}
