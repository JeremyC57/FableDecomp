// Behaviour test for 00c7b8f0: Is32vec4::operator>>=(int) -- arithmetic shift of every lane in place, returns *this
#include <stdio.h>
#include <emmintrin.h>
class M128 { protected: __m128i vec; public: M128() {} M128(__m128i mm) { vec = mm; } operator __m128i() const { return vec; } };
class I32vec4 : public M128 { public: I32vec4() {} I32vec4(__m128i mm) : M128(mm) {} };
class Is32vec4 : public I32vec4 {
public:
    Is32vec4() {} Is32vec4(__m128i mm) : I32vec4(mm) {}
    Is32vec4& operator>>=(int count) { return *this = (Is32vec4)_mm_srai_epi32(vec, count); }
};
int main()
{
    Is32vec4 v(_mm_set_epi32((int)0x80000000, 7, -64, 0x100));
    int count = 3;
    Is32vec4& r = (v >>= count);
    __m128i m = v; long* lanes = (long*)&m;
    bool ok = &r == &v && lanes[0] == 0x20 && lanes[1] == -8 && lanes[2] == 0 && lanes[3] == (long)0xF0000000;
    printf(ok ? "00c7b8f0_TEST PASS\n" : "00c7b8f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
