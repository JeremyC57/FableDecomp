// Behaviour test for 00c78670: ivec.h MMX class-library routine of the libpk software mixer
#include <cstdio>
#include <cstring>
#include <mmintrin.h>

// copy of the ivec.h model used by the source, plus test-only lane setters/getters
class M64 {
protected:
    __m64 vec;
public:
    M64() {}
    M64(__m64 mm) { vec = mm; }
    operator __m64() const { return vec; }
    void Get(void* out) const { std::memcpy(out, &vec, 8); }
};
class I32vec2 : public M64 {
public:
    I32vec2() {}
    I32vec2(__m64 mm) : M64(mm) {}
};
class Is32vec2 : public I32vec2 {
public:
    Is32vec2() {}
    Is32vec2(__m64 mm) : I32vec2(mm) {}
    Is32vec2(int hi, int lo) { vec = _mm_set_pi32(hi, lo); }
    Is32vec2& operator+=(const I32vec2& a) { return *this = (Is32vec2)_m_paddd(vec, a); }
    Is32vec2 operator>>(int count) { return _m_psradi(vec, count); }
    Is32vec2& operator>>=(int count) { return *this = (Is32vec2)_m_psradi(vec, count); }
};
class I16vec4 : public M64 {
public:
    I16vec4() {}
    I16vec4(__m64 mm) : M64(mm) {}
};
class Is16vec4 : public I16vec4 {
public:
    Is16vec4() {}
    Is16vec4(__m64 mm) : I16vec4(mm) {}
    Is16vec4(short s3, short s2, short s1, short s0) { vec = _mm_set_pi16(s3, s2, s1, s0); }
};
inline Is32vec2 unpack_low(const Is32vec2& a, const Is32vec2& b) { return _m_punpckldq(a, b); }
inline Is32vec2 unpack_high(const Is32vec2& a, const Is32vec2& b) { return _m_punpckhdq(a, b); }
inline Is16vec4 sat_add(const Is16vec4& a, const Is16vec4& b) { return _m_paddsw(a, b); }
inline Is16vec4 mul_high(const Is16vec4& a, const Is16vec4& b) { return _m_pmulhw(a, b); }
inline Is32vec2 mul_add(const Is16vec4& a, const Is16vec4& b) { return _m_pmaddwd(a, b); }
inline Is16vec4 pack_sat(const Is32vec2& a, const Is32vec2& b) { return _m_packssdw(a, b); }
inline Is32vec2 operator+(const Is32vec2& a, const Is32vec2& b) { return _m_paddd(a, b); }

static int Check32(const M64& v, int lo, int hi) { int r[2]; v.Get(r); _mm_empty(); return r[0] == lo && r[1] == hi; }
static int Check16(const M64& v, short s0, short s1, short s2, short s3) { short r[4]; v.Get(r); _mm_empty(); return r[0] == s0 && r[1] == s1 && r[2] == s2 && r[3] == s3; }

int main()
{
    int ok = 0;
    Is32vec2 a(-64, 1024);
    Is32vec2 r = a >> 3;
    ok = Check32(r, 128, -8) && Check32(a, 1024, -64);
    _mm_empty();
    if (!ok) { std::printf("00c78670_TEST FAIL\n"); return 1; }
    std::printf("00c78670_TEST PASS\n");
    return 0;
}