// CTCScriptedControl::ClearLocked() -- unlocks the scripted control: a temporary CIntelligentPointer set to null is copied into the controller pointer at +0xc and the locked flag (+0x18) is cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00712C80, length 70 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90,
// ~CBaseIntelligentPointer @ 0x00A01C10; the temporary's vtable is this module's CIntelligentPointer copy 0x0126552C.
// Caller: ~CScriptGameResourceObjectScriptedThing @ 0x00903AC0. CTCScriptedControl::IsActive (0x004C70F0) reads the same
// byte at +0x18, which confirms the class. Shape: the landed "temporary CIntelligentPointer" family (0x0084B1C0);
// SetPItem(0) pushes the literal; the flag store sits after the temporary's destructor only when the temporary lives in an
// inner scope (at function scope VC7.1 schedules the store before the destructor call); a plain `ret` (no arguments).
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                  // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    ~CBaseIntelligentPointer();            // 0x00A01C10
    void SetPItem(CBaseObject* item);      // 0x00A01B90
public:
    CBaseObject* GetPItem() const;   // 0x00A01B50
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CTCScriptedControl { public:
    unsigned char _pad[0xc];
    CBaseIntelligentPointer m_pController;   // +0xc
    unsigned char _pad14[4];
    unsigned char m_locked;                  // +0x18
    unsigned char _pad19[3];
    long m_lockType;                         // +0x1c
    void ClearLocked();
};

void CTCScriptedControl::ClearLocked()
{
    {
        CIntelligentPointer pointer;
        pointer.SetPItem(0);
        m_pController.SetPItem(pointer.GetPItem());
    }
    m_locked = 0;
}