// mul_high(const Is16vec4& a, const Is16vec4& b) -- ivec.h: high 16 bits of the four signed 16x16 products (pmulhw), result by value
// Leaf: ?mul_high@@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C78810, length 52 bytes (+ int3 padding), __cdecl `ret`. No callers.
// Shape: `return _m_pmulhw(a, b);` -- hidden result at [ebp+8], a in mm1, b in mm0.
// Built under /GS: the __m64 union-of-arrays makes VC7.1 give the function a cookie in an 8-byte aligned frame
// (`and esp,-8 ; sub esp,8`, cookie 0x0139C8A8 at +4, checked by __security_check_cookie 0x00BFE9F9 before the return).
// Library: libpk software mixer (no PDB/RTTI); the class and function names are the Intel ivec.h spellings.
#include <mmintrin.h>

// Intel C++ class library (ivec.h) model: VC7.1's RTM include directory has no ivec.h, so the classes the
// libpk mixer used are restated here with the header's own member spellings.
class M64 {
protected:
    __m64 vec;                                         // +0x00
public:
    M64() {}
    M64(__m64 mm) { vec = mm; }
    operator __m64() const { return vec; }
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
};

Is16vec4 mul_high(const Is16vec4& a, const Is16vec4& b)
{
    return _m_pmulhw(a, b);
}