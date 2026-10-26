// Behaviour test for 0x005B7049 CTCInventoryClothing::~CTCInventoryClothing
#include <cstdio>
#include <cstdlib>
static int g_order[16], g_n = 0;
static void Log(int v) { g_order[g_n++] = v; }
class CThing { public: void Kill(bool) { Log(1); } };
struct CVec { void* _M_start; ~CVec() { if (_M_start) { std::free(_M_start); Log(6); } } };
struct CMapA { ~CMapA() { Log(5); } };
struct CMapB { ~CMapB() { Log(4); } };
struct CMapC { ~CMapC() { Log(3); } };
struct CInvBase { virtual ~CInvBase() { Log(8); } };
struct CBase { virtual ~CBase() { Log(7); } };
struct CClothing : CInvBase, CBase {
    CVec ClothingMenuEntries; CMapA CurrentClothingSelected; CMapB MorphPrimitiveHandles;
    CThing* PMannequin; CMapC ClothingItemMap;
    void ClearClothingMenuEntries() { Log(2); }
    ~CClothing()
    {
        if (PMannequin)
            PMannequin->Kill(false);
        ClearClothingMenuEntries();
    }
};
int main()
{
    static const int want[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    CThing t;
    { CClothing* c = new CClothing; c->ClothingMenuEntries._M_start = std::malloc(4); c->PMannequin = &t; delete (CInvBase*)c; }
    bool ok = g_n == 8;
    for (int i = 0; ok && i < 8; ++i) if (g_order[i] != want[i]) ok = false;
    g_n = 0;
    { CClothing c; c.ClothingMenuEntries._M_start = 0; c.PMannequin = 0; }
    if (g_n != 6 || g_order[0] != 2) ok = false;
    if (!ok) { std::printf("005b7049_TEST FAIL\n"); return 1; }
    std::printf("005b7049_TEST PASS\n");
    return 0;
}