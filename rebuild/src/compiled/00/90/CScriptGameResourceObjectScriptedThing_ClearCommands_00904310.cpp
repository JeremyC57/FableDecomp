// CScriptGameResourceObjectScriptedThing::ClearCommands() -- for a live scripted thing, tell its scripted-control component (interface 0x1f) to clean up all its actions
// Leaf: ClearCommands
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00904310, length 112 bytes (110 + int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 three times (PThing at +0x14),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the inlined GetInterface),
// CTCScriptedControl::CleanupAllActions @ 0x00713630.
// Shape: nested null / dead-bit (+0x91 bit 0) tests re-fetching the item, then the inlined
// GetInterface template (landed 0x008386E0 shape): interface 0x1f is bit 31 of the bitset dword at +0x20,
// so the presence test is `test ecx,ecx ; jns`. The miss path calls through the never-written component
// local, homed in the key temporary's frame word (`mov eax,[esp+4]`).
// `this` uses engine/CScriptGameResourceObjectScriptedThing.h (PThing reached through a local view).
#include "engine/CScriptGameResourceObjectScriptedThing.h"

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);          // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing_Retail { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                   // +0x44
    unsigned char  _pad4c[0x45];
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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCScriptedControl { public:
    void CleanupAllActions();                      // 0x00713630
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void ClearCommands();
};

void CScriptGameResourceObjectScriptedThing_Methods::ClearCommands()
{
    CIntelligentPointer<CThing_Retail>* thing = (CIntelligentPointer<CThing_Retail>*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            control->CleanupAllActions();
        }
    }
}