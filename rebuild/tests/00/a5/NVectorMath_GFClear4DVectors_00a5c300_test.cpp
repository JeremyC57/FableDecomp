#include <cstdio>
// behaviour test of NVectorMath::GFClear4DVectors (0x00A5C300): fill count aligned 4D vectors with one value (SSE loop or the scalar fill)
#ifdef _MSC_VER
#define ALIGN16 __declspec(align(16))
#else
#define ALIGN16 __attribute__((aligned(16)))
#endif
class C4DVector {
public:
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(const C4DVector& other) : X(other.X), Y(other.Y), Z(other.Z), W(other.W) { }
};
bool g_SSEAvailable = false;
long g_scalarCalls = 0;
void GFClear4DVectorsScalar(C4DVector* dst, unsigned long count, const C4DVector* value)
{
    ++g_scalarCalls;
    for (unsigned long i = 0; i < count; ++i) dst[i] = *value;
}
#ifdef _MSC_VER
#pragma optimize("y", off)
void GFClear4DVectors(C4DVector* dst, unsigned long count, const C4DVector* value)
{
    if (g_SSEAvailable) {
        __asm {
            mov     eax, value
            mov     edx, dst
            movups  xmm0, [eax]
            mov     ecx, count
            test    ecx, ecx
            je      done
        next:
            movaps  [edx], xmm0
            add     edx, 16
            dec     ecx
            jne     next
        done:
        }
    } else {
        C4DVector copy = *value;
        GFClear4DVectorsScalar(dst, count, &copy);
    }
}
#else
void GFClear4DVectors(C4DVector* dst, unsigned long count, const C4DVector* value)
{
    if (g_SSEAvailable) {
        C4DVector v = *value;                       // the SSE path: an unaligned load of the value, aligned stores
        for (unsigned long i = 0; i < count; ++i) dst[i] = v;
    } else {
        C4DVector copy = *value;
        GFClear4DVectorsScalar(dst, count, &copy);
    }
}
#endif
static bool Check(const C4DVector* v, unsigned long n)
{
    for (unsigned long i = 0; i < n; ++i)
        if (v[i].X != 1.5f || v[i].Y != -2.0f || v[i].Z != 3.0f || v[i].W != 0.25f) return false;
    return true;
}
int main()
{
    ALIGN16 C4DVector dst[6];
    unsigned char raw[20];
    C4DVector* value = (C4DVector*)(raw + 2);           // deliberately unaligned source
    value->X = 1.5f; value->Y = -2.0f; value->Z = 3.0f; value->W = 0.25f;
    for (int i = 0; i < 6; ++i) { dst[i].X = dst[i].Y = dst[i].Z = dst[i].W = 9.0f; }
    g_SSEAvailable = true;
    GFClear4DVectors(dst, 5, value);
    bool ok = Check(dst, 5) && dst[5].X == 9.0f && g_scalarCalls == 0;
    GFClear4DVectors(dst + 5, 0, value);
    ok = ok && dst[5].X == 9.0f;
    for (int i = 0; i < 6; ++i) { dst[i].X = dst[i].Y = dst[i].Z = dst[i].W = 9.0f; }
    g_SSEAvailable = false;
    GFClear4DVectors(dst, 4, value);
    ok = ok && Check(dst, 4) && dst[4].X == 9.0f && g_scalarCalls == 1;
    std::printf(ok ? "00a5c300_TEST PASS\n" : "00a5c300_TEST FAIL\n");
    return ok ? 0 : 1;
}