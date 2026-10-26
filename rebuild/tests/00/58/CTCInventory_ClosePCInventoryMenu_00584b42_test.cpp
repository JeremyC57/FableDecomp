// Behaviour test for 00584b42: body copied from src/00584b42.cpp (CTCInventory::ClosePCInventoryMenu), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
struct CPresenceBits {
    unsigned long w[9];
    bool test(long bit) { return (w[bit >> 5] & (1UL << (bit & 31))) != 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;
    CInterfaceMap m_interfaces;
    bool HasTC(long wanted) { return m_present.test(wanted); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
static int g_closeCalls = 0; static void* g_closeThis = 0;
class CTCInGameMenu { public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {}
    virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {}
    virtual void v15() {} virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {}
    virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {} virtual void v24() {}
    virtual void v25() {} virtual void v26() {} virtual void v27() {} virtual void v28() {} virtual void v29() {}
    virtual void v30() {}
    virtual void ClosePCInventoryMenu() { ++g_closeCalls; g_closeThis = this; }
};
class CTCInventory { public:
    virtual void v00() {}
    CThing* ParentThing;
    void ClosePCInventoryMenu();
};
void CTCInventory::ClosePCInventoryMenu()
{
    CThing* thing = ParentThing;
    if (thing->HasTC(0x12))
        thing->GetTC<CTCInGameMenu>(0x12)->ClosePCInventoryMenu();
}
int main()
{
    CThing thing; memset(&thing, 0, sizeof(thing)); CTCInGameMenu menu;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0x12, (CTCBase*)&menu } };
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    CTCInventory inv; inv.ParentThing = &thing;
    // 1. no in-game menu component: nothing
    inv.ClosePCInventoryMenu(); CHECK(g_closeCalls == 0);
    // 2. component present: its virtual ClosePCInventoryMenu runs
    thing.m_present.w[0x12 >> 5] = 1UL << (0x12 & 31);
    inv.ClosePCInventoryMenu(); CHECK(g_closeCalls == 1); CHECK(g_closeThis == &menu);
    if (fails == 0) printf("00584b42_TEST PASS\n"); else printf("00584b42_TEST FAIL\n");
    return fails;
}