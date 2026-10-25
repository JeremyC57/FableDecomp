// Behaviour test for 00c7b870: dvec.h Is32vec4::operator+= of the libpk software mixer (lane-wise 32-bit add in place)
#include <cstdio>
#include <cstring>
#include <emmintrin.h>

class M128 {
protected:
    __m128i vec;
public:
    M128() {}
    M128(__m128i mm) { vec = mm; }
    operator __m128i() const { return vec; }
    void Get(int* out) const { std::memcpy(out, &vec, 16); }
};
class I32vec4 : public M128 {
public:
    I32vec4() {}
    I32vec4(__m128i mm) : M128(mm) {}
};
class Is32vec4 : public I32vec4 {
public:
    Is32vec4() {}
    Is32vec4(__m128i mm) : I32vec4(mm) {}
    Is32vec4(int i3, int i2, int i1, int i0) { vec = _mm_set_epi32(i3, i2, i1, i0); }
    Is32vec4& operator+=(const I32vec4& a) { return *this = (Is32vec4)_mm_add_epi32(vec, a); }
};

int main()
{
    Is32vec4 a(1, -2, 0x7fffffff, 100);
    Is32vec4 b(10, 20, 1, -300);
    Is32vec4& r = (a += b);
    int x[4], y[4];
    a.Get(x); b.Get(y);
    if (&r != &a || x[0] != -200 || x[1] != (int)0x80000000 || x[2] != 18 || x[3] != 11
        || y[0] != -300 || y[3] != 10) { std::printf("00c7b870_TEST FAIL\n"); return 1; }
    std::printf("00c7b870_TEST PASS\n");
    return 0;
}