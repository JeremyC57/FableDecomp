// CGameScriptInterface::SetCombatNearbyBreakOffRange(CScriptThing, float) -- GetThing through the counted pointer, TC 0xaa from the thing's TC map (+0x44, inlined find), store the argument at +0x64; CScriptThing destroyed inline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A54E0, oracle length 218 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; mov ecx,[thing.PImp.Data] ; push edi ; xor edi,edi ; cmp ecx,edi ; je X ; mov eax,[ecx] ; call [eax+0x2c] ; cmp eax,edi ; jne B ; X: (CScriptThing dtor: vtable store, count release, delete) ; jmp D ; B: (GetThing again) ; mov ecx,[eax+0x34] ; test flag ; je N ; push esi ; lea esi,[eax+0x44] ; lea eax,[esp+8] ; push eax ; mov ecx,esi ; mov dword ptr [esp+0Ch],0xaa ; call CVectorMap::LowerBound ; mov esi,[esi+4] ; cmp eax,esi ; je L ; cmp dword ptr [eax],0xaa ; jle F ; L: mov eax,esi ; F: mov eax,[eax+4] ; pop esi ; jmp S ; N: mov eax,[dead slot] ; S: mov ecx,[value] ; mov [eax+0x64],ecx ; (dtor) ; D: lea ecx,[esp+0Ch] ; mov [esp+14h],edi ; mov [esp+10h],edi ; call CBaseClass::~CBaseClass ; pop edi ; pop ecx ; ret 10h
// Script API entry point (virtual on CGameScriptInterface; `this` unused): the by-value
// CScriptThing resolves its thing through virtual slot 11 of the counted pointee (null
// tested, evaluated once for the guard and once for the body), the TC entry 0xaa is looked
// up in the thing's CVectorMap at +0x44 when flag 0x400 of +0x34 is set (inlined find
// over LowerBound @ 0x0040F020; when the flag is clear retail reads an uninitialised local),
// and the argument is stored at +0x64 of the TC. The CScriptThing parameter's destructor
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


template <class T> class CCountedPointer { public:
    T*  m_pData;                // +0
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

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
inline CThing* CScriptThing::GetPThing() const { return PImp.m_pData == 0 ? 0 : PImp.m_pData->GetPThing(); }

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

struct STC_64 {
    unsigned char _pad[0x64];
    float m_64;                          // +0x64 (float: the PDB setter's parameter type; retail copies the raw dword)
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetCombatNearbyBreakOffRange(CScriptThing thing, float value)
{
    if (!thing.GetPThing())
        return;                            // the parameter's destructor is inlined here too
    CThing* pThing = thing.GetPThing();
    ((STC_64*)pThing->GetTC((ETCInterfaceType)0xaa))->m_64 = value;
}