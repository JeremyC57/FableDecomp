// Behaviour test for 0x007CABD0 CTCHeroReceiveItems::AddItemToInventory: an inventory item goes into the hero's
// inventory of its type (flag passed through); a non-item, or an item whose inventory the hero lacks, is killed.
#include <cstdio>
#include <cstdlib>

class CTCBase { public: int unused; };
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
class CThing {
public:
    unsigned int  m_presentBits[9];
    CInterfaceMap m_interfaces;
    CInterfaceMap::SEntry entries[4];
    int killed;
    void Kill(bool immediately) { (void)immediately; ++killed; }
    bool TestComponent(unsigned int position) const
    {
        if (position >= 274) { std::puts("bitset"); std::abort(); }
        return (m_presentBits[position >> 5] & (1U << (position & 31))) != 0;
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1U << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap::SEntry* it = m_interfaces.LowerBound(&type);
            if (it == m_interfaces.last || interfaceType < it->type) it = m_interfaces.last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> bool GetCheckedInterface(T** ppOut, long interfaceType)
    {
        if (TestComponent(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap::SEntry* it = m_interfaces.LowerBound(&type);
            if (it == m_interfaces.last || interfaceType < it->type) it = m_interfaces.last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void Init()
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_interfaces.first = entries; m_interfaces.last = entries; killed = 0;
    }
    void Add(long type, CTCBase* c)   // types added in ascending order
    {
        m_interfaces.last->type = type; m_interfaces.last->component = c; ++m_interfaces.last;
        m_interfaces.last->type = 0x7fffffff; m_interfaces.last->component = 0;
        m_presentBits[type >> 5] |= 1U << (type & 31);
    }
};
class CTCInventoryBase : public CTCBase { public: int got; };
class CTCInventoryItem : public CTCBase {
public:
    long type;
    CTCInventoryBase* addedTo;
    bool lastFlag;
    long GetInventoryType() const { return type; }
    bool AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long price, bool c, bool d)
    {
        if (a || !b || price != 0 || c) return false;
        addedTo = inventory; lastFlag = d; ++inventory->got; return true;
    }
};
class CTCHeroReceiveItems {
public:
    CThing* ParentThing;
    void AddItemToInventory(CThing& thing, bool flag)
    {
        CTCInventoryItem* item;
        if (thing.GetInterface(&item, 0x1A)) {
            CThing* parent = ParentThing;
            CTCInventoryBase* inventory;
            if (parent->GetCheckedInterface(&inventory, item->GetInventoryType()))
                item->AddToInventory(inventory, false, true, 0, false, flag);
            else
                thing.Kill(false);
        }
        else
            thing.Kill(false);
    }
};

int main()
{
    CThing hero; hero.Init();
    CTCInventoryBase weapons; weapons.got = 0;
    hero.Add(0x30, &weapons);
    CTCHeroReceiveItems tc; tc.ParentThing = &hero;

    CThing sword; sword.Init();
    CTCInventoryItem swordItem; swordItem.type = 0x30; swordItem.addedTo = 0; swordItem.lastFlag = false;
    sword.Add(0x1A, &swordItem);
    tc.AddItemToInventory(sword, true);
    bool ok = swordItem.addedTo == &weapons && swordItem.lastFlag && weapons.got == 1 && sword.killed == 0;

    CThing book; book.Init();
    CTCInventoryItem bookItem; bookItem.type = 0x31; bookItem.addedTo = 0; bookItem.lastFlag = false;
    book.Add(0x1A, &bookItem);
    tc.AddItemToInventory(book, false);
    ok = ok && bookItem.addedTo == 0 && book.killed == 1;

    CThing rock; rock.Init();
    tc.AddItemToInventory(rock, true);
    ok = ok && rock.killed == 1 && weapons.got == 1;
    if (!ok) { std::printf("007cabd0_TEST FAIL\n"); return 1; }
    std::printf("007cabd0_TEST PASS\n");
    return 0;
}