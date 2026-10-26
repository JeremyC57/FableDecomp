// CThingCompare_BestForTargeting2::GetThingCompareMultiplier(CThing* thing) const -- targeting weight of a thing: 0 when dead, 0.1 when its TC 9 component flags it (bit 2 of +0xc), the enemy preference factor for enemies of the hero, else 1
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F9E70, 129 bytes (`ret 4`, float in st0).
// Callees: CInterfaceMap::LowerBound @ 0x004365B0, AreThingsEnemies(CThing*, CThing*) @ 0x0076D2F0 (`__fastcall`).
// Float pool: 0x0122DEDC = 0.0, 0x0122EE58 = 0.1, 0x0122DED8 = 1.0.
// Layout: CThingCompare_BestForTargeting2 (Ego_r.pdb 52 bytes): Hero +0x00, VectorCompare +0x04,
//   EnemyPreferenceFactor +0x30; retail CThing: interface bitset +0x20 (TC 9 = word +0x20 bit 9), CVectorMap +0x44,
//   Dead +0x91 bit 0; the TC 9 component's flag word at +0xc (bit 2 tested; component class not identified).
// Shape: IS_THING_ALIVE inline; the TC 9 lookup is the out-parameter GetInterface(&tc, 9) (the component re-uses eax;
//   a GetTC return value takes ecx); the hero read once.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CTCInterface9 {                          // the component behind ETCInterfaceType 9
    unsigned char _pad[0xc];
    unsigned long m_flags;                      // +0x0c (bit 2: low-priority target)
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                     // +0x91 bit 0
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
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
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};
bool __fastcall AreThingsEnemies(CThing* a, CThing* b);     // 0x0076D2F0
__forceinline bool IS_THING_ALIVE(CThing* p) { return p && !p->Dead; }

class CVectorCompare_BestForTargeting { public: unsigned long m_words[11]; };
class CThingCompare_BestForTargeting2 { public:
    CThing* Hero;                               // +0x00
    CVectorCompare_BestForTargeting VectorCompare;  // +0x04
    float EnemyPreferenceFactor;                // +0x30
    float GetThingCompareMultiplier(CThing* thing) const;
};

float CThingCompare_BestForTargeting2::GetThingCompareMultiplier(CThing* thing) const
{
    if (!IS_THING_ALIVE(thing))
        return 0.0f;
    CTCInterface9* tc;
    if (thing->GetInterface(&tc, 9) && (tc->m_flags & 4))
        return 0.1f;
    if (AreThingsEnemies(Hero, thing))
        return EnemyPreferenceFactor;
    return 1.0f;
}