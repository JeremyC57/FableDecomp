// Behaviour test for 00c88a50: libpk SSE mixer kernel CMixKernelsSSE::Scale (aligned four-sample blocks)
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
void Scale(float* buffer, float gain, unsigned long count)
{
    F32vec4 g(gain, gain, gain, gain);
    F32vec4* p = (F32vec4*)buffer;
    for (unsigned long i = 0; i < count / 4; i++)
        *p++ *= g;
}

static int Run()
{
    float* dst = (float*)g_dstStore;
    float* src = (float*)g_srcStore;
    (void)src;
    for (int i = 0; i < 12; i++) dst[i] = (float)(i + 1);
    Scale(dst, 0.5f, 8);
    for (int i = 0; i < 12; i++) if (!Near(dst[i], i < 8 ? (i + 1) * 0.5f : (float)(i + 1))) return 0;
    Scale(dst, 2.0f, 3);   // fewer than four samples: no block
    return Near(dst[0], 0.5f);
}

int main()
{
    if (!Run()) { std::printf("00c88a50_TEST FAIL\n"); return 1; }
    std::printf("00c88a50_TEST PASS\n");
    return 0;
}