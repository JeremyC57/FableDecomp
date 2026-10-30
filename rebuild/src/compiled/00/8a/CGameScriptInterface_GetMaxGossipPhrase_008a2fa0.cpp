// CGameScriptInterface::GetMaxGossipPhrase(CScriptThing) -- 0 unless the counted pointer resolves a thing; else the dword at +0x2c of TC 0xaa (thing's TC map +0x44, inlined find); CScriptThing destroyed inline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A2FA0, oracle length 238 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; mov ecx,[thing.PImp.Data] ; push edi ; xor edi,edi ; cmp ecx,edi ; je X ; mov eax,[ecx] ; call [eax+0x2c] ; cmp eax,edi ; jne B ; X: (CScriptThing dtor) ; xor eax,eax ; pop edi ; pop ecx ; ret 0Ch ; B: (GetThing again) ; mov ecx,[eax+0x34] ; test flag ; push esi ; je N ; (inlined CVectorMap find of 0xaa) ; jmp S ; N: mov eax,[dead slot] ; S: mov esi,[eax+0x2c] ; (dtor) ; lea ecx,[esp+10h] ; mov [esp+14h],edi ; mov [esp+18h],edi ; call CBaseClass::~CBaseClass ; mov eax,esi ; pop esi ; pop edi ; pop ecx ; ret 0Ch
// Script API getter (virtual on CGameScriptInterface; `this` unused): returns 0 when the
// by-value CScriptThing does not resolve a thing (virtual slot 11 of the counted pointee),
// else the dword at +0x2c of TC 0xaa, looked up in the thing's CVectorMap at +0x44 when
// flag 0x400 of +0x34 is set (inlined find over LowerBound @ 0x0040F020; with the flag
// clear retail reads an uninitialised local). The result is held in esi across the inlined
// CScriptThing destructor. One of 9 bodies of this shape.
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

struct STC_2c {
    unsigned char _pad[0x2c];
    unsigned long m_2c;                 // +0x2c
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

long CGameScriptInterface::GetMaxGossipPhrase(CScriptThing thing)
{
    if (!thing.GetPThing())
        return 0;
    CThing* pThing = thing.GetPThing();
    return ((STC_2c*)pThing->GetTC((ETCInterfaceType)0xaa))->m_2c;
}