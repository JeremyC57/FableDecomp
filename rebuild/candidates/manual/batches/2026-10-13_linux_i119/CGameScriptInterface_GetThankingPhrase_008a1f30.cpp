// CGameScriptInterface::GetThankingPhrase(CScriptThing) -- 0 unless the counted pointer resolves a thing; else the dword at +0xc of TC 0xaa (thing's TC map +0x44, inlined find); CScriptThing destroyed inline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A1F30, oracle length 238 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; mov ecx,[thing.PImp.Data] ; push edi ; xor edi,edi ; cmp ecx,edi ; je X ; mov eax,[ecx] ; call [eax+0x2c] ; cmp eax,edi ; jne B ; X: (CScriptThing dtor) ; xor eax,eax ; pop edi ; pop ecx ; ret 0Ch ; B: (GetThing again) ; mov ecx,[eax+0x34] ; test flag ; push esi ; je N ; (inlined CVectorMap find of 0xaa) ; jmp S ; N: mov eax,[dead slot] ; S: mov esi,[eax+0xc] ; (dtor) ; lea ecx,[esp+10h] ; mov [esp+14h],edi ; mov [esp+18h],edi ; call CBase::~CBase ; mov eax,esi ; pop esi ; pop edi ; pop ecx ; ret 0Ch
// Script API getter (virtual on CGameScriptInterface; `this` unused): returns 0 when the
// by-value CScriptThing does not resolve a thing (virtual slot 11 of the counted pointee),
// else the dword at +0xc of TC 0xaa, looked up in the thing's CVectorMap at +0x44 when
// flag 0x400 of +0x34 is set (inlined find over LowerBound @ 0x0040F020; with the flag
// clear retail reads an uninitialised local). The result is held in esi across the inlined
// CScriptThing destructor. One of 9 bodies of this shape.
struct CThing;
struct CTCBase;

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

struct CBase {
    virtual void v00();
    ~CBase();                              // 0x0099A2E0 (sets the CBase vtable; manifest label propagated)
};

struct CScriptThing : CBase {             // 12 bytes, passed by value (vtable 0x01238C8C, rtti-vtables.tsv)
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

struct CThing {
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

struct STC_0c {
    unsigned char _pad[0xc];
    unsigned long m_0c;                 // +0xc
};

struct CGameScriptInterface {
    unsigned long GetThankingPhrase(CScriptThing thing);
};

unsigned long CGameScriptInterface::GetThankingPhrase(CScriptThing thing)
{
    if (!thing.GetThing())
        return 0;
    CThing* pThing = thing.GetThing();
    return ((STC_0c*)pThing->GetTC((ETCInterfaceType)0xaa))->m_0c;
}
