// M64::M64(__m64 mm) -- ivec.h base constructor: store the by-register __m64 into the member (VC7.1 itself emits the closing emms)
// Leaf: ??0M64@@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C78590, length 38 bytes. No callers (an unreferenced COMDAT of the
// libpk software mixer's MMX TU). Byte-identical twins: 0x00C785D0, 0x00C78600, 0x00C78760, 0x00C787A0 (the I32vec2 /
// Is32vec2 / I16vec4 / Is16vec4 forwarding constructors, which inline this body); which twin is which class is not
// recoverable without symbols -- this one sits right before M64::operator __m64 (0x00C785C0), so it is the base.
// Shape: VC7.1 passes a by-value __m64 in mm0, so the body is `mov eax,ecx ; movq [eax],mm0`; for a function that
// receives an __m64 in an MMX register the compiler appends `emms` after the epilogue on its own (no _mm_empty in
// the source -- probe-verified).
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
    M64(__m64 mm) ;
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

M64::M64(__m64 mm)
{
    vec = mm;
}