#include <cstdio>

struct Info { bool combo; bool IsCombinationAnimation() { return combo; } };
static Info g_info; static long g_id = 0;
struct Mgr { Info* GetAnimationInfo(long id) { g_id = id; return &g_info; } };
struct DE { unsigned char pad[0x3c]; Mgr* AnimationManager; };
static Mgr g_mgr; static DE g_de;
DE* GFGetDisplayEngine() { return &g_de; }
struct Cr { bool Is(long id) { if (id <= 0) return false; Info* i = GFGetDisplayEngine()->AnimationManager->GetAnimationInfo(id); return i->IsCombinationAnimation(); } };
int main()
{
    g_de.AnimationManager = &g_mgr; g_info.combo = true; Cr c;
    if (c.Is(0) || !c.Is(7) || g_id != 7) { std::printf("00662820_TEST FAIL\n"); return 1; }
    std::printf("00662820_TEST PASS\n");
    return 0;
}
