#include <cstdio>
// Behaviour model: byte 2 becomes 2; an unchanged type is not re-applied.
static int applied = 0; static unsigned int cur = 0x11223344u;
static void Set() { unsigned int t = (cur & 0xff00ffffu) | 0x20000u; if (cur != t) { cur = t; ++applied; } }
int main()
{
    Set(); Set();
    if (applied != 1 || ((cur >> (8 * 2)) & 0xff) != 2) { std::printf("00c1f3c0_TEST FAIL\n"); return 1; }
    std::printf("00c1f3c0_TEST PASS\n");
    return 0;
}