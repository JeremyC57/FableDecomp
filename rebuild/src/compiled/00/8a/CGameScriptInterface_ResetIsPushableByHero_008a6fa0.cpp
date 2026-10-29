// CGameScriptInterface::ResetIsPushableByHero(CScriptThing) -- GetThing through the counted pointer, TC 0xaa from the thing's TC map (+0x44, inlined find), reset +0x79 from the TC's def (+0x80 -> +0x95); CScriptThing destroyed inline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A6FA0, oracle length 229 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; mov ecx,[thing.PImp.Data] ; push edi ; xor edi,edi ; cmp ecx,edi ; je X ; mov eax,[ecx] ; call [eax+0x2c] ; cmp eax,edi ; jne B ; X: (CScriptThing dtor) ; jmp D ; B: (GetThing again) ; mov ecx,[eax+0x34] ; test flag ; je N ; (inlined CVectorMap find of 0xaa) ; jmp S ; N: mov eax,[dead slot] ; S: mov ecx,[eax+0x80] ; mov edx,[ecx+0x95] ; mov [eax+0x79],edx ; (dtor) ; D: lea ecx,[esp+0Ch] ; mov [esp+14h],edi ; mov [esp+10h],edi ; call CBaseClass::~CBaseClass ; pop edi ; pop ecx ; ret 0Ch
// Script API entry point (virtual on CGameScriptInterface; `this` unused): the by-value
// CScriptThing resolves its thing through virtual slot 11 of the counted pointee, the TC
// entry 0xaa is looked up in the thing's CVectorMap at +0x44 when flag 0x400 of +0x34
// is set (inlined find over LowerBound @ 0x0040F020; with the flag clear retail reads an
// uninitialised local), and the TC member at +0x79 is reset from its def: the object at
// +0x80 of the TC supplies +0x95. The CScriptThing destructor is inlined on both paths.
// One of 11 bodies of this shape.
class CThing;
class CTCBase;

enum ETCInterfaceType { TC_FIRST = 0 };

struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};

struct CScriptEntity {
    virtual void _slot00();
    virtual void _slot01();
    virtual void _slot02();
    virtual void _slot03();
    virtual void _slot04();
    virtual void _slot05();
    virtual void _slot06();
    virtual void _slot07();
    virtual void _slot08();
    virtual void _slot09();
    virtual void _slot10();
    virtual CThing* GetThing();            // slot 11 (vtable offset 0x2c)
};

struct CCountedPointer {
    CScriptEntity* m_pData;                // +0
    SCountBlock* m_pInfo;                  // +4
    ~CCountedPointer()
    {
        if (m_pInfo) {
            --m_pInfo->m_references;
            if (m_pInfo->m_references == 0) {
                m_pInfo->m_pfnDestroy(m_pInfo->m_pObject);
                delete m_pInfo;
            }
        }
        m_pData = 0;
        m_pInfo = 0;
    }
};

class CBaseClass {
public:
    virtual ~CBaseClass();                              // 0x0099A2E0 (sets the CBase vtable; manifest label propagated)
};

class CScriptThing : public CBaseClass { public:             // 12 bytes, passed by value (vtable 0x01238C8C, rtti-vtables.tsv)
    CCountedPointer PImp;                  // +4 Data / +8 Info
    ~CScriptThing() { }
    CThing* GetThing() const { return PImp.m_pData == 0 ? 0 : PImp.m_pData->GetThing(); }
};

struct SPair {
    long first;                            // ETCInterfaceType
    CTCBase* second;
};

struct CVectorMap {
    SPair* first;                          // +0
    SPair* last;                           // +4
    SPair* end;                            // +8
    SPair* LowerBound(const long& key);    // 0x0040F020
    SPair* Find(ETCInterfaceType key)      // the enum converts to LowerBound's long reference through a frame temporary; the compare keeps the constant
    {
        SPair* it = LowerBound(key);
        if (it == last || key < it->first)
            it = last;
        return it;
    }
};

class CThing { public:
    unsigned char _pad[0x34];
    unsigned long m_34;                 // +0x34 flags
    unsigned char _pad2[0xc];
    CVectorMap m_44;                    // +0x44 TC map
    CTCBase* GetTC(ETCInterfaceType type)
    {
        CTCBase* tc;                       // deliberately left unset when the map is absent (retail reads the dead slot)
        if (m_34 & 0x400)
            tc = m_44.Find(type)->second;
        return tc;
    }
};

struct STCDef_95 {
    unsigned char _pad[0x95];
    unsigned char m_95;              // +0x95
};

struct STC_79_80 {
    unsigned char _pad[0x79];
    unsigned char m_79;              // +0x79
    unsigned char _pad2[0x6];
    STCDef_95* m_80;                 // +0x80 def
};

class CGameScriptInterface { public:
    void ResetIsPushableByHero(CScriptThing thing);
};

void CGameScriptInterface::ResetIsPushableByHero(CScriptThing thing)
{
    if (!thing.GetThing())
        return;                            // the parameter's destructor is inlined here too
    CThing* pThing = thing.GetThing();
    STC_79_80* tc = (STC_79_80*)pThing->GetTC((ETCInterfaceType)0xaa);
    tc->m_79 = tc->m_80->m_95;
}