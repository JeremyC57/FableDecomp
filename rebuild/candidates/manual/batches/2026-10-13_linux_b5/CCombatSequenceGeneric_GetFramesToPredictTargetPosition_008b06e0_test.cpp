#include <cstdio>
static long g_fps = 15; struct D { unsigned char pad[0x60]; float s; };
struct C { unsigned char pad[0x28]; D* d; long Frames() { return (long)(g_fps * d->s); } };
int main()
{
    D d; d.s = 2.5f; C c; c.d = &d; if (c.Frames() != 37) { std::printf("008b06e0_TEST FAIL\n"); return 1; }

    std::printf("008b06e0_TEST PASS\n");
    return 0;
}
