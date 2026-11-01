// 00c6b260 salFrameFloatToInt
// Leaf: _salFrameFloatToInt
// Flags: /Oy-
#include <emmintrin.h>
// Original SDK COFF names and CodeView class access; no instance fields.
class CSalCpuInfo {
    static bool m_bInitialized,m_bHasSSE,m_bHasSSE2;
    static void __cdecl Initialize();
public:
    __forceinline CSalCpuInfo() { if(!m_bInitialized) Initialize(); }
    __forceinline static bool __cdecl HasSSE() { return m_bHasSSE; }
    __forceinline static bool __cdecl HasSSE2() { return m_bHasSSE2; }
};
#pragma auto_inline(off)
int __cdecl sal_round(float value);
#pragma auto_inline(on)
extern "C" void __cdecl salFrameFloatToInt(int* pDest,const float* pfSource) {
    CSalCpuInfo cpu;
    if(cpu.HasSSE2()) {
        unsigned int rounding=_MM_GET_ROUNDING_MODE();
        _MM_SET_ROUNDING_MODE(_MM_ROUND_NEAREST);
        unsigned int i=8;
        do {
            __m128i a=_mm_cvtps_epi32(_mm_load_ps(pfSource));
            __m128i b=_mm_cvtps_epi32(_mm_load_ps(pfSource+4));
            __m128i c=_mm_cvtps_epi32(_mm_load_ps(pfSource+8));
            __m128i d=_mm_cvtps_epi32(_mm_load_ps(pfSource+12));
            _mm_store_si128((__m128i*)pDest,a);
            _mm_store_si128((__m128i*)(pDest+4),b);
            _mm_store_si128((__m128i*)(pDest+8),c);
            _mm_store_si128((__m128i*)(pDest+12),d);
            pfSource+=16;pDest+=16;
        } while(--i);
        _MM_SET_ROUNDING_MODE(rounding);
    } else if(cpu.HasSSE()) {
        unsigned int rounding=_MM_GET_ROUNDING_MODE();
        _MM_SET_ROUNDING_MODE(_MM_ROUND_NEAREST);
        unsigned int i=16;
        do {
            __m64 a=_mm_cvtps_pi32(_mm_loadl_pi(_mm_setzero_ps(),(__m64 const*)pfSource));
            __m64 b=_mm_cvtps_pi32(_mm_loadl_pi(_mm_setzero_ps(),(__m64 const*)(pfSource+2)));
            __m64 c=_mm_cvtps_pi32(_mm_loadl_pi(_mm_setzero_ps(),(__m64 const*)(pfSource+4)));
            __m64 d=_mm_cvtps_pi32(_mm_loadl_pi(_mm_setzero_ps(),(__m64 const*)(pfSource+6)));
            *(__m64*)pDest=a;
            *(__m64*)(pDest+2)=b;
            *(__m64*)(pDest+4)=c;
            *(__m64*)(pDest+6)=d;
            pfSource+=8;pDest+=8;
        } while(--i);
        _mm_empty();
        _MM_SET_ROUNDING_MODE(rounding);
    } else {
        for(unsigned int i=0;i<128;++i) pDest[i]=sal_round(pfSource[i]);
    }
}
