// Row dual-build test: GFROR13 0x00497890 (in-row two-path: the retail asm `ror` and the FABLE_PORTABLE rotate).
// tools/decomp_pipeline/portable_twins_check.py builds this driver with VC7.1 as-is and with /DFABLE_PORTABLE and
// requires the two outputs to be identical.
#include <stdio.h>
#include "../../../src/compiled/00/49/global_GFROR13_00497890.cpp"
int main()
{
    static const unsigned long in[] = {0ul, 1ul, 0x1FFFul, 0x2000ul, 0x80000000ul, 0xFFFFFFFFul, 0x12345678ul, 0xDEADBEEFul};
    for (int i = 0; i < (int)(sizeof in / sizeof in[0]); ++i)
        printf("%08lX -> %08lX\n", in[i], (unsigned long)GFROR13(in[i]));
    printf("DONE\n");
    return 0;
}
