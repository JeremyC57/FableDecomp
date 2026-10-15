// Behaviour test for CThingNoise::PassToNoiseComponent @ 0x008386E0: the thing reaches this thing's own interface-0x37 component.
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

struct CTCNoise : CTCBase {
    CThing* taken; int calls;
    void TakeThing(CThing* thing) { taken = thing; ++calls; }
};

struct CThingNoise : CTestThing {
    void PassToNoiseComponent(CThing* thing)
    {
        CTCNoise* noise;
        GetInterface(&noise, 0x37);
        noise->TakeThing(thing);
    }
};

int main()
{
    CTCNoise noise; noise.taken = 0; noise.calls = 0;
    CTCNoise other; other.taken = 0; other.calls = 0;
    CThingNoise self; self.Init(0x37, &noise, &other);
    CTestThing hearer; hearer.Init(0x10, 0, 0);
    self.PassToNoiseComponent(&hearer);
    bool ok = noise.taken == &hearer && noise.calls == 1 && other.calls == 0;
    if (!ok) { std::printf("008386e0_TEST FAIL\n"); return 1; }
    std::printf("008386e0_TEST PASS\n");
    return 0;
}
