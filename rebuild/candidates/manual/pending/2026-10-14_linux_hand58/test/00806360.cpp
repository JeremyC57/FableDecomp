#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_calls = 0;
struct CPtr { void* vt; void* item; void* GetPItem() const { ++g_calls; return item; } };
struct CRec { CPtr p; long idx; long operator==(const CRec& o) const { if (p.GetPItem() == o.p.GetPItem() && idx == o.idx) return 1; return 0; } };
int main()
{
    int x, y; CRec a, b; a.p.item = &x; a.idx = 3; b.p.item = &x; b.idx = 3;
    if ((a == b) != 1) { std::printf("00806360_TEST FAIL\n"); return 1; }
    b.idx = 4; if ((a == b) != 0) { std::printf("00806360_TEST FAIL\n"); return 1; }
    g_calls = 0; b.p.item = &y; if ((a == b) != 0 || g_calls != 2) { std::printf("00806360_TEST FAIL\n"); return 1; }
    std::printf("00806360_TEST PASS\n");
    return 0;
}
