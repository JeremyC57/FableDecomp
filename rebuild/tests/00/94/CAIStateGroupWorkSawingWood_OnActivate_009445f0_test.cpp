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

struct CTCUserActionPointBase : CTCBase {
    bool free_; long index_; CThing* who;
    bool IsActionPointFree() { return free_; }
    bool GetFreeActionPoint(CThing* creature, float* pos, unsigned char* rh, long* index)
    { if (index_ < 0) return false; who = creature; *index = index_; pos[0] = 1.0f; rh[0] = 7; return true; }
};

struct CStateGroup {
    CAIBrain* PBrain; bool TookActionPoint; CBaseIntelligentPointer PObject;
    float ActionPointPos[3]; unsigned char ActionPointRHSet[0x1b]; long ActionPointIndex; int terminated;
    void Terminate() { ++terminated; }
    void OnActivate()
    {
        CTCUserActionPointBase* actionPoint;
        if (IS_THING_ALIVE(PObject) && PObject.GetPItem()->GetInterface(&actionPoint, 0x2D)
            && actionPoint->IsActionPointFree()) {
            CThing* creature = PBrain->PCreature;
            if (actionPoint->GetFreeActionPoint(creature, ActionPointPos, ActionPointRHSet, &ActionPointIndex)) {
                TookActionPoint = true;
                return;
            }
        }
        TookActionPoint = false;
        Terminate();
    }
};

static int fail() { std::printf("009445f0_TEST FAIL\n"); return 1; }

int main()
{
    CThing creature; creature.Reset();
    CAIBrain brain; brain.PCreature = &creature;
    CThing obj; obj.Reset();
    CTCUserActionPointBase ap; ap.free_ = true; ap.index_ = 3; ap.who = 0;
    obj.Add(0x2D, &ap);
    CStateGroup g; g.PBrain = &brain; g.PObject.m_pItem = &obj; g.terminated = 0; g.ActionPointIndex = -1;
    g.OnActivate();
    if (!g.TookActionPoint || g.ActionPointIndex != 3 || ap.who != &creature || g.terminated) return fail();
    ap.free_ = false; g.OnActivate();
    if (g.TookActionPoint || g.terminated != 1) return fail();
    obj.m_flags91 = 1; ap.free_ = true; g.OnActivate();
    if (g.TookActionPoint || g.terminated != 2) return fail();
    obj.m_flags91 = 0; g.PObject.m_pItem = 0; g.OnActivate();
    if (g.TookActionPoint || g.terminated != 3) return fail();
    CThing bare; bare.Reset(); g.PObject.m_pItem = &bare; g.OnActivate();
    if (g.TookActionPoint || g.terminated != 4) return fail();
    std::printf("009445f0_TEST PASS\n");
    return 0;
}