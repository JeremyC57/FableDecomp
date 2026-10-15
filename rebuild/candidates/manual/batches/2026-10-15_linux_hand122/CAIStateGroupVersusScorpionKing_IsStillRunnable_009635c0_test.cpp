#include <cstdio>

// Test model of a thing: presence bitset + sorted {type, component} map + dead flag.
struct CTCBase { long tag; };

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

struct CAIBrain { CThing* PCreature; };

struct CTCCreatureModeManager : CTCBase {
    long mode;
    bool HasMode(long m) { return mode == m; }
};

struct CVersusEnemyBase {
    CBaseIntelligentPointer PTargetThing; bool baseRunnable;
    bool IsStillRunnable() { return baseRunnable; }
};

struct CVersusScorpionKing : CVersusEnemyBase {
    bool IsStillRunnable()
    {
        if (!CVersusEnemyBase::IsStillRunnable())
            return false;
        CTCCreatureModeManager* modes;
        PTargetThing.GetPItem()->GetInterface(&modes, 0x31);
        return modes->HasMode(0x38) ? false : true;
    }
};

static int fail() { std::printf("009635c0_TEST FAIL\n"); return 1; }

int main()
{
    CTCCreatureModeManager modes; modes.mode = 1;
    CThing target; target.Reset(); target.Add(0x31, &modes);
    CVersusScorpionKing g; g.PTargetThing.m_pItem = &target; g.baseRunnable = true;
    if (!g.IsStillRunnable()) return fail();
    modes.mode = 0x38;
    if (g.IsStillRunnable()) return fail();
    modes.mode = 1; g.baseRunnable = false;
    if (g.IsStillRunnable()) return fail();
    std::printf("009635c0_TEST PASS\n");
    return 0;
}
