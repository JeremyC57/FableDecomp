// Behaviour test for RandomBelow (0x00C42A20): rand() * n / 32768 always lands in [0, n).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static unsigned long RandomBelow(unsigned long n, int r) { return (unsigned long)(r * (double)n * (1.0 / 32768)); }
int main()
{
    bool ok = RandomBelow(10, 0) == 0 && RandomBelow(10, 32767) == 9 && RandomBelow(4, 16384) == 2 && RandomBelow(0x80000001UL, 32767) < 0x80000001UL;
    printf(ok ? "00c42a20_TEST PASS\n" : "00c42a20_TEST FAIL\n");
    return ok ? 0 : 1;
}