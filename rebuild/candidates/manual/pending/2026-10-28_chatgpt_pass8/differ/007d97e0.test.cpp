#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CUnprovenSwapRecord_007d97e0{public:unsigned int UnprovenWords[4];};typedef char ExactObservedRecordSize[sizeof(CUnprovenSwapRecord_007d97e0)==16?1:-1];
namespace std{template<class I>struct iterator_traits;template<class T>struct iterator_traits<T*>{typedef T value_type;};template<class I1,class I2,class T>__forceinline void __fastcall __iter_swap(I1 a,I2 b,T*){T temporary=*a;*a=*b;*b=temporary;}template<class I1,class I2>void __fastcall iter_swap(I1 a,I2 b){__iter_swap(a,b,(typename iterator_traits<I1>::value_type*)0);}}
template void __fastcall std::iter_swap<CUnprovenSwapRecord_007d97e0*,CUnprovenSwapRecord_007d97e0*>(CUnprovenSwapRecord_007d97e0*,CUnprovenSwapRecord_007d97e0*);
int main(){typedef CUnprovenSwapRecord_007d97e0 T;T a,b;for(int i=0;i<4;++i){a.UnprovenWords[i]=0x10203040u+i;b.UnprovenWords[i]=0x8090a0b0u+i;}T first=a,second=b;std::iter_swap(&a,&b);bool ok=!memcmp(&a,&second,sizeof(T))&&!memcmp(&b,&first,sizeof(T));std::iter_swap(&a,&a);ok=ok&&!memcmp(&a,&second,sizeof(T));puts(ok?"007d97e0_TEST PASS":"007d97e0_TEST FAIL");return ok?0:1;}
