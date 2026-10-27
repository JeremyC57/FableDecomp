// CTCInventoryMap::TeleportToActiveRegion() -- when the highlighted region is not the current one, find a teleporter thing in any of its maps, close the inventory and send the hero through it
// Grade: DIFFER(274v277) -- frame 0x1C v 0x18: retail packs the found thing, the GetInterface key and the teleporter TC into one slot and stores the Bind2 addresses after the size test; register rotation (ebx/ebp)
// Retail 0x005FBFA0, 288 bytes (+ int3), esp frame (speed TU).
// Callees: CTCBase::GetWorld() const 0x00686A80, CWorld virtual slot 13 (+0x34, GetWorldMap), CWorldMap::
// GetCurrentRegionNumber 0x004FB150 (manifest "CWorld::DrawGetEnvironment" is propagated), CWorldMap::GetRegion(long)
// 0x004FC180, CThingSearchTools::GetPFirstThing<CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap>, CThing>
// 0x00615750, this's virtual slot 32 (+0x80, AddCloseInventoryEvent), CVectorMap::LowerBound 0x0040F020 (the inline
// non-const GetInterface for the teleporter component 0xDD), CTCTeleporter::TeleportHero 0x007DFCF0.
// Shape: ActiveRegion (+0x238) against the world map's current region; then for each map number of that region the
// HasTC(0xDD) / InMap filters (bound by pointer) search for a teleporter; the first hit clears the global byte
// 0x0137D76C (meaning unknown), closes the inventory and calls TeleportHero on the teleporter component (an
// uninitialised out local: retail re-reads the stale slot on the miss path).
// Layout: Ego_r.pdb CRegion::Maps vector<long> at +0; retail CThing interface bitset +0x20 / map +0x44.
extern "C" int __cdecl puts(const char* text);              // 0x00BFEAF2 (MSVCR71 import thunk)
extern "C" __declspec(noreturn) void __cdecl abort(void);   // 0x00BFEAEC
namespace std {
template <unsigned int _Nb>
class bitset {                                              // STLport 4.6 bitset<274>: 9 words, test() range-checked (no exceptions)
public:
    enum { _Words = (_Nb + 31) / 32 };
    unsigned long _M_w[_Words];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static unsigned int _S_whichbit(unsigned int __pos) { return __pos % 32; }
    static unsigned long _S_maskbit(unsigned int __pos) { return 1UL << _S_whichbit(__pos); }
    unsigned long _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
    bool _Unchecked_test(unsigned int __pos) const { return (_M_getword(__pos) & _S_maskbit(__pos)) != 0; }
    bool test(unsigned int __pos) const
    {
        if (__pos >= _Nb) {
            puts("bitset");
            abort();
        }
        return _Unchecked_test(__pos);
    }
};
}
struct CWorldMap;
extern unsigned char g_bInventoryMapFlag;           // 0x0137D76C

struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    std::bitset<274> UsingComponents;                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents.test(interfaceType)) {
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
struct CWorldMap {
    long GetCurrentRegionNumber();                  // 0x004FB150
    CRegion& GetRegion(long region);                // 0x004FC180
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
struct CTCTeleporter {                              // interface type 0xDD
    void TeleportHero();                            // 0x007DFCF0
};

struct CTCInventoryMap {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void AddCloseInventoryEvent();          // slot 32 (+0x80)
    unsigned char _pad004[0x238 - 0x004];
    long ActiveRegion;                              // +0x238
    CWorld* GetWorld() const;                       // 0x00686A80 (CTCBase)
    void TeleportToActiveRegion();
};

void CTCInventoryMap::TeleportToActiveRegion()
{
    CWorldMap* worldMap = GetWorld()->GetWorldMap();
    if (worldMap->GetCurrentRegionNumber() == ActiveRegion)
        return;
    CRegion& region = worldMap->GetRegion(ActiveRegion);
    CThingFilter_HasTC hasTC;
    CThingFilter_InMap inMap;
    CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap> filter(hasTC, inMap);
    for (unsigned long i = 0; i < region.Maps.size(); ++i) {
        CThing* teleporter;
        inMap.WorldMap = worldMap;
        inMap.MapNo = region.Maps[i];
        hasTC.Type = 0xDD;
        if (GetWorld()->ThingSearchTools->GetPFirstThing(filter, &teleporter)) {
            g_bInventoryMapFlag = 0;
            AddCloseInventoryEvent();
            CTCTeleporter* teleporterTC;
            teleporter->GetInterface(&teleporterTC, 0xDD);
            teleporterTC->TeleportHero();
            return;
        }
    }
}
