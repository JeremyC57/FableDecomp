// Behaviour test for 00927df0: body copied from src/00927df0.cpp (CAIStateGroup_SetupWares::IsRunnable), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase;
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
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x45];
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
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    virtual bool IsRunnable() = 0;
    CAIBrain* PBrain;
};
// observable stub
static int g_open, g_perm, g_setCalls, g_lastSet, g_openCalls;
struct CTCShop {
    unsigned char _pad[0xc];
    long StockState;
    bool IsPermanentlyStocked() { return g_perm != 0; }
    bool IsOpeningHours() { ++g_openCalls; return g_open != 0; }
    void SetStockState(long state) { ++g_setCalls; g_lastSet = state; StockState = state; }
};
static CTCShop* g_shop;
struct CTCShopKeeper {
    CTCShop* GetShopTC() { return g_shop; }
};
struct CAIStateGroup_SetupWares : CAIStateGroupBase {
    virtual bool IsRunnable();
};
bool CAIStateGroup_SetupWares::IsRunnable()
{
    CTCShopKeeper* keeper;
    PBrain->PCreature->GetInterface(&keeper, 0x90);
    CTCShop* shop = keeper->GetShopTC();
    if (shop != 0) {
        if ((shop->IsOpeningHours() && shop->StockState == 1) || (!shop->IsOpeningHours() && shop->StockState == 0) ||
            shop->StockState == 2) {
            if (shop->IsPermanentlyStocked()) {
                if (shop->IsOpeningHours())
                    shop->SetStockState(0);
                else
                    shop->SetStockState(1);
                return false;
            }
            return true;
        }
    }
    return false;
}
static void reset(int open, int perm, long state)
{
    g_open = open; g_perm = perm; g_setCalls = 0; g_lastSet = -1; g_openCalls = 0;
    g_shop->StockState = state;
}
int main()
{
    CTCShopKeeper keeper;
    CThing thing; memset(&thing, 0, sizeof(thing));
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0x90; entries[1].component = (CTCBase*)&keeper;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    thing.m_presentBits[0x90 >> 5] |= 1UL << (0x90 & 31);
    CAIBrain brain; brain.PCreature = &thing;
    CAIStateGroup_SetupWares sg; sg.PBrain = &brain;
    CTCShop shop; memset(&shop, 0, sizeof(shop));
    // 1. no shop component: not runnable
    g_shop = 0; g_open = 1; g_perm = 0; g_setCalls = 0;
    CHECK(sg.IsRunnable() == false); CHECK(g_setCalls == 0);
    g_shop = &shop;
    // 2. open and set out (state 0): in step, not runnable
    reset(1, 0, 0); CHECK(sg.IsRunnable() == false); CHECK(g_setCalls == 0);
    // 3. open but packed away (state 1): runnable
    reset(1, 0, 1); CHECK(sg.IsRunnable() == true); CHECK(g_setCalls == 0);
    // 4. closed but still set out (state 0): runnable
    reset(0, 0, 0); CHECK(sg.IsRunnable() == true);
    // 5. closed and packed (state 1): not runnable
    reset(0, 0, 1); CHECK(sg.IsRunnable() == false);
    // 6. mid-change (state 2): runnable regardless of hours
    reset(0, 0, 2); CHECK(sg.IsRunnable() == true);
    reset(1, 0, 2); CHECK(sg.IsRunnable() == true);
    // 7. permanently stocked, open: snapped to state 0, not runnable
    reset(1, 1, 1); CHECK(sg.IsRunnable() == false); CHECK(g_setCalls == 1); CHECK(g_lastSet == 0); CHECK(shop.StockState == 0);
    // 8. permanently stocked, closed: snapped to state 1, not runnable
    reset(0, 1, 0); CHECK(sg.IsRunnable() == false); CHECK(g_setCalls == 1); CHECK(g_lastSet == 1); CHECK(shop.StockState == 1);
    // 9. permanently stocked but already in step: no snap
    reset(1, 1, 0); CHECK(sg.IsRunnable() == false); CHECK(g_setCalls == 0);
    if (fails == 0) printf("00927df0_TEST PASS\n");
    else printf("00927df0_TEST FAIL (%d)\n", fails);
    return fails;
}