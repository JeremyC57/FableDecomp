// CTCWife::IsCourtingHero() const -- courtable and the parent's opinion-of-hero component has its courting flag set
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CF890, 98 bytes (+14 int3). __fastcall, returns bool in al.
// Callees: CTCWife::IsCourtableAndNotBlockingIt() const 0x006CF810; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0.
// Shape: HasTC(0x4F) (CTCCreatureOpinionOfHero, bitset word 2 bit 15: `mov ecx,8000h ; test ecx,edx` twice) followed by an
//        inline GetTC that re-tests the bit and returns an unset local on the absent path (`mov eax,[esp+4]`); the flag
//        byte is the component's +0x340 (descriptive name).
// Layout: CTCWife ParentThing +0x04; retail CThing bitset +0x20, interface map +0x44.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    unsigned char _pad92[0x1c9 - 0x92];
    unsigned char Flags1C9;                     // +0x1c9 (CThingCreatureBase flag byte; bit 3 tested, descriptive)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;               // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
};
class CTCCreatureOpinionOfHero { public:
    unsigned char _pad_0x000[0x340];
    bool CourtingHero;                           // +0x340 (descriptive)
};
__forceinline bool HasTC(CThing* thing, long type)
{
    if (thing->m_presentBits[type >> 5] & (1UL << (type & 31)))
        return true;
    return false;
}
template <class T> __forceinline T* GetTC(CThing* thing, long type)
{
    T* tc;
    thing->GetInterface(&tc, type);
    return tc;
}
class CTCWife { public:
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    bool IsCourtableAndNotBlockingIt() const;    // 0x006CF810
    bool IsCourtingHero() const;
};
bool CTCWife::IsCourtingHero() const
{
    if (IsCourtableAndNotBlockingIt() && HasTC(ParentThing, 0x4f)) {
        if (GetTC<CTCCreatureOpinionOfHero>(ParentThing, 0x4f)->CourtingHero)
            return true;
    }
    return false;
}