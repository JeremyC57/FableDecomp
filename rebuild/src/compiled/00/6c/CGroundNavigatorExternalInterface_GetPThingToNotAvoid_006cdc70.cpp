// CGroundNavigatorExternalInterface::GetPThingToNotAvoid() const -- the word at +0x20 of the record (+0x0c) of the navigation component of the thing the creature's navigation component (kind 7) is told not to avoid; 0 when there is no such thing or it has no navigation component
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CDC70, length 135 bytes (padded to 144).
// Callees: the const component-map LowerBound @ 0x004365B0 (kind 7, twice), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
//          (the intelligent pointer at +0x38 of the navigation component).
// Shape: the landed GetNavigatorObstacles (0x006CDBD0) out-parameter GetInterface for the creature (the component left
// unset on the absent path: the dead key word is read); the other thing's component goes through a null-initialised
// out pointer declared before the `if (other)` (absent paths join the `xor eax,eax` exit), then `if (!p) return 0;`
// ahead of the dereference (retail falls through into the zero exit and jumps forward to the found tail).
// Model: the engine header for the interface (Creature at +4); the creature and the navigation component are local
// models (as the landed siblings).
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the creature's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound_004365b0(const long* kind);          // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound)
    STCEntry_Retail* LowerBound_004365b0(const long* kind) const;    // 0x004365B0 (the second instantiation)
};
class CThingCreatureBase { public:                        // the retail creature as seen from the navigator interface
    virtual void v000();
    unsigned char         _pad04[0x1c];
    unsigned long         m_presentBits[9];        // +0x20 bitset of the component kinds present
    CTCMap_Retail         m_tcs;                   // +0x44 the sorted {kind, component} map
    template<class T> __forceinline void GetInterface(T** ppOut, long kind)   // the inlined CThing::GetInterface (out-parameter form)
    {
        if (m_presentBits[kind >> 5] & (1UL << (kind & 31))) {
            long type = kind;
            CTCMap_Retail* map = &m_tcs;
            STCEntry_Retail* it = map->LowerBound_004365b0(&type);
            if (it == map->m_pLast || kind < it->m_kind)
                it = map->m_pLast;
            *ppOut = (T*)it->m_pTC;
        }
    }
    template<class T> __forceinline void GetInterface(T** ppOut, long kind) const
    {
        if (m_presentBits[kind >> 5] & (1UL << (kind & 31))) {
            long type = kind;
            const CTCMap_Retail* map = &m_tcs;
            STCEntry_Retail* it = map->LowerBound_004365b0(&type);
            if (it == map->m_pLast || kind < it->m_kind)
                it = map->m_pLast;
            *ppOut = (T*)it->m_pTC;
        }
    }
};
#include "engine/CGroundNavigatorExternalInterface.h"
struct CBaseIntelligentPointer {                   // CIntelligentPointer<CThing>: vptr + item
    void* m_pVtbl;
    void* m_pItem;
    CThingCreatureBase* GetPItem() const;          // 0x00A01B50
};
struct SNavigationRecord_Retail {
    unsigned char _pad00[0x20];
    void*         m_pThing;                        // +0x20
};
struct CTCCreatureNavigation_Retail {              // component kind 7
    unsigned char             _pad00[0x0c];
    SNavigationRecord_Retail* m_pRecord;           // +0x0c
    unsigned char             _pad10[0x28];
    CBaseIntelligentPointer   m_thingToNotAvoid;   // +0x38
};

struct CGroundNavigatorExternalInterface_Methods : CGroundNavigatorExternalInterface {
    void* GetPThingToNotAvoid() const;
};

void* CGroundNavigatorExternalInterface_Methods::GetPThingToNotAvoid() const
{
    CTCCreatureNavigation_Retail* navigation;
    Creature->GetInterface(&navigation, 7);
    CThingCreatureBase* other = navigation->m_thingToNotAvoid.GetPItem();
    CTCCreatureNavigation_Retail* otherNavigation = 0;
    if (other)
        other->GetInterface(&otherNavigation, 7);
    if (!otherNavigation)
        return 0;
    return otherNavigation->m_pRecord->m_pThing;
}