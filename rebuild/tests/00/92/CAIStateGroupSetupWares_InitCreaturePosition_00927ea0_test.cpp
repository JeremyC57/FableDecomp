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

class CTCShop { public:
    bool open, permanent; int removed, displayed; long state;
    bool IsOpeningHours() { return open; }
    void RemoveStockItemsFromShop() { ++removed; }
    void StockDisplay() { ++displayed; }
    void SetStockState(long s) { state = s; }
    bool IsPermanentlyStocked() { return permanent; }
};

class CTCShopKeeper : public CTCBase { public:
    CTCShop* shop;
    CTCShop* GetShopTC() { return shop; }
};

struct CSetupWares {
    CAIBrain* PBrain;
    bool InitCreaturePosition()
    {
        CTCShopKeeper* shopKeeper;
        if (PBrain->PCreature->GetInterface(&shopKeeper, 0x90)) {
            CTCShop* shop = shopKeeper->GetShopTC();
            if (shop != 0) {
                if (shop->IsOpeningHours()) {
                    shop->RemoveStockItemsFromShop();
                    shop->StockDisplay();
                    shop->SetStockState(0);
                } else if (!shop->IsPermanentlyStocked()) {
                    shop->RemoveStockItemsFromShop();
                }
            }
        }
        return false;
    }
};

static int fail() { std::printf("00927ea0_TEST FAIL\n"); return 1; }

int main()
{
    CTCShop shop = { true, false, 0, 0, 5 };
    CTCShopKeeper keeper; keeper.shop = &shop;
    CThing creature; creature.Reset(); creature.Add(0x90, &keeper);
    CAIBrain brain; brain.PCreature = &creature;
    CSetupWares g; g.PBrain = &brain;
    if (g.InitCreaturePosition() || shop.removed != 1 || shop.displayed != 1 || shop.state != 0) return fail();
    shop.open = false; shop.permanent = true;
    if (g.InitCreaturePosition() || shop.removed != 1) return fail();
    shop.permanent = false;
    if (g.InitCreaturePosition() || shop.removed != 2 || shop.displayed != 1) return fail();
    keeper.shop = 0;
    if (g.InitCreaturePosition() || shop.removed != 2) return fail();
    CThing bare; bare.Reset(); brain.PCreature = &bare;
    if (g.InitCreaturePosition()) return fail();
    std::printf("00927ea0_TEST PASS\n");
    return 0;
}