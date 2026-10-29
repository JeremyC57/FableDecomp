// CScriptGameResourceObjectScriptedThing::Speak(const CScriptThing& listener, unsigned long textID, ETextGroupSelectionMethod method, bool b1, bool b2, bool b3) -- queue a talk-to action speaking the text entry textID at a live listener for the live scripted thing
// Leaf: Speak
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009046A0, length 213 bytes (`ret 18h`), vtable 0x0128D86C slot 14.
// PDB: ?Speak@CScriptGameResourceObjectScriptedThing@@UAEXABVCScriptThing@@KW4ETextGroupSelectionMethod@@_N22@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CScriptThing::IsAlive (slot 75)
// and GetPThing (slot 11), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined
// GetInterface, interface 0x1f = CTCScriptedControl), operator new @ 0x00BFEA1A (0xa0 bytes),
// NTCScriptedControl::CActionTalkToThing constructor @ 0x00717C50 (vtable 0x012659C8), CTCScriptedControl::AddAction @ 0x007137D0.
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

class CScriptThing {                               // vtable 0x01238C8C
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual CThing_Retail* GetPThing() const;      // slot 11 (+0x2c) 0x004AAA60
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46();
    virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsAlive() const;                  // slot 75 (+0x12c) 0x004AB130
};

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
    CActionTalkToThing(CThing_Retail* listener, unsigned long textID, const char* textGroup, ETextGroupSelectionMethod method, bool b1, bool b2, bool b3);   // 0x00717C50
};
}

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void Speak(const CScriptThing& listener, unsigned long textID, ETextGroupSelectionMethod method, bool b1, bool b2, bool b3);
};

void CScriptGameResourceObjectScriptedThing_Methods::Speak(const CScriptThing& listener, unsigned long textID, ETextGroupSelectionMethod method, bool b1, bool b2, bool b3)
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            if (listener.IsAlive()) {
                CThing_Retail* me = thing->GetPItem();
                CThing_Retail* listenerThing = listener.GetPThing();
                CTCScriptedControl* control;
                me->GetInterface(&control, 0x1f);
                control->AddAction(new NTCScriptedControl::CActionTalkToThing(listenerThing, textID, 0, method, b1, b2, b3));
            }
        }
    }
}