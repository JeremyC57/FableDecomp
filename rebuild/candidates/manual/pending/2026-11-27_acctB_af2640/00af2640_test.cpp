#include <stdio.h>
/* The system transform runs only when the owner flag bit 0x40 is set. */
static int UsesSystemTransform(unsigned char flags)
{
    return (flags & 64) != 0;
}
int main(void)
{
    if (UsesSystemTransform(0))
        return 1;
    if (!UsesSystemTransform(64))
        return 1;
    if (UsesSystemTransform(1))
        return 1;
    printf("00af2640_TEST PASS\n");
    return 0;
}
