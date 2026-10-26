// CTCShop::SetStockState(long state) -- on a change: store it, tell the keeper's CTCShopKeeper component (interface 0x90) to repress opinions unless the state is 1, then refresh the player GUI
// Leaf: SetStockState
// Retail: Fable.exe 0x00773C00, 140 bytes (`ret 4`; the GUI refresh is a `mov [esp+4],0 ; jmp` tail call).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (inline GetInterface), CTCShopKeeper::RepressOpinions(bool) @ 0x007996B0, CPlayerGui::Refresh(long)
// @ 0x004378C0 (`mov ecx,[ecx+160h] ; jmp`; manifest "_Cons_val<...>" is propagated) on the global @ 0x013B8790.
// Layout: retail CTCShop: StockState +0x0C, PShopKeeper +0x10; CThing presence bitset +0x20 (0x90 = bit 16 of +0x30),
// interface map +0x44, dead flag +0x91 bit 0.
// Shape: the new state is stored before the keeper tests; `state != 1` is pushed as `setne dl`. The shop-keeper
// component comes from the out-parameter GetInterface whose entry pick is one end/hit ternary: the component loads
// straight into ecx before the bool is formed (a HasInterface test + GetTC, or the if-form pick, forms the bool first).
struct CTCBaseAny;
struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBaseAny* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long key = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +4
    CThing* GetPItem() const;                       // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCShopKeeper { public:
    void RepressOpinions(bool repress);             // 0x007996B0
};
class CPlayerGui { public:
    void Refresh(long flag);                        // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790
class CTCShop { public:
    virtual void v00();
    void*                   ParentThing;            // +0x04 (CTCBase)
    bool                    ToBeRemoved;            // +0x08
    bool                    IsInGlobalUpdateTCs;    // +0x09
    long                    StockState;             // +0x0c
    CBaseIntelligentPointer PShopKeeper;            // +0x10
    void SetStockState(long state);
};

void CTCShop::SetStockState(long state)
{
    if (state != StockState) {
        StockState = state;
        if (IS_THING_ALIVE(PShopKeeper)) {
            CThing* keeper = PShopKeeper.GetPItem();
            CTCShopKeeper* shopKeeper;
            if (keeper->GetInterface(&shopKeeper, 0x90))
                shopKeeper->RepressOpinions(state != 1);
        }
        g_pPlayerGui->Refresh(0);
    }
}
// ---- harness ----
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
void CBaseIntelligentPointer::v00() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static int g_represses = 0; static bool g_lastRepress = false; static const void* g_keeperThis = 0;
void CTCShopKeeper::RepressOpinions(bool repress) { ++g_represses; g_lastRepress = repress; g_keeperThis = this; }
static int g_refreshes = 0; static long g_lastRefresh = -1;
void CPlayerGui::Refresh(long flag) { ++g_refreshes; g_lastRefresh = flag; }
static CPlayerGui g_gui;
CPlayerGui* g_pPlayerGui = &g_gui;
void CTCShop::v00() { }
static CThing g_keeper;
static CTCShopKeeper g_shopKeeper;
static CInterfaceMap::SEntry g_entries[2];
int main()
{
    g_entries[0].type = 0x40; g_entries[0].component = 0;
    g_entries[1].type = 0x90; g_entries[1].component = (CTCBaseAny*)&g_shopKeeper;
    g_keeper.m_interfaces.first = g_entries; g_keeper.m_interfaces.last = g_entries + 2;
    g_keeper.m_presentBits[0x90 >> 5] = 1UL << (0x90 & 31);
    CTCShop shop;
    shop.StockState = 0;
    shop.PShopKeeper.m_pItem = &g_keeper;
    shop.SetStockState(2);
    CHECK(shop.StockState == 2 && g_represses == 1 && g_lastRepress && g_keeperThis == &g_shopKeeper && g_refreshes == 1 && g_lastRefresh == 0);
    shop.SetStockState(2);                          // unchanged: nothing
    CHECK(g_represses == 1 && g_refreshes == 1);
    shop.SetStockState(1);
    CHECK(shop.StockState == 1 && g_represses == 2 && !g_lastRepress && g_refreshes == 2);
    g_keeper.m_flags91 = 1;                         // keeper dead: refresh only
    shop.SetStockState(3);
    CHECK(shop.StockState == 3 && g_represses == 2 && g_refreshes == 3);
    g_keeper.m_flags91 = 0; g_keeper.m_presentBits[0x90 >> 5] = 0;   // not a shop keeper
    shop.SetStockState(4);
    CHECK(g_represses == 2 && g_refreshes == 4);
    shop.PShopKeeper.m_pItem = 0;                   // no keeper
    shop.SetStockState(5);
    CHECK(g_represses == 2 && g_refreshes == 5 && shop.StockState == 5);
    if (fails == 0) printf("00773c00_TEST PASS\n");
    return fails;
}