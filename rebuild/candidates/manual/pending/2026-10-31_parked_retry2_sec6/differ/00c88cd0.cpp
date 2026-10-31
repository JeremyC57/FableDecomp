// _global::DSP_AddVector_00c88cd0(float* dst, const float* src, unsigned long count) -- SSE DSP kernel: dst[i] += src[i] over `count` floats, four at a time (aligned float4 blocks)
// Leaf: DSP_AddVector_00c88cd0
// Grade: DIFFER(62v62) -- the dst copy `float* p = dst;` loaded and stored through after the early pointer steps gives
//   retail's whole loop (`movaps xmm0,[edx] ; mov eax,ecx ; movaps xmm1,[eax] ; add ecx,10h ; add edx,10h ; dec esi ;
//   addps ; movaps [eax],xmm1`) and the length; left: retail computes the block count in eax, loads dst/src before the
//   count test and copies the count into the esi down-counter after `push esi` (`mov esi,eax`); ours computes it
//   straight into esi with `push esi` first. Tried this pass: down-counting `i = blocks; i != 0` and `i != blocks` loops
//   (identical)
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
        float* p = dst;
        __m128 b = _mm_load_ps(p);
        dst += 4;
        src += 4;
        _mm_store_ps(p, _mm_add_ps(b, a));
    }
}
