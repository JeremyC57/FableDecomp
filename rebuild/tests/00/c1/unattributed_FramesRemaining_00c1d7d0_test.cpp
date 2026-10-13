#include <cstdio>
struct C { unsigned char pad[0x20]; long end; long Q() { return 3; } long R() { return end - Q(); } };
int main()
{
    C c; c.end = 10; if (c.R() != 7) { std::printf("00c1d7d0_TEST FAIL\n"); return 1; }

    std::printf("00c1d7d0_TEST PASS\n");
    return 0;
}