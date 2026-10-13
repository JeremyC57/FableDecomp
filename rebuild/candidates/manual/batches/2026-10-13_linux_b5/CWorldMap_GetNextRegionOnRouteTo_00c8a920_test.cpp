#include <cstdio>
struct R { long Route() { return 3; } };
struct W { unsigned char pad[0x48]; R* r; long Get() { if (!r) return -131; return r->Route(); } };
int main()
{
    W w; w.r = 0; if (w.Get() != -131) { std::printf("00c8a920_TEST FAIL\n"); return 1; }
    R r; w.r = &r; if (w.Get() != 3) { std::printf("00c8a920_TEST FAIL\n"); return 1; }

    std::printf("00c8a920_TEST PASS\n");
    return 0;
}
