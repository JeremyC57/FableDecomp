// 00adb4d0 std::__pop_heap_aux
// Leaf: ??$__pop_heap_aux@PAVCBlockInfo@CVertexGroupBlockOptimiser@@V12@U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@std@@@std@@YIXPAVCBlockInfo@CVertexGroupBlockOptimiser@@00U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@0@@Z
// Flags:
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#endif
// Layout: PDB CBlockInfo has BlockIndex+0, Cost+4, Benefit+8; retail moves all 12 bytes and compares Cost.
// No engine header exists for this nested record or its naming-scope outer class.
class CVertexGroupBlockOptimiser { public: class CBlockInfo { public: unsigned long BlockIndex; long Cost,Benefit; bool operator<(const CBlockInfo&other)const{return Cost<other.Cost;} }; };
namespace std {
template<class A,class B,class R>struct binary_function {typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class Iterator,class Distance,class T,class Compare>void __fastcall __adjust_heap(Iterator,Distance,Distance,T,Compare);
template<class Iterator,class T,class Compare,class Distance>inline void __fastcall __pop_heap(Iterator first,Iterator last,Iterator result,T value,Compare comp,Distance*){*result=*first;__adjust_heap(first,Distance(0),Distance(last-first),value,comp);}
template<class Iterator,class T,class Compare>inline void __fastcall __pop_heap_aux(Iterator first,Iterator last,T*,Compare comp){__pop_heap(first,last-1,last-1,T(*(last-1)),comp,(ptrdiff_t*)0);}
template void __fastcall __pop_heap_aux<CVertexGroupBlockOptimiser::CBlockInfo*,CVertexGroupBlockOptimiser::CBlockInfo,less<CVertexGroupBlockOptimiser::CBlockInfo> >(CVertexGroupBlockOptimiser::CBlockInfo*,CVertexGroupBlockOptimiser::CBlockInfo*,CVertexGroupBlockOptimiser::CBlockInfo*,less<CVertexGroupBlockOptimiser::CBlockInfo>);
}