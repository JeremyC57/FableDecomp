#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class CUnprovenSwapRecord_007d9a00 { public: unsigned long UnprovenWords[4]; };
namespace std { template<class I1,class I2> void __fastcall iter_swap(I1 a,I2 b) { CUnprovenSwapRecord_007d9a00 temporary=*a; *a=*b; *b=temporary; } }
template void __fastcall std::iter_swap<CUnprovenSwapRecord_007d9a00*,CUnprovenSwapRecord_007d9a00*>(CUnprovenSwapRecord_007d9a00*,CUnprovenSwapRecord_007d9a00*);
int main() { CUnprovenSwapRecord_007d9a00 a,b; for(int i=0;i<4;++i) { a.UnprovenWords[i]=i*7+1; b.UnprovenWords[i]=i*9+3; } std::iter_swap(&a,&b); for(int i=0;i<4;++i) if(a.UnprovenWords[i]!=unsigned(i*9+3) || b.UnprovenWords[i]!=unsigned(i*7+1)) goto fail; std::iter_swap(&a,&a); for(int i=0;i<4;++i) if(a.UnprovenWords[i]!=unsigned(i*9+3)) goto fail; puts("007d9a00_TEST PASS"); return 0; fail: puts("007d9a00_TEST FAIL"); return 1; }
