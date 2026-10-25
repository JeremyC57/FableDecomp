// Behaviour test for 00c88ab0: libpk SSE mixer kernel CMixKernelsSSE::ScaleCopy (aligned four-sample blocks)
#include <cstdio>
#include <xmmintrin.h>

class F32vec4 {
protected:
    __m128 vec;
public:
    F32vec4() {}
    F32vec4(__m128 m) { vec = m; }
    F32vec4(float f3, float f2, float f1, float f0) { vec = _mm_set_ps(f3, f2, f1, f0); }
    operator __m128() const { return vec; }
    F32vec4& operator*=(const F32vec4& a) { return *this = _mm_mul_ps(vec, a); }
    F32vec4& operator+=(const F32vec4& a) { return *this = _mm_add_ps(vec, a); }
    friend F32vec4 operator*(const F32vec4& a, const F32vec4& b) { return _mm_mul_ps(a, b); }
    friend F32vec4 operator+(const F32vec4& a, const F32vec4& b) { return _mm_add_ps(a, b); }
};

static __m128 g_dstStore[3];
static __m128 g_srcStore[3];
static int Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

// copy of the kernel body (free function: the member never reads `this`)
void ScaleCopy(float* dst, const float* src, float gain, unsigned long count)
{
    F32vec4 g(gain, gain, gain, gain);
    F32vec4* d = (F32vec4*)dst;
    const F32vec4* s = (const F32vec4*)src;
    for (unsigned long i = 0; i < count / 4; i++)
        _mm_store_ps((float*)d++, *s++ * g);
}

static int Run()
{
    float* dst = (float*)g_dstStore;
    float* src = (float*)g_srcStore;
    (void)src;
    for (int i = 0; i < 12; i++) { src[i] = (float)(i + 1); dst[i] = -1.0f; }
    ScaleCopy(dst, src, 3.0f, 8);
    for (int i = 0; i < 12; i++) if (!Near(dst[i], i < 8 ? (i + 1) * 3.0f : -1.0f)) return 0;
    return Near(src[5], 6.0f);
}

int main()
{
    if (!Run()) { std::printf("00c88ab0_TEST FAIL\n"); return 1; }
    std::printf("00c88ab0_TEST PASS\n");
    return 0;
}