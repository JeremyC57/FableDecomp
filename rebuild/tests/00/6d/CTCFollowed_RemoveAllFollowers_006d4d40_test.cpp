#include <cstdio>
static int g_step = 0, g_followersAt = -1, g_aiAt = -1, g_refreshAt = -1; static long g_refreshFlag = -1;
struct CFollowerList { int size; void clear() { size = 0; g_followersAt = g_step++; } };
struct CAIFollowerList { int size; void clear() { size = 0; g_aiAt = g_step++; } };
class CPlayerGui { public: void Refresh(long flag) { g_refreshFlag = flag; g_refreshAt = g_step++; } };
static CPlayerGui g_gui; static CPlayerGui* g_pPlayerGui = &g_gui;
class CTCFollowed { public:
    CFollowerList Followers; CAIFollowerList AIFollowers;
    void RemoveAllFollowers() { Followers.clear(); AIFollowers.clear(); g_pPlayerGui->Refresh(0); }
};
int main()
{
    CTCFollowed f; f.Followers.size = 3; f.AIFollowers.size = 2;
    f.RemoveAllFollowers();
    if (f.Followers.size != 0 || f.AIFollowers.size != 0 || g_followersAt != 0 || g_aiAt != 1 || g_refreshAt != 2 || g_refreshFlag != 0) { std::printf("006d4d40_TEST FAIL\n"); return 1; }
    std::printf("006d4d40_TEST PASS\n");
    return 0;
}