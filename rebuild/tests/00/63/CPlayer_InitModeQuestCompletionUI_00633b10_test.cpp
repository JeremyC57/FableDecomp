#include <cstdio>

static int g_removed = 0; static void* g_added = 0;
struct Main { void RemoveFromInterface() { ++g_removed; } };
struct V { virtual void v00() {} virtual bool AddToInterface(void* p) { g_added = p; return true; } };
struct P { unsigned char pad[0xc]; void* PlayerInterface; unsigned char p2[0x100-0x10]; Main* MainProc; unsigned char p3[0x1d0-0x104]; V* QuestUI;
    void Init(long, void*) { MainProc->RemoveFromInterface(); QuestUI->AddToInterface(PlayerInterface); } };
int main()
{
    Main m; V v; P p; int iface; p.PlayerInterface = &iface; p.MainProc = &m; p.QuestUI = &v; p.Init(0, 0);
    if (g_removed != 1 || g_added != &iface) { std::printf("00633b10_TEST FAIL\n"); return 1; }
    std::printf("00633b10_TEST PASS\n");
    return 0;
}