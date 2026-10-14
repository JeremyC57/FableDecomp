// CTCNamedTargetBase_007b2720::~CTCNamedTargetBase_007b2720() -- release the counted action, the name and the target pointer, then ~CTCBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B2720, length 51 bytes.
// Callees: the counted object's virtual slot 1 (`call [eax+4]`, Delete) when the release at +0x60
// drops the count to zero, CCharString::~CCharString @ 0x0099EAE0 (the name at +0x44),
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (the target pointer at +0xc),
// CTCBase::~CTCBase @ 0x00686830 (tail jump; stores the CTCBase vtable 0x0125BE0C).
// The manifest's "~CAIStateGroup_SummonerCharge" label is propagated; the base is CTCBase (0xc
// bytes) and this class stores NO vtable of its own before destroying its members, so it is an
// abstract (`__declspec(novtable)`) intermediate component class; named by what it owns.
// Shape: members destroyed in reverse order -- the inline counted-pointer release (`dec [ecx+4] ;
// jne`, virtual Delete, null store), the string, the intelligent pointer -- then the base.
struct CCountedObject {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1: `call [eax+4]`
    long m_refCount;                            // +0x04
};

struct CCountedPointer {
    CCountedObject* m_pObject;                  // +0x00
    ~CCountedPointer()
    {
        CCountedObject* object = m_pObject;     // named: retail never re-reads the member
        if (object != 0) {
            if (--object->m_refCount == 0)
                object->Delete();
            m_pObject = 0;
        }
    }
};

struct CCharString {
    void* PStringData;
    ~CCharString();                             // 0x0099EAE0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

struct CTCBase {
    virtual void v00();
    unsigned char _pad[0x8];                    // +0x04..+0x0b
    ~CTCBase();                                 // 0x00686830
};

struct __declspec(novtable) CTCNamedTargetBase_007b2720 : CTCBase {
    CBaseIntelligentPointer m_pTarget;          // +0x0c CIntelligentPointer<CThing>
    unsigned char _pad1[0x30];                  // +0x14..+0x43
    CCharString m_name;                         // +0x44
    unsigned char _pad2[0x18];                  // +0x48..+0x5f
    CCountedPointer m_pAction;                  // +0x60 CCountedPointer<...>
    virtual void v01();
    ~CTCNamedTargetBase_007b2720();
};

CTCNamedTargetBase_007b2720::~CTCNamedTargetBase_007b2720()
{
}
