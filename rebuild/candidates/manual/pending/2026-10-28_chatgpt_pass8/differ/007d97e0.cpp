// std::iter_swap(CUnprovenSwapRecord* a,CUnprovenSwapRecord* b) -- exchange the observed complete16-byte values.
// Leaf: ??$iter_swap@PAVCUnprovenSwapRecord_007d97e0@@PAV1@@std@@YIXPAVCUnprovenSwapRecord_007d97e0@@0@Z
// Identity: descriptive original function/type role remains unproven; exact contract is16-byte bitwise exchange with ECX/EDX receivers. Actual standard STLport iter_swap->__iter_swap wrapper from documented GOTCHAS1378/1409, genuine value_type tag/placement and forceinline inner body; no invented nontrivial copy constructor or padding to force spills. Record words now portable32-bit unsignedint; original record field composition/name/class key not recovered, explicit address-specific CUnproven projection.
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CUnprovenSwapRecord_007d97e0{public:unsigned int UnprovenWords[4];};typedef char ExactObservedRecordSize[sizeof(CUnprovenSwapRecord_007d97e0)==16?1:-1];
namespace std{template<class I>struct iterator_traits;template<class T>struct iterator_traits<T*>{typedef T value_type;};template<class I1,class I2,class T>__forceinline void __fastcall __iter_swap(I1 a,I2 b,T*){T temporary=*a;*a=*b;*b=temporary;}template<class I1,class I2>void __fastcall iter_swap(I1 a,I2 b){__iter_swap(a,b,(typename iterator_traits<I1>::value_type*)0);}}
template void __fastcall std::iter_swap<CUnprovenSwapRecord_007d97e0*,CUnprovenSwapRecord_007d97e0*>(CUnprovenSwapRecord_007d97e0*,CUnprovenSwapRecord_007d97e0*);
