#include <cstdio>

static int g_up = 0;
struct N { unsigned char pad[0x9c]; N* Parent; unsigned char p2[0x44]; bool Visible; bool Updated;
    void UpdateParentChainVisibilityFlag() { ++g_up; }
    void Set(bool v) { if (Visible != v) { Visible = v; Updated = true; if (Parent) Parent->UpdateParentChainVisibilityFlag(); } } };
int main()
{
    N parent, n; n.Parent = &parent; n.Visible = false; n.Updated = false; n.Set(false); if (n.Updated || g_up) { std::printf("00bf7370_TEST FAIL\n"); return 1; }
    n.Set(true); if (!n.Visible || !n.Updated || g_up != 1) { std::printf("00bf7370_TEST FAIL\n"); return 1; }
    std::printf("00bf7370_TEST PASS\n");
    return 0;
}