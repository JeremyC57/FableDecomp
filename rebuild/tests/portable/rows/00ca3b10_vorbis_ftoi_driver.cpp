// Row dual-build test: vorbis_ftoi 0x00CA3B10 (libvorbis os.h) (the row reaches its retail asm through a shared helper header: built as-is = the asm path, /DFABLE_PORTABLE = the header's C++ twin; 2026-11-05 asm migration).
#include <stdio.h>
#include "../../../src/compiled/00/ca/global_FtoL_00ca3b10.cpp"
int main()
{
    static const double in[] = {0.0, -0.0, 0.5, -0.5, 1.5, 2.5, -2.5, 3.49999999999, 1e-300, -1e-300, 2147483647.0,
                                2147483647.4, 2147483647.5, -2147483648.0, -2147483648.5, -2147483648.6, 4e9, -4e9, 1e300};
    for (int i = 0; i < (int)(sizeof in / sizeof in[0]); ++i)
        printf("%.17g -> %08lX\n", in[i], (unsigned long)vorbis_ftoi(in[i]));
    printf("DONE\n");
    return 0;
}
