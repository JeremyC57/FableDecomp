#ifndef _MSC_VER
#define __cdecl
#define __forceinline inline
#define TEST_ALIGN __attribute__((aligned(16)))
#else
#define TEST_ALIGN __declspec(align(16))
#endif
// 00c6b260 salFrameFloatToInt
// Leaf: _salFrameFloatToInt
// Flags: /Oy-
#include <emmintrin.h>
// Original SDK COFF names and CodeView class access; no instance fields.
class CSalCpuInfo {
public: // Fixture exposes original static state; target body unchanged.
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

#include <stdio.h>
#include <math.h>
#include <string.h>
bool CSalCpuInfo::m_bInitialized=false;
bool CSalCpuInfo::m_bHasSSE=false;
bool CSalCpuInfo::m_bHasSSE2=false;
static unsigned int initCalls,roundCalls,scalarMode;
static float seen[128];
void CSalCpuInfo::Initialize(){++initCalls;m_bInitialized=true;}
static int expectedRound(float value,unsigned int mode){
 double x=value;
 if(x!=x||x>=2147483648.0||x< -2147483648.0)return (-2147483647-1);
 double n;
 if(mode==1)n=floor(x);
 else if(mode==2)n=ceil(x);
 else if(mode==3)n=x<0?ceil(x):floor(x);
 else {
  double base=floor(x),fraction=x-base;
  n=base;
  if(fraction>0.5||(fraction==0.5&&fmod(base,2.0)!=0.0))n+=1;
 }
 if(n>=2147483648.0||n< -2147483648.0)return (-2147483647-1);
 return (int)n;
}
int __cdecl sal_round(float value){
 if(roundCalls<128)seen[roundCalls]=value;
 ++roundCalls;return expectedRound(value,scalarMode);
}
#define CHECK(c) do{if(!(c)){fprintf(stderr,"failure line %d, cases %u\n",__LINE__,cases);_mm_setcsr(saved);return 1;}}while(0)
int main(){
 unsigned int saved=_mm_getcsr(),cases=0;
 TEST_ALIGN float source[132];TEST_ALIGN int dest[132];
 const unsigned int patterns[]={0,0x80000000U,0x3f000000U,0xbf000000U,0x3fc00000U,0xbfc00000U,0x40200000U,0xc0200000U,0x4effffffU,0x4f000000U,0xcf000000U,0xcf000001U,0x7f800000U,0xff800000U,0x7fc00001U,0x3e800000U};
 for(unsigned int path=0;path<3;++path)for(unsigned int csrMode=0;csrMode<4;++csrMode)for(scalarMode=0;scalarMode<4;++scalarMode)for(unsigned int initialized=0;initialized<2;++initialized)for(unsigned int pattern=0;pattern<8;++pattern){
  for(unsigned int j=0;j<132;++j){
   unsigned int bits=patterns[(j+pattern*3)%16];memcpy(&source[j],&bits,4);dest[j]=0x12345678;
  }
  CSalCpuInfo::m_bInitialized=initialized!=0;CSalCpuInfo::m_bHasSSE=path>=1;CSalCpuInfo::m_bHasSSE2=path==2;
  initCalls=roundCalls=0;memset(seen,0,sizeof(seen));
  unsigned int initial=0x1f80|(csrMode<<13);_mm_setcsr(initial);
  salFrameFloatToInt(dest,source);
  unsigned int after=_mm_getcsr();
  CHECK((after&~0x3fU)==initial);
  CHECK(initCalls==(initialized?0U:1U));CHECK(CSalCpuInfo::m_bInitialized);
  CHECK(roundCalls==(path==0?128U:0U));
  for(unsigned int j=0;j<128;++j){
   CHECK(dest[j]==expectedRound(source[j],path?0:scalarMode));
   if(path==0)CHECK(memcmp(&source[j],&seen[j],4)==0);
  }
  for(unsigned int j=128;j<132;++j)CHECK(dest[j]==0x12345678);
  for(unsigned int j=0;j<132;++j){unsigned int bits=patterns[(j+pattern*3)%16];CHECK(memcmp(&source[j],&bits,4)==0);}
  ++cases;
 }
 CHECK(cases==768);_mm_setcsr(saved);puts("00c6b260_TEST PASS");return 0;
}
