// CScriptGameResourceObjectScriptedThing::Speak(const CScriptThing& listener, const char* textGroup, ETextGroupSelectionMethod method, bool b1, bool b2, bool b3) -- queue a talk-to action speaking from the named text group at a live listener for the live scripted thing
// Leaf: Speak
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00904780, length 213 bytes (`ret 18h`), vtable 0x0128D86C slot 13.
// PDB: ?Speak@CScriptGameResourceObjectScriptedThing@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CScriptThing::IsAlive (slot 75)
// and GetPThing (slot 11), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined
// GetInterface, interface 0x1f = CTCScriptedControl), operator new @ 0x00BFEA1A (0xa0 bytes),
// NTCScriptedControl::CActionTalkToThing constructor @ 0x00717AA0 (vtable 0x012659C8), CTCScriptedControl::AddAction @ 0x007137D0.
// Shape: the family guard; the own thing is fetched into a named local BEFORE the listener's thing (so the
// lookup runs off that register: `mov eax,[esi+20h]`, `add esi,44h`), then the unchecked GetInterface whose
// key and miss read share the dead first-argument slot; the second text pointer of the action is null.
// `this` uses engine/CScriptGameResourceObjectScriptedThing.h (PThing reached through a local view).
#include <stddef.h>
#include "engine/CScriptGameResourceObjectScriptedThing.h"

void* __cdecl operator new(size_t size);           // 0x00BFEA1A

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);          // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                   // +0x44
    unsigned char  _pad4c[0x20];
    unsigned long  ThingTypeFlags;                 // +0x6c
    unsigned char  _pad70[0x21];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
struct CIntelligentPointer_Retail {                // CIntelligentPointer<CThing>
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50 CBaseIntelligentPointer::GetPItem
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

namespace NTCScriptedControl {
struct CActionBase {                               // vtable 0x01265504
    virtual void v00();
};
}

class CTCScriptedControl { public:                        // interface 0x1f component
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

enum ETextGroupSelectionMethod { TEXT_GROUP_SELECT_RANDOM = 0 };

namespace NTCScriptedControl {
struct CActionTalkToThing : CActionBase {                      // vtable 0x012659C8, 0xa0 bytes
    unsigned char _body[0x9c];
    CActionTalkToThing(CThing_Retail* listener, const char* textGroup, const char* textGroup2, ETextGroupSelectionMethod method, bool b1, bool b2, bool b3);   // 0x00717AA0
};
}

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void Speak(const CScriptThing& listener, const char* textGroup, ETextGroupSelectionMethod method, bool b1, bool b2, bool b3);
};

void CScriptGameResourceObjectScriptedThing_Methods::Speak(const CScriptThing& listener, const char* textGroup, ETextGroupSelectionMethod method, bool b1, bool b2, bool b3)
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            if (listener.IsAlive()) {
                CThing_Retail* me = thing->GetPItem();
                CThing_Retail* listenerThing = ((CThing_Retail*)listener.GetPThing());
                CTCScriptedControl* control;
                me->GetInterface(&control, 0x1f);
                control->AddAction(new NTCScriptedControl::CActionTalkToThing(listenerThing, textGroup, 0, method, b1, b2, b3));
            }
        }
    }
}