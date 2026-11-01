// Behaviour test for NVectorMath::GFInterpolateVectors (one weight) 0x00A5BBA0: out = from + (to - from) * weight over three
// strided 4-vector streams, count elements (the x87 fallback path; the SSE kernel computes from*(1-w) + to*w, the same value).
#include <cstdio>
#include <cmath>

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
static void GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* from, unsigned long fromStride,
                                 const C4DVector* to, unsigned long toStride, unsigned long count, float weight)
{
    C4DVector* pOut = out;
    const C4DVector* pFrom = from;
    const C4DVector* pTo = to;
    for (unsigned long i = 0; i < count; ++i) {
        *pOut = pFrom->Interpolate(*pTo, weight);
        pOut = (C4DVector*)((char*)pOut + outStride);
        pFrom = (const C4DVector*)((const char*)pFrom + fromStride);
        pTo = (const C4DVector*)((const char*)pTo + toStride);
    }
}
struct Padded { C4DVector v; float pad[2]; };   // a 24-byte stride stream

int main()
{
    const int n = 7;                             // 4 unrolled + 3 remainder
    Padded from[n], to[n]; C4DVector out[n + 1];
    for (int i = 0; i < n; ++i) {
        from[i].v = C4DVector(1.f * i, 2.f * i, 3.f * i, 4.f * i);
        to[i].v = C4DVector(10.f + i, 20.f + i, 30.f + i, 40.f + i);
    }
    for (int i = 0; i <= n; ++i) out[i] = C4DVector(-1.f, -1.f, -1.f, -1.f);
    GFInterpolateVectors(out, sizeof(C4DVector), &from[0].v, sizeof(Padded), &to[0].v, sizeof(Padded), n, 0.25f);
    for (int i = 0; i < n; ++i) {
        C4DVector e = from[i].v + (to[i].v - from[i].v) * 0.25f;
        if (std::fabs(out[i].X - e.X) > 1e-5f || std::fabs(out[i].Y - e.Y) > 1e-5f ||
            std::fabs(out[i].Z - e.Z) > 1e-5f || std::fabs(out[i].W - e.W) > 1e-5f) { std::printf("00a5bba0_TEST FAIL\n"); return 1; }
    }
    if (out[n].X != -1.f) { std::printf("00a5bba0_TEST FAIL\n"); return 1; }
    GFInterpolateVectors(out, sizeof(C4DVector), &from[0].v, sizeof(Padded), &to[0].v, sizeof(Padded), 0, 0.5f);   // count 0: untouched
    if (out[0].X != from[0].v.X + (to[0].v.X - from[0].v.X) * 0.25f) { std::printf("00a5bba0_TEST FAIL\n"); return 1; }
    std::printf("00a5bba0_TEST PASS\n");
    return 0;
}