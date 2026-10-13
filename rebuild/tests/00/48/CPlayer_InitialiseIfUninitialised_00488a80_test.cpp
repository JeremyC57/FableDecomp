#include <cstdio>

static int g_setup = 0;
struct Loader { bool busy; bool IsLoading() { return busy; } };
struct Comp { unsigned char pad[0x28]; Loader* Loader_; };
struct P { unsigned char pad[0x14]; Comp* Component; unsigned char p2[0x216 - 0x18]; bool Init;
    void Setup() { ++g_setup; }
    void Run() { if (!Init) { if (!Component->Loader_->IsLoading()) { Init = true; Setup(); } } } };
int main()
{
    Loader l; Comp c; c.Loader_ = &l; P p; p.Component = &c; p.Init = false; l.busy = true; p.Run(); if (p.Init || g_setup) { std::printf("00488a80_TEST FAIL\n"); return 1; }
    l.busy = false; p.Run(); if (!p.Init || g_setup != 1) { std::printf("00488a80_TEST FAIL\n"); return 1; }
    p.Run(); if (g_setup != 1) { std::printf("00488a80_TEST FAIL\n"); return 1; }
    std::printf("00488a80_TEST PASS\n");
    return 0;
}