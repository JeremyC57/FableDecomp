// CGameScriptInterface::SetTargetingDistanceOffset(CScriptThing, long) -- GetThing through the counted pointer, TC 0xaa from the thing's TC map (+0x44, inlined find), store the argument at +0x7c; CScriptThing destroyed inline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A7610, oracle length 218 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; mov ecx,[thing.PImp.Data] ; push edi ; xor edi,edi ; cmp ecx,edi ; je X ; mov eax,[ecx] ; call [eax+0x2c] ; cmp eax,edi ; jne B ; X: (CScriptThing dtor: vtable store, count release, delete) ; jmp D ; B: (GetThing again) ; mov ecx,[eax+0x34] ; test flag ; je N ; push esi ; lea esi,[eax+0x44] ; lea eax,[esp+8] ; push eax ; mov ecx,esi ; mov dword ptr [esp+0Ch],0xaa ; call CVectorMap::LowerBound ; mov esi,[esi+4] ; cmp eax,esi ; je L ; cmp dword ptr [eax],0xaa ; jle F ; L: mov eax,esi ; F: mov eax,[eax+4] ; pop esi ; jmp S ; N: mov eax,[dead slot] ; S: mov ecx,[value] ; mov [eax+0x7c],ecx ; (dtor) ; D: lea ecx,[esp+0Ch] ; mov [esp+14h],edi ; mov [esp+10h],edi ; call CBaseClass::~CBaseClass ; pop edi ; pop ecx ; ret 10h
// Script API entry point (virtual on CGameScriptInterface; `this` unused): the by-value
// CScriptThing resolves its thing through virtual slot 11 of the counted pointee (null
// tested, evaluated once for the guard and once for the body), the TC entry 0xaa is looked
// up in the thing's CVectorMap at +0x44 when flag 0x400 of +0x34 is set (inlined find
// over LowerBound @ 0x0040F020; when the flag is clear retail reads an uninitialised local),
// and the argument is stored at +0x7c of the TC. The CScriptThing parameter's destructor
// is inlined on both paths (vtable store, count-block release through the flag-tested
// operator delete, fields nulled, then CBaseClass::~CBaseClass @ 0x0099A2E0). One of 28 bodies.
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

namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> SPair;

template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    SPair* first;                          // +0
    SPair* last;                           // +4
    SPair* end;                            // +8
    SPair* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
    SPair* Find(ETCInterfaceType key)      // the enum converts to LowerBound's long reference through a frame temporary; the compare keeps the constant
    {
        SPair* it = LowerBound((ETCInterfaceType)(long)key);
        if (it == last || key < it->first)
            it = last;
        return it;
    }
};

class CThing { public:
    unsigned char _pad[0x34];
    unsigned long m_34;                 // +0x34 flags
    unsigned char _pad2[0xc];
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > m_44;                    // +0x44 TC map
    CTCBase* GetTC(ETCInterfaceType type)
    {
        CTCBase* tc;                       // deliberately left unset when the map is absent (retail reads the dead slot)
        if (m_34 & 0x400)
            tc = m_44.Find(type)->second;
        return tc;
    }
};

struct STC_7c {
    unsigned char _pad[0x7c];
    long m_7c;                          // +0x7c
};

class CGameScriptInterface { public:
    void SetTargetingDistanceOffset(CScriptThing thing, long value);
};

void CGameScriptInterface::SetTargetingDistanceOffset(CScriptThing thing, long value)
{
    if (!thing.GetThing())
        return;                            // the parameter's destructor is inlined here too
    CThing* pThing = thing.GetThing();
    ((STC_7c*)pThing->GetTC((ETCInterfaceType)0xaa))->m_7c = value;
}