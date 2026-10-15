// NCreatureMode::CModeBriarRoseConcentrate::FindHitEvent() -- true when the thing (+4) has an entity-events component (interface type 0x42, the inlined CThing::GetInterface: presence bit, CVectorMap::LowerBound @ 0x004365B0 on the map at +0x44) holding an event of type 1 (HasEvent<CIsEventOfType> @ 0x008E89D0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E79A0, length 84 bytes.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0, CTCEntityEvents::HasEvent<NEntityEvents::CIsEventOfType> @ 0x008E89D0 (the filter by reference). The GetInterface shape is the landed out-parameter model (0x00895D50): the lookup key and then the one-word filter share the single frame slot. Shape: `push ecx` frame, the map in esi, `mov al,1` / `xor al,al` tails, `ret`.
namespace NCreatureMode {
struct CTCBase;
struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20 the bitset of the interface types present
    CInterfaceMap m_interfaces;                    // +0x44
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

namespace NEntityEvents {
struct CIsEventOfType {
    long m_type;
    CIsEventOfType(long type) : m_type(type) { }
};
}
struct CTCEntityEvents {
    bool HasEvent(NEntityEvents::CIsEventOfType& filter);   // 0x008E89D0
};

struct CModeBase {                                 // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual ~CModeBase();
    CThing* m_pThing;                              // +0x04
};

struct CModeBriarRoseConcentrate : CModeBase {
    bool FindHitEvent();
};

bool CModeBriarRoseConcentrate::FindHitEvent()
{
    CTCEntityEvents* events;
    if (m_pThing->GetInterface(&events, 0x42)) {
        NEntityEvents::CIsEventOfType filter(1);
        if (events->HasEvent(filter))
            return true;
    }
    return false;
}
}
