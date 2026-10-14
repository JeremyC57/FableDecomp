// CVectorSSE_00c7b840::CVectorSSE_00c7b840(__m128i value) -- store the SSE2 integer vector argument into the member
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C7B840, length 48 bytes. Built under /GS.
// Callees: __security_check_cookie @ 0x00BFE9F9 (cookie @ 0x0139C8A8).
// Shape: VC7.1 passes a by-value `__m128i` in xmm0 (the callers at 0x00C87A12.. compute `pmaddwd xmm0,xmm1`
// and call with ecx = the object), so the body is a single `movdqa [this],xmm0`; the union-of-arrays
// parameter makes /GS give the function a cookie in a 16-byte aligned frame (`and esp,-16 ; sub esp,10h`,
// cookie at +0Ch) checked before the return of `this` in eax. The manifest label "CDateAndTime::CDateAndTime"
// is propagated (CDateAndTime has no SSE members); the class is named by address and shape. Siblings:
// 0x00C7B7D0 and 0x00C7B810 are the same body for two more classes.
#include <emmintrin.h>

struct CVectorSSE_00c7b840 {
    __m128i m_v;                       // +0x00
    CVectorSSE_00c7b840(__m128i value);
};

CVectorSSE_00c7b840::CVectorSSE_00c7b840(__m128i value)
{
    m_v = value;
}