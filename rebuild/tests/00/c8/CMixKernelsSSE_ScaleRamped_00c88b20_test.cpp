// Behaviour test for 00c88b20: libpk SSE mixer kernel CMixKernelsSSE::ScaleRamped (aligned four-sample blocks)
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
void ScaleRamped(float* buffer, float start, float step, unsigned long count)
{
    F32vec4 g(start + step * 3.0f, start + step * 2.0f, start + step, start);
    float step4 = step * 4.0f;
    F32vec4 inc(step4, step4, step4, step4);
    F32vec4* p = (F32vec4*)buffer;
    for (unsigned long i = 0; i < count / 4; i++) {
        *p++ *= g;
        g += inc;
    }
}

static int Run()
{
    float* dst = (float*)g_dstStore;
    float* src = (float*)g_srcStore;
    (void)src;
    for (int i = 0; i < 12; i++) dst[i] = 2.0f;
    ScaleRamped(dst, 1.0f, 0.25f, 8);
    for (int i = 0; i < 12; i++) if (!Near(dst[i], i < 8 ? 2.0f * (1.0f + 0.25f * i) : 2.0f)) return 0;
    return 1;
}

int main()
{
    if (!Run()) { std::printf("00c88b20_TEST FAIL\n"); return 1; }
    std::printf("00c88b20_TEST PASS\n");
    return 0;
}