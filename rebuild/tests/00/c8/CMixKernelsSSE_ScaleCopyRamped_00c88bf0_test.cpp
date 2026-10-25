// Behaviour test for 00c88bf0: libpk SSE mixer kernel CMixKernelsSSE::ScaleCopyRamped (aligned four-sample blocks)
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
void ScaleCopyRamped(float* dst, const float* src, float start, float step, unsigned long count)
{
    F32vec4 g(start + step * 3.0f, start + step * 2.0f, start + step, start);
    float step4 = step * 4.0f;
    F32vec4 inc(step4, step4, step4, step4);
    F32vec4* d = (F32vec4*)dst;
    const F32vec4* s = (const F32vec4*)src;
    for (unsigned long i = 0; i < count / 4; i++) {
        _mm_store_ps((float*)d++, *s++ * g);
        g += inc;
    }
}

static int Run()
{
    float* dst = (float*)g_dstStore;
    float* src = (float*)g_srcStore;
    (void)src;
    for (int i = 0; i < 12; i++) { src[i] = (float)(i + 1); dst[i] = 9.0f; }
    ScaleCopyRamped(dst, src, 0.5f, -0.125f, 12);
    for (int i = 0; i < 12; i++) if (!Near(dst[i], (i + 1) * (0.5f - 0.125f * i))) return 0;
    return 1;
}

int main()
{
    if (!Run()) { std::printf("00c88bf0_TEST FAIL\n"); return 1; }
    std::printf("00c88bf0_TEST PASS\n");
    return 0;
}