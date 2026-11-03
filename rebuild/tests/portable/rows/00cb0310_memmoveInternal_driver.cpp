// Row dual-build test: memmoveInternal 0x00CB0310 (DirectShow wxutil.cpp; in-row two-path: the _X86_ rep movs blocks and memmove).
#include <stdio.h>
#include "../../../src/compiled/00/cb/global_memmoveInternal_00cb0310.cpp"
static void run(int dstOff, int srcOff, unsigned int count)
{
    unsigned char buf[64];
    for (int i = 0; i < 64; ++i) buf[i] = (unsigned char)i;
    void* r = memmoveInternal(buf + dstOff, buf + srcOff, count);
    printf("%d %d %u ret+%d:", dstOff, srcOff, count, (int)((unsigned char*)r - buf));
    for (int i = 0; i < 64; ++i) printf(" %02X", buf[i]);
    printf("\n");
}
int main()
{
    run(0, 8, 13); run(8, 0, 13); run(3, 4, 30); run(4, 3, 30); run(10, 10, 5); run(0, 32, 32); run(32, 0, 31); run(5, 1, 0);
    printf("DONE\n");
    return 0;
}
