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

struct C3DVector { float X, Y, Z; };

struct CTCWanderControl : CTCBase { bool enabled; bool IsEnabled() { return enabled; } };

struct CCreature : CThing {
    float InitialPosX, InitialPosY; C3DVector pos;
    C3DVector* GetPosition() { return &pos; }
};

struct CBrain2 { CCreature* PCreature; };

struct CWander {
    CBrain2* PBrain;
    bool IsRunnable()
    {
        CTCWanderControl* control;
        if (PBrain->PCreature->GetInterface(&control, 8) && control->IsEnabled()) {
            CCreature* creature = PBrain->PCreature;
            C3DVector* position = creature->GetPosition();
            float dx = creature->InitialPosX - position->X;
            float dy = creature->InitialPosY - position->Y;
            float distanceSq = dx * dx;
            distanceSq += dy * dy;
            if (distanceSq > 100.0f)
                return true;
        }
        return false;
    }
};

static int fail() { std::printf("00911a00_TEST FAIL\n"); return 1; }

int main()
{
    CTCWanderControl control; control.enabled = true;
    CCreature c; c.Reset(); c.Add(8, &control);
    c.InitialPosX = 0.0f; c.InitialPosY = 0.0f; c.pos.X = 6.0f; c.pos.Y = 8.0f; c.pos.Z = 50.0f;  // 100: not beyond
    CBrain2 brain; brain.PCreature = &c;
    CWander g; g.PBrain = &brain;
    if (g.IsRunnable()) return fail();
    c.pos.Y = 8.5f;
    if (!g.IsRunnable()) return fail();
    control.enabled = false;
    if (g.IsRunnable()) return fail();
    std::printf("00911a00_TEST PASS\n");
    return 0;
}
