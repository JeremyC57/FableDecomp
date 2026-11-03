// Row dual-build test: GFGetInterpolatedColour 0x00A44250 (in-row two-path: the MMX blend and its 16-bit-lane C++ twin).
#include <stdio.h>
#include "../../../src/compiled/00/a4/global_GFGetInterpolatedColour_00a44250.cpp"
int main()
{
    static const unsigned char c[][4] = {{0, 0, 0, 0}, {255, 255, 255, 255}, {1, 2, 3, 4}, {200, 100, 50, 25}, {128, 127, 129, 255}};
    static const unsigned long w[] = {0ul, 1ul, 128ul, 255ul, 256ul, 300ul, 0x10080ul, 0xFFFFFFFFul};
    int n = (int)(sizeof c / sizeof c[0]);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < (int)(sizeof w / sizeof w[0]); ++k) {
                CRGBColour a(c[i][0], c[i][1], c[i][2], c[i][3]), b(c[j][0], c[j][1], c[j][2], c[j][3]);
                CRGBColour r = GFGetInterpolatedColour(a, b, w[k]);
                printf("%d %d %lu -> %02X %02X %02X %02X\n", i, j, w[k], r.R, r.G, r.B, r.A);
            }
    printf("DONE\n");
    return 0;
}
