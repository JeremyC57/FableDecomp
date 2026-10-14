// NTCScriptedControl::CActionTalkedTo::CActionTalkedTo(CThing* talker, CBaseObject* target) -- constructor: CBase(), the common action members zeroed, own vtable, CIntelligentPointer at +0xc set to target, CScriptThing at +0x14 from talker->GetScriptThing()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00712DF0, length 80 bytes.
// Callees: CBase::CBase @ 0x0099A2F0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90, CThing::GetScriptThing @ 0x004C7E50 (by-value
// CScriptThing through the hidden result pointer, built straight into the member at +0x14).
// Vtables: 0x0126580C = NTCScriptedControl::CActionTalkedTo, 0x01265804 = CIntelligentPointer
// (rtti-vtables.tsv). Shape: base constructor, zero stores (+4, +8, +9) sharing eax, the action's
// vtable store sunk to just before the member constructors; the intelligent pointer at +0xc is the
// inlined family-44 shape (base ctor, its vtable, SetPItem(target)); then GetScriptThing on the
// first argument with &m_talker pushed (ret 8).
struct CThing;
struct CBaseObject;

struct CScriptThing {                       // engine/CScriptThing.h layout (12 bytes)
    void* __vftable;
    CThing* PImp_Data;
    void* PImp_Info;
    CScriptThing(const CScriptThing& other);
};

struct CThing {
    CScriptThing GetScriptThing();          // 0x004C7E50
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PItem;                     // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01265804
    virtual void v00();
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct CBase {
    virtual void v00();
    CBase();                                // 0x0099A2F0
};

struct __declspec(novtable) CAction : CBase {   // NTCScriptedControl::CAction: abstract, its own vtable store is dead
    CThing* m_pThing;                       // +0x04
    bool    m_active;                       // +0x08
    bool    m_finished;                     // +0x09
    CAction() : CBase(), m_pThing(0), m_active(false), m_finished(false) { }
};

struct CActionTalkedTo : CAction {            // vtable 0x0126580C
    CIntelligentPointer m_target;           // +0x0c
    CScriptThing        m_talker;           // +0x14
    virtual void v00();
    CActionTalkedTo(CThing* talker, CBaseObject* target);
};

CActionTalkedTo::CActionTalkedTo(CThing* talker, CBaseObject* target)
    : CAction()
    , m_target(target)
    , m_talker(talker->GetScriptThing())
{
}