#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class CUnprovenSwapRecord_00806220 { public: unsigned long UnprovenWords[4]; };
namespace std { template<class I1,class I2> void __fastcall iter_swap(I1 a,I2 b) { CUnprovenSwapRecord_00806220 temporary=*a; *a=*b; *b=temporary; } }
template void __fastcall std::iter_swap<CUnprovenSwapRecord_00806220*,CUnprovenSwapRecord_00806220*>(CUnprovenSwapRecord_00806220*,CUnprovenSwapRecord_00806220*);
int main() { CUnprovenSwapRecord_00806220 a,b; for(int i=0;i<4;++i) { a.UnprovenWords[i]=i*7+1; b.UnprovenWords[i]=i*9+3; } std::iter_swap(&a,&b); for(int i=0;i<4;++i) if(a.UnprovenWords[i]!=unsigned(i*9+3) || b.UnprovenWords[i]!=unsigned(i*7+1)) goto fail; std::iter_swap(&a,&a); for(int i=0;i<4;++i) if(a.UnprovenWords[i]!=unsigned(i*9+3)) goto fail; puts("00806220_TEST PASS"); return 0; fail: puts("00806220_TEST FAIL"); return 1; }
