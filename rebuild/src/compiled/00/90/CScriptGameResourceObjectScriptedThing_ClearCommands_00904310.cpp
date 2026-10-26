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

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
};

struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                   // +0x44
    unsigned char  _pad4c[0x45];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CIntelligentPointer_Retail {
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50
};

class CTCScriptedControl { public:
    void CleanupAllActions();                      // 0x00713630
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void ClearCommands();
};

void CScriptGameResourceObjectScriptedThing_Methods::ClearCommands()
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            control->CleanupAllActions();
        }
    }
}