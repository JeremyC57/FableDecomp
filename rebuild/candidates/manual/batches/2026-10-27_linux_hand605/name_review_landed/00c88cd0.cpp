// CMixKernelsSSE::Add(float* dst, const float* src, unsigned long count) -- accumulate one 16-byte aligned float block into another (dst += src), four samples per SSE step
// Leaf: ?Add@CMixKernelsSSE@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C88CD0, length 62 bytes (+ int3 padding), thiscall `ret 0Ch`; a virtual member
//   that never reads `this` (ecx is reused for dst at once). No callees.
// Identity: slot 5 of the libpk SSE mixer kernel table, vtable 0x012BF1F4 (slots 0x00C88A40, 0x00C88BF0, 0x00C88B20, 0x00C88AB0,
//   0x00C88A50, 0x00C88CD0, 0x00C88D80, 0x00C88D10, 0x00C76880), the class the six landed siblings model (CMixKernelsSSE::Scale
//   0x00C88A50, ScaleCopy 0x00C88AB0, ScaleRamped 0x00C88B20, ScaleCopyRamped 0x00C88BF0, Mix 0x00C88D10, MixRamped 0x00C88D80),
//   all of which already declare this address as `virtual void Add(float* dst, const float* src, unsigned long count)`.
//   libpk has no PDB/RTTI: the class/method names are the landed DESCRIPTIVE names (name_review). The catalog label
//   `_Copy_impl<std::pair<float,CSubtitledMessage::CCameraTag>*,...>` is propagated and wrong.
// Shape: the Intel fvec.h F32vec4 model of the siblings and `*d++ += *s++;` -- the member operator+= on the post-incremented dst
//   gives retail's `mov eax,ecx` block-pointer copy (load + store through eax, ecx/edx stepped before the addps), the count/4
//   computed in eax before `push esi` and copied into the esi down-counter; the aligned __m128 traffic builds the
//   `and esp,-16 ; sub esp,0Ch` frame. (The earlier raw `_mm_load_ps` model with a manual `float* p = dst` copy got the loop
//   but put the count straight into esi.)
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

void CMixKernelsSSE::Add(float* dst, const float* src, unsigned long count)
{
    F32vec4* d = (F32vec4*)dst;
    const F32vec4* s = (const F32vec4*)src;
    for (unsigned long i = 0; i < count / 4; i++)
        *d++ += *s++;
}
