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
    long released;
    void ReleaseActionPoint(long index) { released = index; }
};

struct CTCCarrying : CTCBase {
    int removed;
    CThing* RemoveThingFromPrimarySlot(bool) { ++removed; return 0; }
};

struct CGameTime { long Day; };
struct CWorld { CGameTime* PGameTime; };

struct CSitForMeal {
    CAIBrain* PBrain; CWorld* world;
    CBaseIntelligentPointer PPot; long ActionPointIndex; bool HoldingBowl; long DayLastAte;
    CWorld* GetPWorld() { return world; }
    void ReleaseControlOfPot()
    {
        if (ActionPointIndex >= 0) {
            if (IS_THING_ALIVE(PPot)) {
                CTCUserActionPointBase* actionPoint;
                if (PPot.GetPItem()->GetInterface(&actionPoint, 0x2D))
                    actionPoint->ReleaseActionPoint(ActionPointIndex);
            }
            ActionPointIndex = -1;
        }
    }
    void Cleanup()
    {
        ReleaseControlOfPot();
        DayLastAte = GetPWorld()->PGameTime->Day;
        if (HoldingBowl) {
            CTCCarrying* carrying;
            PBrain->PCreature->GetInterface(&carrying, 0x46);
            carrying->RemoveThingFromPrimarySlot(false);
            HoldingBowl = false;
        }
    }
};

static int fail() { std::printf("00945530_TEST FAIL\n"); return 1; }

int main()
{
    CThing pot; pot.Reset();
    CTCUserActionPointBase ap; ap.released = -9;
    pot.Add(0x2D, &ap);
    CSitForMeal g; g.PPot.m_pItem = &pot; g.ActionPointIndex = 4;
    g.ReleaseControlOfPot();
    if (ap.released != 4 || g.ActionPointIndex != -1) return fail();
    ap.released = -9; g.ReleaseControlOfPot();               // negative index: nothing happens
    if (ap.released != -9 || g.ActionPointIndex != -1) return fail();
    pot.m_flags91 = 1; g.ActionPointIndex = 2; g.ReleaseControlOfPot();   // dead pot: index still reset
    if (ap.released != -9 || g.ActionPointIndex != -1) return fail();
    std::printf("00945530_TEST PASS\n");
    return 0;
}
