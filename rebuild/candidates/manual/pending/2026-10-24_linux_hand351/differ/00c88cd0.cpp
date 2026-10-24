// _global::DSP_AddVector_00c88cd0(float* dst, const float* src, unsigned long count) -- SSE DSP kernel: dst[i] += src[i] over `count` floats, four at a time (aligned float4 blocks)
// Leaf: DSP_AddVector_00c88cd0
// Grade: DIFFER(49v62) -- retail keeps the block count in esi (`mov esi,eax`, so `sub esp,0Ch ; push esi` in the aligned frame), loads dst/src before the count test, and copies dst per iteration (`mov eax,ecx`) to store through after the early `add ecx,10h`; this source gets jbe + separate pointer steps but no counter register or copy. Tried: __m128* + _mm_add_ps (57 bytes, merged src-dst induction `[edx+eax]`), _mm_load_ps/_mm_store_ps + `i < blocks` (49), inlined CVector4SSE::operator+= (57), the same with /G7 (49).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C88CD0, length 62 bytes (+ int3 padding), `ret 0Ch` (__stdcall).
// Callees: none. Referenced only from the SSE DSP function table @ 0x012BF1E0.. (slot 0x012BF208, between the catalog's
// DSP_ScaleFloatVector @ 0x00C88A50 and DSP_AddScaledVectorWithModulation @ 0x00C88D80 / DSP_AddScaledVector @ 0x00C88D10).
// Shape: `blocks = count >> 2; for (i = 0; i < blocks; ++i) dst[0..3] += src[0..3]` over aligned float4 blocks --
// `shr eax,2 ; test ; jbe` (unsigned `0 < blocks`), each block `movaps` src, `movaps` dst, `addps` into the dst register
// and stored back through a copy of the dst pointer (`mov eax,ecx`). The __m128 traffic makes VC7.1 build the 16-byte
// aligned frame (`and esp,-16 ; sub esp,0Ch` + the esi push) although nothing is spilled.
// Label correction: the catalog's `_Copy_impl<std::pair<float,CSubtitledMessage::CCameraTag>*,...>` is propagated; the
// name follows the neighbouring DSP kernels' catalog names (descriptive).
#include <xmmintrin.h>

void __stdcall DSP_AddVector_00c88cd0(float* dst, const float* src, unsigned long count)
{
    unsigned long blocks = count >> 2;
    for (unsigned long i = 0; i < blocks; ++i) {
        __m128 a = _mm_load_ps(src);
        __m128 b = _mm_load_ps(dst);
        _mm_store_ps(dst, _mm_add_ps(b, a));
        dst += 4;
        src += 4;
    }
}
