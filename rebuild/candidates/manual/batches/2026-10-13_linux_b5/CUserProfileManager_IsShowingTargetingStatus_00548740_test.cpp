#include <cstdio>
struct P { unsigned char p0[0xc]; bool use; unsigned char p1[0x6c-0xd]; bool cur; unsigned char p2[0x134-0x6d]; bool nw;
    bool Get() { if (use) return nw; return cur; } };
int main()
{
    P p; p.cur = true; p.nw = false; p.use = false; if (!p.Get()) { std::printf("00548740_TEST FAIL\n"); return 1; }
    p.use = true; if (p.Get()) { std::printf("00548740_TEST FAIL\n"); return 1; }

    std::printf("00548740_TEST PASS\n");
    return 0;
}
