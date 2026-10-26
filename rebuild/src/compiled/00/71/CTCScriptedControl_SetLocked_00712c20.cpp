// CTCScriptedControl::SetLocked(CBaseObject*, long) -- locks the scripted control onto a thing: the thing goes through a temporary CIntelligentPointer into the controller pointer at +0xc, then the locked flag (+0x18) is set and the lock argument (+0x1c) stored
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00712C20, length 82 bytes (+14 int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90,
// ~CBaseIntelligentPointer @ 0x00A01C10; the temporary's vtable is this module's CIntelligentPointer copy 0x0126552C.
// Caller: CScriptGameResourceObjectScriptedThing @ 0x009039D0. CTCScriptedControl::IsActive (0x004C70F0) reads the same
// byte at +0x18, which confirms the class. Shape: the landed "temporary CIntelligentPointer" family (0x0084B1C0); the second
// argument is read only after the temporary's destructor (`mov ecx,[esp+14h]`), the two stores follow the source order;
// they sit after the destructor only because the temporary lives in an inner scope.
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                  // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    ~CBaseIntelligentPointer();            // 0x00A01C10
    CBaseObject* GetPItem();               // 0x00A01B50
    void SetPItem(CBaseObject* item);      // 0x00A01B90
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
    void SetLocked(CBaseObject* thing, long lockType);
};

void CTCScriptedControl::SetLocked(CBaseObject* thing, long lockType)
{
    {
        CIntelligentPointer pointer;
        pointer.SetPItem(thing);
        m_pController.SetPItem(pointer.GetPItem());
    }
    m_locked = 1;
    m_lockType = lockType;
}