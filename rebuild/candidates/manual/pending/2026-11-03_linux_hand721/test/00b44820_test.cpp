// Behaviour test for 0x00B44820 (C++ shell of the retail SSE lighting block): the vertex count is turned into a count of
// four-vertex quads with signed division (`cdq ; and edx,3 ; add eax,edx ; sar eax,2`), and the block walks that many quads,
// stepping the float streams by 16 bytes and the colour / position streams by 64 bytes per quad.
#include <stdio.h>
static long QuadCount(long count) { count /= 4; return count; }
static long Walk(long count, long* colourBytes, long* streamBytes)
{
    long quads = QuadCount(count), i = 0;
    *colourBytes = 0; *streamBytes = 0;
    for (; i < quads; ++i) { *colourBytes += 0x40; *streamBytes += 0x10; }
    return i;
}
int main()
{
    long c, s;
    int ok = 1;
    ok &= QuadCount(16) == 4 && QuadCount(7) == 1 && QuadCount(3) == 0 && QuadCount(-5) == -1;
    ok &= Walk(12, &c, &s) == 3 && c == 0xC0 && s == 0x30;
    ok &= Walk(2, &c, &s) == 0 && c == 0;
    printf(ok ? "00b44820_TEST PASS\n" : "00b44820_TEST FAIL\n");
    return ok ? 0 : 1;
}
