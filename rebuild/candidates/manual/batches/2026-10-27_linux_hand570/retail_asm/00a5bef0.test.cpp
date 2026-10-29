// Behaviour test for NVectorMath::GFInterpolateVectors 0x00A5BEF0: both paths compute out = from + (to - from) * w per
// element with independent byte strides (the SSE kernel as from*(1-w) + to*w, the x87 path through C4DVector operators).
#include <cstdio>
class C4DVector {
public:
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
    const C4DVector operator-(const C4DVector& o) const { return C4DVector(X - o.X, Y - o.Y, Z - o.Z, W - o.W); }
    const C4DVector operator+(const C4DVector& o) const { return C4DVector(X + o.X, Y + o.Y, Z + o.Z, W + o.W); }
    const C4DVector operator*(float s) const { return C4DVector(X * s, Y * s, Z * s, W * s); }
    const C4DVector Interpolate(const C4DVector& to, float t) const { return (to - *this) * t + *this; }
};
static bool g_SSEAvailable = false;
static void Interp(C4DVector* out, unsigned int outStride, const C4DVector* from, unsigned int fromStride,
                   const C4DVector* to, unsigned int toStride, const float* weights, unsigned int weightStride,
                   unsigned int count)
{
    if (g_SSEAvailable) {           // the kernel's arithmetic: from * (1 - w) + to * w
        for (unsigned int n = count; n != 0; --n) {
            float w = *weights, iw = 1.0f - w;
            out->X = from->X * iw + to->X * w; out->Y = from->Y * iw + to->Y * w;
            out->Z = from->Z * iw + to->Z * w; out->W = from->W * iw + to->W * w;
            out = (C4DVector*)((char*)out + outStride);
            from = (const C4DVector*)((const char*)from + fromStride);
            to = (const C4DVector*)((const char*)to + toStride);
            weights = (const float*)((const char*)weights + weightStride);
        }
    } else {
        C4DVector* pOut = out; const C4DVector* pFrom = from; const C4DVector* pTo = to; const float* pWeight = weights;
        for (unsigned int i = 0; i < count; ++i) {
            *pOut = pFrom->Interpolate(*pTo, *pWeight);
            pOut = (C4DVector*)((char*)pOut + outStride);
            pFrom = (const C4DVector*)((const char*)pFrom + fromStride);
            pTo = (const C4DVector*)((const char*)pTo + toStride);
            pWeight = (const float*)((const char*)pWeight + weightStride);
        }
    }
}
static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
int main()
{
    C4DVector from[7], to[7], out[14];
    float weights[14];
    for (int i = 0; i < 7; ++i) {
        from[i] = C4DVector((float)i, 2.0f * i, -1.0f * i, 10.0f);
        to[i] = C4DVector((float)i + 4.0f, 0.0f, 3.0f * i, 20.0f);
        weights[2 * i] = 0.125f * i; weights[2 * i + 1] = 99.0f;
    }
    for (int pass = 0; pass < 2; ++pass) {
        g_SSEAvailable = pass != 0;
        for (int i = 0; i < 14; ++i) out[i] = C4DVector(-7.0f, -7.0f, -7.0f, -7.0f);
        // out every other slot, weights every other float, 7 elements (unrolled 4 + remainder 3 in retail)
        Interp(out, 2 * sizeof(C4DVector), from, sizeof(C4DVector), to, sizeof(C4DVector), weights, 2 * sizeof(float), 7);
        for (int i = 0; i < 7; ++i) {
            float w = 0.125f * i;
            const C4DVector& r = out[2 * i];
            if (!Near(r.X, from[i].X + (to[i].X - from[i].X) * w) || !Near(r.Y, from[i].Y + (to[i].Y - from[i].Y) * w) ||
                !Near(r.Z, from[i].Z + (to[i].Z - from[i].Z) * w) || !Near(r.W, 10.0f + 10.0f * w) ||
                out[2 * i + 1].X != -7.0f) {
                std::printf("00a5bef0_TEST FAIL\n");
                return 1;
            }
        }
        Interp(out, sizeof(C4DVector), from, sizeof(C4DVector), to, sizeof(C4DVector), weights, sizeof(float), 0);
        if (out[0].X != from[0].X) { std::printf("00a5bef0_TEST FAIL\n"); return 1; }
    }
    std::printf("00a5bef0_TEST PASS\n");
    return 0;
}
