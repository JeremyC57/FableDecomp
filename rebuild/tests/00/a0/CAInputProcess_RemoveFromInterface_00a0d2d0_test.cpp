#include <cstdio>

static void* g_removed = 0; static int g_hook = 0;
struct Mgr { void RemoveInputProcess(void* p) { g_removed = p; } };
struct Proc { virtual void v00(){} virtual void v04(){} virtual void v08(){} virtual void OnRemoved() { ++g_hook; }
    Mgr* PPlayerInterface;
    void RemoveFromInterface() { if (PPlayerInterface) { OnRemoved(); PPlayerInterface->RemoveInputProcess(this); PPlayerInterface = 0; } } };
int main()
{
    Mgr m; Proc p; p.PPlayerInterface = 0; p.RemoveFromInterface(); if (g_hook != 0) { std::printf("00a0d2d0_TEST FAIL\n"); return 1; }
    p.PPlayerInterface = &m; p.RemoveFromInterface(); if (g_hook != 1 || g_removed != &p || p.PPlayerInterface != 0) { std::printf("00a0d2d0_TEST FAIL\n"); return 1; }
    std::printf("00a0d2d0_TEST PASS\n");
    return 0;
}