#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
struct CTCBase { int m_dummy; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
    SEntry* LowerBound(const long* type) const { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
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
class CThingVector {
public:
    CThing* m_items[8];
    int m_count;
    CThingVector() : m_count(0) {}
    void push_back(CThing* const& thing) { m_items[m_count++] = thing; }
};
class CAddFreeBenchToList {
public:
    CThingVector* m_pBenches;
    void operator()(const CIntelligentPointer<CThing>& thing) const;
};
void CAddFreeBenchToList::operator()(const CIntelligentPointer<CThing>& thing) const
{
    if (thing.GetPItem() && thing.GetPItem()->m_thingType == 5 && ((unsigned char)(thing.GetPItem()->GetStateFlags() >> 17) & 1)) {
        CTCUserActionPointBase* actionPoint;
        if (thing.GetPItem()->GetInterfaceConst(0x2d, &actionPoint) && actionPoint->IsActionPointFree()) {
            CThing* bench = thing.GetPItem();
            m_pBenches->push_back(bench);
        }
    }
}
int main()
{
    CThingVector benches;
    CAddFreeBenchToList add; add.m_pBenches = &benches;
    CTCUserActionPointBase freeAp; freeAp.m_free = true; freeAp.m_inUse = 0;
    CTCUserActionPointBase busyAp; busyAp.m_free = false; busyAp.m_inUse = 2;
    CThing b0, b1, b2, b3;
    b0.m_stateFlags = 1UL << 17; b0.AddTC(0x2d, &freeAp);
    b1.m_stateFlags = 1UL << 17; b1.AddTC(0x2d, &busyAp);
    b2.m_stateFlags = 1UL << 16; b2.AddTC(0x2d, &freeAp);
    b3.m_stateFlags = 1UL << 17; b3.AddTC(0x2d, &freeAp);
    add(CIntelligentPointer<CThing>(&b0)); add(CIntelligentPointer<CThing>(&b1)); add(CIntelligentPointer<CThing>(0));
    add(CIntelligentPointer<CThing>(&b2)); add(CIntelligentPointer<CThing>(&b3));
    if (benches.m_count != 2 || benches.m_items[0] != &b0 || benches.m_items[1] != &b3) { std::printf("0090c060_TEST FAIL\n"); return 1; }
    std::printf("0090c060_TEST PASS\n");
    return 0;
}