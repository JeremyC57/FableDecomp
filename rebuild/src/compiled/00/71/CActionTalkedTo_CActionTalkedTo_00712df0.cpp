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
class CThing;
class CBaseObject;

class CScriptThing { public:                       // engine/CScriptThing.h layout (12 bytes)
    void* __vftable;
    CThing* PImp_Data;
    void* PImp_Info;
    CScriptThing(const CScriptThing& other);
};

class CThing { public:
    CScriptThing GetScriptThing();          // 0x004C7E50
};

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

namespace NTCScriptedControl { class CScriptConversation; }
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01265804
public:
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
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
    CIntelligentPointer<NTCScriptedControl::CScriptConversation> m_target;           // +0x0c
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