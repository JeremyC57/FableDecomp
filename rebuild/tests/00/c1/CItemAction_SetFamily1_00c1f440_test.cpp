#include <cstdio>
// Behaviour model: byte 2 becomes 1; an unchanged type is not re-applied.
static int applied = 0; static unsigned int cur = 0x11223344u;
static void Set() { unsigned int t = (cur & 0xff00ffffu) | 0x10000u; if (cur != t) { cur = t; ++applied; } }
int main()
{
    Set(); Set();
    if (applied != 1 || ((cur >> (8 * 2)) & 0xff) != 1) { std::printf("00c1f440_TEST FAIL\n"); return 1; }
    std::printf("00c1f440_TEST PASS\n");
    return 0;
}