#include <cstdio>
static int g_del = 0, g_v = 0, g_i = 0; static bool g_vok = true, g_iok = true;
struct DI { virtual ~DI() { ++g_del; } };
struct P {
    virtual void v00() {}
    unsigned char pad[0x28]; DI* data; unsigned char pad2[8]; unsigned long vs, is; unsigned char pad3[0x24]; bool up;
    bool V(unsigned long) { ++g_v; return g_vok; }
    bool I(unsigned long) { ++g_i; return g_iok; }
    bool Up(unsigned long f) {
        if (!(vs & 1)) { if (!V(f)) return false; }
        if (!(is & 1)) { if (!I(f)) return false; }
        if (f & 8) { DI* o = data; data = 0; delete o; }
        up = true; return true;
    }
};
int main()
{
    P p; p.data = new DI; p.vs = 0; p.is = 1; p.up = false;
    if (!p.Up(8) || g_v != 1 || g_i != 0 || g_del != 1 || p.data != 0 || !p.up) { std::printf("00a93320_TEST FAIL\n"); return 1; }
    p.vs = 1; p.is = 0; g_iok = false; p.up = false;
    if (p.Up(0) || g_i != 1 || p.up) { std::printf("00a93320_TEST FAIL\n"); return 1; }
    std::printf("00a93320_TEST PASS\n");
    return 0;
}
