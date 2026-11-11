#include <cstdio>
// Behaviour of CTCTargetingSpirit::vfunc_10 (0x004DEBF7): slot 10 forwards to slot 9.
static int g_hits = 0;
struct CTCTargetingSpirit_Calls {
    virtual long v00() { return 0; }
    virtual long v01() { return 1; }
    virtual long v02() { return 2; }
    virtual long v03() { return 3; }
    virtual long v04() { return 4; }
    virtual long v05() { return 5; }
    virtual long v06() { return 6; }
    virtual long v07() { return 7; }
    virtual long v08() { return 8; }
    virtual void vfunc_9() { ++g_hits; }
    virtual void vfunc_10();
};
void CTCTargetingSpirit_Calls::vfunc_10() { vfunc_9(); }
int main()
{
    CTCTargetingSpirit_Calls t; t.vfunc_10(); t.vfunc_10();
    if (g_hits != 2) { std::printf("004debf7_TEST FAIL\n"); return 1; }
    std::printf("004debf7_TEST PASS\n"); return 0;
}