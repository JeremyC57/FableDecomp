#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CTCBase { public: int m_dummy; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
    SEntry* LowerBound(const long* type) const { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing { public:
    unsigned long m_stateFlags;
    unsigned char m_thingType;
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    CInterfaceMap::SEntry m_entries[6];
    CThing() : m_stateFlags(0), m_thingType(5), m_flags91(0)
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        for (int i = 0; i < 6; ++i) { m_entries[i].type = 0x7fffffff; m_entries[i].component = 0; }
        m_interfaces.first = m_interfaces.last = m_interfaces.end = m_entries;
    }
    virtual ~CThing() {}
    virtual unsigned long GetStateFlags() { return m_stateFlags; }
    void AddTC(long type, CTCBase* component)     // test helper: add in ascending type order
    {
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        m_interfaces.last->type = type; m_interfaces.last->component = component;
        ++m_interfaces.last; m_interfaces.end = m_interfaces.last;
    }
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool GetInterfaceConst(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CBaseIntelligentPointer {
public:
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return m_pItem; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    CIntelligentPointer(CThing* thing) { m_pItem = thing; }
    CIntelligentPointer(const CIntelligentPointer& o) : CBaseIntelligentPointer() { m_pItem = o.m_pItem; }
};
struct CTCUserActionPointBase : CTCBase {
    bool m_free;
    long m_inUse;
    bool IsActionPointFree() { return m_free; }
    long GetNumberOfActionPointsInUse() { return m_inUse; }
};
class CAIStateGroup_LightHomeCandle {
public:
    CThing* m_pNeedsChanging;
    bool IsCandleInNeedOfChanging(CThing* candle) { return candle == m_pNeedsChanging; }
};
class CIsCandleNeedingChanging {
public:
    CAIStateGroup_LightHomeCandle* m_pStateGroup;
    bool operator()(const CIntelligentPointer<CThing>& thing) const;
    static __forceinline bool IsCandle(CThing* candle, const CIntelligentPointer<CThing>& thing)
    {
        return ((unsigned char)(candle->GetStateFlags() >> 8) & 1)
            || (((unsigned char)(candle->GetStateFlags() >> 9) & 1) && thing.GetPItem()->HasTC(0x27));
    }
};
bool CIsCandleNeedingChanging::operator()(const CIntelligentPointer<CThing>& thing) const
{
    if (thing.GetPItem() && thing.GetPItem()->m_thingType == 5 && IsCandle(thing.GetPItem(), thing)
        && !(thing.GetPItem()->m_flags91 & 0x20)) {
        CTCUserActionPointBase* actionPoint;
        if (thing.GetPItem()->GetInterfaceConst(0x2d, &actionPoint) && actionPoint->IsActionPointFree()
            && actionPoint->GetNumberOfActionPointsInUse() == 0)
            return m_pStateGroup->IsCandleInNeedOfChanging(thing.GetPItem());
    }
    return false;
}
int main()
{
    CTCBase lampTc;
    CTCUserActionPointBase ap; ap.m_free = true; ap.m_inUse = 0;
    CThing c8; c8.m_stateFlags = 1UL << 8; c8.AddTC(0x2d, &ap);
    CThing c9; c9.m_stateFlags = 1UL << 9; c9.AddTC(0x27, &lampTc); c9.AddTC(0x2d, &ap);
    CThing c9bare; c9bare.m_stateFlags = 1UL << 9; c9bare.AddTC(0x2d, &ap);
    CAIStateGroup_LightHomeCandle group; group.m_pNeedsChanging = &c8;
    CIsCandleNeedingChanging pred; pred.m_pStateGroup = &group;
    if (!pred(CIntelligentPointer<CThing>(&c8)) || pred(CIntelligentPointer<CThing>(&c9))) { std::printf("00946c60_TEST FAIL\n"); return 1; }
    group.m_pNeedsChanging = &c9;
    if (!pred(CIntelligentPointer<CThing>(&c9))) { std::printf("00946c60_TEST FAIL\n"); return 1; }
    group.m_pNeedsChanging = &c9bare;
    if (pred(CIntelligentPointer<CThing>(&c9bare))) { std::printf("00946c60_TEST FAIL\n"); return 1; }                  // bit 9 without interface 0x27
    group.m_pNeedsChanging = &c9;
    ap.m_inUse = 1;
    if (pred(CIntelligentPointer<CThing>(&c9))) { std::printf("00946c60_TEST FAIL\n"); return 1; }
    ap.m_inUse = 0; ap.m_free = false;
    if (pred(CIntelligentPointer<CThing>(&c9))) { std::printf("00946c60_TEST FAIL\n"); return 1; }
    ap.m_free = true; c9.m_flags91 = 0x20;
    if (pred(CIntelligentPointer<CThing>(&c9))) { std::printf("00946c60_TEST FAIL\n"); return 1; }
    c9.m_flags91 = 0; c9.m_thingType = 3;
    if (pred(CIntelligentPointer<CThing>(&c9)) || pred(CIntelligentPointer<CThing>(0))) { std::printf("00946c60_TEST FAIL\n"); return 1; }
    std::printf("00946c60_TEST PASS\n");
    return 0;
}