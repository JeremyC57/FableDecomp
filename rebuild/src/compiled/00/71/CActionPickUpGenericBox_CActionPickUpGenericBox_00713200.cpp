// NTCScriptedControl::CActionPickUpGenericBox::CActionPickUpGenericBox(CThing* box) -- constructor: CBase(), the common action members zeroed, own vtable, CScriptThing at +0xc from box->GetScriptThing(), frame -1 at +0x18, flag false at +0x1c
// Leaf: ??0CActionPickUpGenericBox@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00713200, length 64 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CThing::GetScriptThing @ 0x004C7E50 (by-value CScriptThing
// through the hidden result pointer, constructed straight into the member at +0xc).
// Vtable 0x012658B0 = NTCScriptedControl::CActionPickUpGenericBox (rtti-vtables.tsv). Same shape as
// the landed CActionDropGenericBox default constructor (009036a0) plus the script-thing member: the
// zero stores share ebx, the vtable store sinks to just before the GetScriptThing call, then the
// -1 frame store follows the false flag (declaration order +0x18, +0x1c; the scheduler stores the
// register zero first).
class CThing;

class CScriptThing { public:                       // engine/CScriptThing.h layout (12 bytes)
    void* __vftable;
    CThing* PImp_Data;
    void* PImp_Info;
    CScriptThing(const CScriptThing& other);
};

class CThing { public:
    CScriptThing GetScriptThing();          // 0x004C7E50
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

struct CActionPickUpGenericBox : CAction {    // vtable 0x012658B0
    CScriptThing m_box;                     // +0x0c
    long         m_frame;                   // +0x18 = -1
    bool         m_pickedUp;                // +0x1c
    CActionPickUpGenericBox(CThing* box);
};

CActionPickUpGenericBox::CActionPickUpGenericBox(CThing* box)
    : CAction()
    , m_box(box->GetScriptThing())
    , m_frame(-1)
    , m_pickedUp(false)
{
}