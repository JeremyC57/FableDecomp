#include <cstdio>
#include <cstring>
// Behaviour of 0x00750960 (chunk 901 piece): slot 10 forwards to slot 9 (the frame update at 0x00750780)
static int g_frames = 0;
struct CTCChest_Calls {
    virtual long v00() { return 0; }
    virtual long v01() { return 1; }
    virtual long v02() { return 2; }
    virtual long v03() { return 3; }
    virtual long v04() { return 4; }
    virtual long v05() { return 5; }
    virtual long v06() { return 6; }
    virtual long v07() { return 7; }
    virtual long v08() { return 8; }
    virtual void FrameUpdate() { ++g_frames; }
    virtual void vfunc_10();
};
void CTCChest_Calls::vfunc_10() { FrameUpdate(); }
int main()
{
    CTCChest_Calls o; o.vfunc_10(); o.vfunc_10(); o.vfunc_10();
    if (g_frames != 3) { std::printf("00750960_TEST FAIL\n"); return 1; }
    std::printf("00750960_TEST PASS\n"); return 0;
}
