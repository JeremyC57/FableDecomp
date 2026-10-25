// CMixKernelsSSE::ScaleCopy(float* dst, const float* src, float gain, unsigned long count) -- write a gain-scaled copy of an aligned float block, four samples per SSE step
// Leaf: ?ScaleCopy@CMixKernelsSSE@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C88AB0, length 105 bytes (+ int3 padding), `ret 10h`; a virtual __fastcall member that never
// reads `this` (ecx is reused at once). No callees. Referenced only from the SSE kernel vtable 0x012BF1F4.
// Library: libpk software mixer (no PDB/RTTI): the class and method names are descriptive; the manifest's
// "DSP_*" labels are descriptive guesses of the same operations. The vector class is the Intel fvec.h F32vec4.
// The aligned __m128 locals give the `push ebp ; mov ebp,esp ; and esp,-16` frame (no /GS cookie: no array).
// Shape: `_mm_store_ps((float*)d++, *s++ * g);` -- the post-incremented store pointer is copied (`mov ecx,eax`)
// before the add, the source is addressed through the dst-relative difference (`[edx+eax]`), and the extra
// register for the counter is esi (pushed after the aligned frame, so the gain sits at +0Ch). `*d++ = *s++ * g`
// (a plain struct store) drops the pointer copy and counts in ecx (probe-verified).
#include <xmmintrin.h>

// Intel C++ class library (fvec.h) model: RTM VC7.1 ships no fvec.h, so the members used here are restated.
class F32vec4 {
protected:
    __m128 vec;                                        // +0x00
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

// The libpk mixer's SSE kernel table (vtable 0x012BF1F4, constructor 0x00C88A30): slot 0 0x00C88A40 reports the
// kernels usable (returns 1), slots 1..7 are the block kernels below (all `this`-free virtual __fastcall members
// over 16-byte aligned float blocks, four samples per iteration), slot 8 0x00C76880 is shared with the x87 base.
class CMixKernelsSSE {
public:
    virtual long IsAvailable();                                                                         // 0x00C88A40
    virtual void ScaleCopyRamped(float* dst, const float* src, float start, float step, unsigned long count); // 0x00C88BF0
    virtual void ScaleRamped(float* buffer, float start, float step, unsigned long count);              // 0x00C88B20
    virtual void ScaleCopy(float* dst, const float* src, float gain, unsigned long count);              // 0x00C88AB0
    virtual void Scale(float* buffer, float gain, unsigned long count);                                 // 0x00C88A50
    virtual void Add(float* dst, const float* src, unsigned long count);                                // 0x00C88CD0
    virtual void MixRamped(float* dst, const float* src, float start, float step, unsigned long count); // 0x00C88D80
    virtual void Mix(float* dst, const float* src, float gain, unsigned long count);                    // 0x00C88D10
};

void CMixKernelsSSE::ScaleCopy(float* dst, const float* src, float gain, unsigned long count)
{
    F32vec4 g(gain, gain, gain, gain);
    F32vec4* d = (F32vec4*)dst;
    const F32vec4* s = (const F32vec4*)src;
    for (unsigned long i = 0; i < count / 4; i++)
        _mm_store_ps((float*)d++, *s++ * g);
}