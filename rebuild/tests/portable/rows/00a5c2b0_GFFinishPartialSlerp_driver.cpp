// Row dual-build test: NVectorMath::GFFinishPartialSlerp 0x00A5C2B0 (in-row two-path: the SSE loop and its per-lane C++
// twin). Inputs are small dyadic values, so every product and sum is exact in float and the SSE single-precision
// rounding and the x87 evaluation of the twin agree bit for bit (general inputs agree to float rounding).
#include <stdio.h>
#include "../../../src/compiled/00/a5/NVectorMath_GFFinishPartialSlerp_00a5c2b0.cpp"
int main()
{
    __declspec(align(16)) CQuaternion out[6], in[6], w[6];
    for (int i = 0; i < 6; ++i) {
        out[i].X = 1.0f + i; out[i].Y = -2.0f * i; out[i].Z = 0.5f; out[i].W = 0.25f * i;
        in[i].X = -1.0f; in[i].Y = 3.0f + i; in[i].Z = 0.125f * i; in[i].W = 8.0f;
        w[i].X = 0.75f; w[i].Y = 0.25f + 0.0625f * i; w[i].Z = 99.0f; w[i].W = -99.0f;
    }
    NVectorMath::GFFinishPartialSlerp(out, 2 * sizeof(CQuaternion), in, sizeof(CQuaternion), w, sizeof(CQuaternion), 3);
    for (int i = 0; i < 6; ++i)
        printf("%d: %.9g %.9g %.9g %.9g\n", i, out[i].X, out[i].Y, out[i].Z, out[i].W);
    printf("DONE\n");
    return 0;
}
