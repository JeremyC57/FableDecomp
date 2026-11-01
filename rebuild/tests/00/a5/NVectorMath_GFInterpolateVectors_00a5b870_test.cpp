// Test for NVectorMath::GFInterpolateVectors (KKM) 0x00A5B870: out = out * (1 - t) + from * t over `count` 4D vectors
// with independent byte strides (the x87 fallback's Interpolate form (from - out) * t + out gives the same values).
#include <cstdio>
#include <cmath>
struct V4 { float X, Y, Z, W; };
static void GFInterpolateVectors(V4* out, unsigned long outStride, const V4* from, unsigned long fromStride, unsigned long count, float t)
{
    V4* pOut = out; const V4* pFrom = from;
    for (unsigned long i = 0; i < count; ++i) {
        pOut->X = (pFrom->X - pOut->X) * t + pOut->X; pOut->Y = (pFrom->Y - pOut->Y) * t + pOut->Y;
        pOut->Z = (pFrom->Z - pOut->Z) * t + pOut->Z; pOut->W = (pFrom->W - pOut->W) * t + pOut->W;
        pOut = (V4*)((char*)pOut + outStride); pFrom = (const V4*)((const char*)pFrom + fromStride);
    }
}
static bool near(float a, float b) { return fabs(a - b) < 1e-5f; }
int main() {
    V4 out[3] = { { 0, 0, 0, 0 }, { 1, 1, 1, 1 }, { 2, 2, 2, 2 } };
    V4 from[6] = { { 4, 8, 12, 16 }, { 9, 9, 9, 9 }, { 5, 5, 5, 5 }, { 9, 9, 9, 9 }, { 0, 0, 0, 0 }, { 9, 9, 9, 9 } };
    GFInterpolateVectors(out, sizeof(V4), from, 2 * sizeof(V4), 3, 0.25f);   // from stride skips the 9s
    bool ok = near(out[0].X, 1.0f) && near(out[0].Y, 2.0f) && near(out[0].Z, 3.0f) && near(out[0].W, 4.0f)
           && near(out[1].X, 2.0f) && near(out[1].W, 2.0f) && near(out[2].X, 1.5f) && near(out[2].Z, 1.5f);
    V4 one = { 1, 2, 3, 4 }; V4 target = { 5, 6, 7, 8 };
    GFInterpolateVectors(&one, 0, &target, 0, 0, 0.5f);                        // zero count leaves the vector alone
    ok = ok && one.X == 1.0f && one.W == 4.0f;
    GFInterpolateVectors(&one, 0, &target, 0, 1, 1.0f);                        // t = 1 copies
    ok = ok && one.X == 5.0f && one.Y == 6.0f && one.Z == 7.0f && one.W == 8.0f;
    if (ok) printf("00a5b870_TEST PASS\n"); else printf("00a5b870_TEST FAIL\n");
    return ok ? 0 : 1;
}