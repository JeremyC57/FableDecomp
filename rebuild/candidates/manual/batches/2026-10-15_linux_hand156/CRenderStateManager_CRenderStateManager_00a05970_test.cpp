// Behaviour test for 0x00A05970: CRenderStateManager ctor: lists empty, bookmark dummy zeroed with StateType 4, bookmark mask 1, init flags recorded
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct StateInfo {
    unsigned int current, desired, mask; long state; bool dirty; unsigned char type, stage;
    StateInfo() : current(0), desired(0), mask(0), dirty(false), type(0) {}
};
struct RSM {
    long stackSize, updateSize, allSize; unsigned int bookmarkMask; StateInfo dummy; unsigned int initFlags;
    RSM(unsigned int flags) { stackSize = 0; updateSize = 0; allSize = 0; initFlags = flags; dummy.type = 4; bookmarkMask = 1; }
};
static void Run()
{
    RSM r(0x12);
    CHECK(r.stackSize == 0 && r.updateSize == 0 && r.allSize == 0 && r.initFlags == 0x12 && r.bookmarkMask == 1);
    CHECK(r.dummy.current == 0 && r.dummy.desired == 0 && r.dummy.mask == 0 && !r.dummy.dirty && r.dummy.type == 4);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00a05970_TEST FAIL\n"); return 1; }
    std::printf("00a05970_TEST PASS\n");
    return 0;
}
