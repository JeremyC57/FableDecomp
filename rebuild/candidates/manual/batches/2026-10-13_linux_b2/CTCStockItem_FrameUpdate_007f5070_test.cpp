#include <cstdio>

static int g_upd = 0; static void* g_holder = 0;
struct Th { void SetAsFrameUpdated() { ++g_upd; } };
struct S { void* vt; Th* parent; unsigned char pad[4]; bool ForSale; unsigned char p2[3]; long price; long h0; long h1;
    void* GetStockHolder() { return g_holder; }
    void FrameUpdate() { if (GetStockHolder() && ForSale && h1 == 0) parent->SetAsFrameUpdated(); } };
int main()
{
    Th t; S s; s.parent = &t; s.ForSale = true; s.h1 = 0; g_holder = 0; s.FrameUpdate(); if (g_upd) { std::printf("007f5070_TEST FAIL\n"); return 1; }
    int x; g_holder = &x; s.FrameUpdate(); if (g_upd != 1) { std::printf("007f5070_TEST FAIL\n"); return 1; }
    s.h1 = 5; s.FrameUpdate(); if (g_upd != 1) { std::printf("007f5070_TEST FAIL\n"); return 1; }
    std::printf("007f5070_TEST PASS\n");
    return 0;
}
