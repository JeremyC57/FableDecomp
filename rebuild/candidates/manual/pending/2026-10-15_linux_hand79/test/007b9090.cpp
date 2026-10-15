// Behaviour test for CTCTavernTable::FreePoint @ 0x007B9090: the index reaches the parent thing's interface-0x2E component.
#include <cstdio>

struct CTCBase { int unused; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};

struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
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

// A thing carrying components of types 0x10, TYPE and 0x90 (sorted), with TYPE's presence bit set.
struct CTestThing : CThing {
    CInterfaceMap::SEntry entries[4];
    void Init(long type, CTCBase* component, CTCBase* sentinel)
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_flags91 = 0;
        entries[0].type = 0x10; entries[0].component = 0;
        entries[1].type = type;  entries[1].component = component;
        entries[2].type = 0x90; entries[2].component = 0;
        entries[3].type = 0x7fffffff; entries[3].component = sentinel;   // the map's end slot
        m_interfaces.first = entries;
        m_interfaces.last = entries + 3;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
    }
};

struct CTCFreePointList : CTCBase {
    long added[4]; int count;
    void AddFreePoint(long index) { if (count < 4) added[count] = index; ++count; }
};

struct CTCTavernTable {
    CThing* ParentThing;
    void FreePoint(long index)
    {
        CTCFreePointList* points;
        ParentThing->GetInterface(&points, 0x2E);
        points->AddFreePoint(index);
    }
};

int main()
{
    CTCFreePointList list; list.count = 0;
    CTCFreePointList other; other.count = 0;
    CTestThing thing; thing.Init(0x2E, &list, &other);
    CTCTavernTable table; table.ParentThing = &thing;
    table.FreePoint(3);
    table.FreePoint(7);
    bool ok = list.count == 2 && list.added[0] == 3 && list.added[1] == 7 && other.count == 0;
    if (!ok) { std::printf("007b9090_TEST FAIL\n"); return 1; }
    std::printf("007b9090_TEST PASS\n");
    return 0;
}
