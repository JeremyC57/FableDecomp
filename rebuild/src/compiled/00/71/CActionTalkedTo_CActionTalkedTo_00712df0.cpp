// NTCScriptedControl::CActionTalkedTo::CActionTalkedTo(CThing* talker, CBaseObject* target) -- constructor: CBase(), the common action members zeroed, own vtable, CIntelligentPointer at +0xc set to target, CScriptThing at +0x14 from talker->GetScriptThing()
// Leaf: ??0CActionTalkedTo@
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00712DF0, length 80 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90, CThing::GetScriptThing @ 0x004C7E50 (by-value
// CScriptThing through the hidden result pointer, built straight into the member at +0x14).
// Vtables: 0x0126580C = NTCScriptedControl::CActionTalkedTo, 0x01265804 = CIntelligentPointer
// (rtti-vtables.tsv). Shape: base constructor, zero stores (+4, +8, +9) sharing eax, the action's
// vtable store sunk to just before the member constructors; the intelligent pointer at +0xc is the
// inlined family-44 shape (base ctor, its vtable, SetPItem(target)); then GetScriptThing on the
// first argument with &m_talker pushed (ret 8).
class CThing;
class CBaseObject;

#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

class CThing { public:
    CScriptThing GetScriptThing() const;          // 0x004C7E50
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

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                // 0x0099A2F0
};

struct __declspec(novtable) CAction : CBaseClassNonCopyable {   // NTCScriptedControl::CAction: abstract, its own vtable store is dead
    CThing* m_pThing;                       // +0x04
    bool    m_active;                       // +0x08
    bool    m_finished;                     // +0x09
    CAction() : CBaseClassNonCopyable(), m_pThing(0), m_active(false), m_finished(false) { }
};

struct CActionTalkedTo : CAction {            // vtable 0x0126580C
    CIntelligentPointer<NTCScriptedControl::CScriptConversation> m_target;           // +0x0c
    CScriptThing        m_talker;           // +0x14
    CActionTalkedTo(CThing* talker, CBaseObject* target);
};

CActionTalkedTo::CActionTalkedTo(CThing* talker, CBaseObject* target)
    : CAction()
    , m_target(target)
    , m_talker(talker->GetScriptThing())
{
}