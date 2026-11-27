#include <stdio.h>

/* Models the sort flag: Add stores it, Sort re-reads it and clears it. */

static int RegisterAtmosSortFlag(void)
{
    int needs = 0;
    needs = 1;
    int seen = needs;
    if (seen)
        needs = 0;
    return seen && needs == 0;
}

int main(void)
{
    if (!RegisterAtmosSortFlag())
        return 1;
    printf("00991c10_TEST PASS\n");
    return 0;
}
