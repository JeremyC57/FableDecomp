// CTCInventoryMap::TeleportToActiveRegion() -- when the highlighted region is not the current one, find a teleporter thing in any of its maps, close the inventory and send the hero through it
// Grade: DIFFER(274v277) -- dl=44: frame matches with the same-TU GetPFirstThing body (landed twin 0x00777000); register rotation left: retail this=edi, worldMap=ebx, region=ebp; ours ebx, ebp, edi
// Leaf: ?TeleportToActiveRegion@CTCInventoryMap@@AAEXXZ
class CTCBase;
extern bool g_bInventoryMapFlag;                    // 0x0137D76C
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                   // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char StateBits;                        // +0x91
    enum { ThingTypeMask = 0 };
    bool IsIncludedInThingScans() const
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long kind)
    {
        long key = kind;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || kind < it->type ? end : it)->component;
    }
    template<class T> __forceinline T* GetTC(long kind)
    {
        T* out;
        if (HasTC(kind))
            out = (T*)GetTC(kind);
        return out;
    }
};
class CTCTeleporter { public:
    void TeleportHero();                            // 0x007DFCF0
};
class CRegion { public:
    long* LinkedMapsStart;                          // +0x00 vector<long>
    long* LinkedMapsFinish;
    long* LinkedMapsEnd;
    unsigned long size() const { return LinkedMapsFinish - LinkedMapsStart; }
};
class CWorldMap { public:
    long GetLoadedRegion() const;                   // 0x004FB150
    const CRegion& GetRegion(long) const;           // 0x004FC180
};
class CThingFilter_HasTC { public:
    long m_type;
    bool operator()(const CThing& thing) const;     // 0x006A7xxx
    CThingFilter_HasTC() { }
    CThingFilter_HasTC(long type) : m_type(type) { }
};
class CThingFilter_InMap { public:
    const CWorldMap* m_worldMap;
    long m_map;
    bool operator()(const CThing& thing) const;     // 0x006A7930
    CThingFilter_InMap() { }
    CThingFilter_InMap(const CWorldMap& worldMap, long map) : m_worldMap(&worldMap), m_map(map) { }
};
template <class A, class B> struct CFilter_Bind2 {
    const A* m_first;
    const B* m_second;
    CFilter_Bind2(const A& first, const B& second) : m_first(&first), m_second(&second) { }
    template <class T> bool operator()(const T& thing) const { return (*m_first)(thing) && (*m_second)(thing); }
};
inline CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap> operator&&(const CThingFilter_HasTC& a, const CThingFilter_InMap& b) { return CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap>(a, b); }
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
struct CThingList {
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CThing* m_value; };
    SNode* m_pHead;
    struct iterator {
        SNode* m_pNode;
        iterator(SNode* node) : m_pNode(node) {}
        CThing* operator*() const { return m_pNode->m_value; }
        iterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    iterator begin() { return iterator(m_pHead->m_pNext); }
    iterator end() { return iterator(m_pHead); }
};
class CThingSearchTools { public:
    CThingList* PeekAllThings();                    // 0x0049C770
    template <class TFilter, class TThing> bool GetPFirstThing(const TFilter& filter, TThing** out);   // 0x00615750
};
// The template body (header-defined, as the landed byte twin 0x00777000).
template <class TFilter, class TThing>
bool CThingSearchTools::GetPFirstThing(const TFilter& filter, TThing** out)
{
    CThingList* things = PeekAllThings();
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (IsThingOfType<TThing>(thing) && thing->IsIncludedInThingScans() && filter(*(TThing*)thing))
        {
            *out = (TThing*)thing;
            return true;
        }
    }
    return false;
}
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();               // slot 12
    virtual const CWorldMap* PeekWorldMap() const;  // slot 13 (+0x34)
    unsigned char _pad04[0x20 - 0x04];
    CThingSearchTools* PThingSearchTools;           // +0x20
};
class CTCBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031();
protected:
    const CWorld& PeekWorld() const;                // 0x00686A80
};
class CTCInventoryMap : public CTCBase { public:
    virtual void AddCloseInventoryEvent();          // slot 32 (+0x80)
    unsigned char _pad004[0x238 - 0x004];   // CTCBase vptr +0x00
    long          ActiveRegionIndex;                // +0x238
private:
    void TeleportToActiveRegion();
};

void CTCInventoryMap::TeleportToActiveRegion()
{
    const CWorldMap* worldMap = PeekWorld().PeekWorldMap();
    if (worldMap->GetLoadedRegion() != ActiveRegionIndex) {
        const CRegion& region = worldMap->GetRegion(ActiveRegionIndex);
        for (unsigned long i = 0; i < region.size(); ++i) {
            CThing* teleporter;
            if (PeekWorld().PThingSearchTools->GetPFirstThing(CThingFilter_HasTC(0xdd) && CThingFilter_InMap(*worldMap, region.LinkedMapsStart[i]), &teleporter)) {
                g_bInventoryMapFlag = false;
                AddCloseInventoryEvent();
                teleporter->GetTC<CTCTeleporter>(0xdd)->TeleportHero();
                return;
            }
        }
    }
}
