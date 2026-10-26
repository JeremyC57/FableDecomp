#include <cstdio>

// Test model of a thing: presence bitset + sorted {type, component} map + dead flag.
class CTCBase { public: long tag; };

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

class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    CInterfaceMap::SEntry m_store[8];
    int m_count;
    void Reset()
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_flags91 = 0; m_count = 0;
        m_interfaces.first = m_store; m_interfaces.last = m_store;
        m_store[0].type = 0; m_store[0].component = 0;
    }
    void Add(long type, CTCBase* c)     // append in ascending type order
    {
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        m_store[m_count].type = type; m_store[m_count].component = c; ++m_count;
        m_interfaces.last = m_store + m_count;
        m_store[m_count].type = 0; m_store[m_count].component = 0;
    }
    bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
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

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public: CThing* PCreature; };

class CTCCreatureModeManager : public CTCBase { public:
    long mode;
    bool HasMode(long m) { return mode == m; }
};

struct CVersusScorpionKing {
    bool FindChargeEvent(const CBaseIntelligentPointer& target)
    {
        CTCCreatureModeManager* modes;
        if (IS_THING_ALIVE(target) && target.GetPItem()->GetInterface(&modes, 0x31) && modes->HasMode(0x3F))
            return false;
        return true;
    }
};

static int fail() { std::printf("008e8a10_TEST FAIL\n"); return 1; }

int main()
{
    CTCCreatureModeManager modes; modes.mode = 0x3F;
    CThing target; target.Reset(); target.Add(0x31, &modes);
    CBaseIntelligentPointer p; p.m_pItem = &target;
    CVersusScorpionKing g;
    if (g.FindChargeEvent(p)) return fail();
    modes.mode = 2;
    if (!g.FindChargeEvent(p)) return fail();
    modes.mode = 0x3F; target.m_flags91 = 1;
    if (!g.FindChargeEvent(p)) return fail();
    p.m_pItem = 0;
    if (!g.FindChargeEvent(p)) return fail();
    CThing bare; bare.Reset(); p.m_pItem = &bare;
    if (!g.FindChargeEvent(p)) return fail();
    std::printf("008e8a10_TEST PASS\n");
    return 0;
}