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

struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };

struct CTCShop { long state; void SetStockState(long s) { state = s; } };
struct CTCShopKeeper : CTCBase { CTCShop* shop; CTCShop* GetShopTC() { return shop; } };

struct CCreature : CThing {
    float speed;
    virtual float GetAverageWalkSpeed(const C3DVector& d) { return (d.X == 0.0f && d.Y == 0.0f && d.Z == 0.0f) ? speed : -1.0f; }
    virtual ~CCreature() {}
};

struct CBrain2 { CCreature* PCreature; };

struct CTavernKeeper {
    CBrain2* PBrain; float AverageWalkSpeed;
    void InitStateGroup()
    {
        C3DVector direction(0.0f, 0.0f, 0.0f);
        AverageWalkSpeed = PBrain->PCreature->GetAverageWalkSpeed(direction);
        CTCShopKeeper* shopKeeper;
        if (PBrain->PCreature->GetInterface(&shopKeeper, 0x90) && shopKeeper->GetShopTC() != 0)
            shopKeeper->GetShopTC()->SetStockState(0);
    }
};

static int fail() { std::printf("009685c0_TEST FAIL\n"); return 1; }

int main()
{
    CTCShop shop; shop.state = 3;
    CTCShopKeeper keeper; keeper.shop = &shop;
    CCreature c; c.Reset(); c.speed = 2.5f; c.Add(0x90, &keeper);
    CBrain2 brain; brain.PCreature = &c;
    CTavernKeeper g; g.PBrain = &brain; g.AverageWalkSpeed = 0.0f;
    g.InitStateGroup();
    if (g.AverageWalkSpeed != 2.5f || shop.state != 0) return fail();
    shop.state = 3; keeper.shop = 0; g.InitStateGroup();
    if (shop.state != 3) return fail();
    std::printf("009685c0_TEST PASS\n");
    return 0;
}