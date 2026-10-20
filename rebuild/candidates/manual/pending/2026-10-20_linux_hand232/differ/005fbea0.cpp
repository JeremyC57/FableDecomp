// Grade: DIFFER(242v248) -- the filter locals are function-scope here (no aligned frame); retail keeps found/key in one slot, hoists only the Bind2 address stores and keeps this in ebp / region reloaded from the arg slot
// CTCInventoryMap::GetRegionTeleporter(CRegion* region) -- the first active teleporter thing found in any of the region's maps, or 0
// Retail 0x005FBEA0, 256 bytes (+ int3), `ret 4`, esp frame (speed TU).
// Callees: CTCBase::GetWorld() const 0x00686A80 (called once for the world map and once per map for the search
// tools), CWorld virtual slot 13 (+0x34, GetWorldMap), CThingSearchTools::GetPFirstThing<CFilter_Bind2<
// CThingFilter_HasTC, CThingFilter_InMap>, CThing> 0x00615750 (landed), the const CVectorMap LowerBound 0x004365B0
// (the inline CThing::GetInterface for the teleporter component 0xDD: bit 29 of the bitset dword at +0x38).
// Shape: for each map number of region->Maps (+0x00, size() re-read every iteration) the HasTC(0xDD) / InMap
// (world map, map) filters are bound by reference into a CFilter_Bind2 whose two address stores VC7.1 hoists above
// the loop; the found thing starts at 0. The teleporter component pointer is an UNINITIALISED local that the
// inline GetInterface writes only when the bit is set (retail re-reads the stale slot otherwise); a thing whose
// teleporter is Active (+0x10) is returned.
// Layout: Ego_r.pdb CRegion::Maps vector<long> at +0; CTCTeleporter::Active +0x10; CWorld::ThingSearchTools +0x20;
// retail CThing interface bitset +0x20 / interface map +0x44 (local models as in the landed
// TeleportHeroToTeleporter 0x005FBD20).
struct CWorldMap;
struct CThing;

struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CThingFilter_HasTC {
    long Type;                                      // ETCInterfaceType
    CThingFilter_HasTC() {}
};
struct CThingFilter_InMap {
    CWorldMap* WorldMap;
    long MapNo;
    CThingFilter_InMap() {}
};
template <class A, class B> struct CFilter_Bind2 {  // both filters, held by pointer
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) {}
};
struct CThingSearchTools {
    template <class TFilter, class T> bool GetPFirstThing(const TFilter& filter, T** found);   // 0x00615750
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CWorldMap* GetWorldMap();               // slot 13 (+0x34)
    unsigned char _pad04[0x20 - 0x04];
    CThingSearchTools* ThingSearchTools;            // +0x20
};
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    long& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CRegion {
    CLongVector Maps;                               // +0x00
};
struct CTCTeleporter {                              // interface type 0xDD
    unsigned char _pad00[0x10];
    bool Active;                                    // +0x10
};

struct CTCInventoryMap {
    CWorld* GetWorld() const;                       // 0x00686A80 (CTCBase)
    CThing* GetRegionTeleporter(CRegion* region);
};

CThing* CTCInventoryMap::GetRegionTeleporter(CRegion* region)
{
    CWorldMap* worldMap = GetWorld()->GetWorldMap();
    CThingFilter_HasTC hasTC;
    CThingFilter_InMap inMap;
    CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap> filter(hasTC, inMap);
    for (unsigned long i = 0; i < region->Maps.size(); ++i) {
        CThing* teleporter = 0;
        inMap.WorldMap = worldMap;
        inMap.MapNo = region->Maps[i];
        hasTC.Type = 0xDD;
        if (GetWorld()->ThingSearchTools->GetPFirstThing(filter, &teleporter)) {
            CTCTeleporter* teleporterTC;
            teleporter->GetInterface(&teleporterTC, 0xDD);
            if (teleporterTC && teleporterTC->Active)
                return teleporter;
        }
    }
    return 0;
}
