#include <cstdio>

struct V { bool patch; virtual void v00(){} virtual void v04(){} virtual void v08(){} virtual void v0c(){} virtual bool IsPatch() { return patch; } };
struct N { unsigned char pad[0xa0]; V* SubNode; V* GetSplitBranch() { if (SubNode && !SubNode->IsPatch()) return SubNode; return 0; } };
int main()
{
    V v; N n; n.SubNode = 0; if (n.GetSplitBranch()) { std::printf("00bf6160_TEST FAIL\n"); return 1; }
    n.SubNode = &v; v.patch = true; if (n.GetSplitBranch()) { std::printf("00bf6160_TEST FAIL\n"); return 1; }
    v.patch = false; if (n.GetSplitBranch() != &v) { std::printf("00bf6160_TEST FAIL\n"); return 1; }
    std::printf("00bf6160_TEST PASS\n");
    return 0;
}