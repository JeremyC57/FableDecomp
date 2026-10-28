// std::vector::_M_insert_overflow(pointer position, const value_type& value, const __false_type& tag, unsigned int count, bool atEnd) -- grow and insert trivial records.
// Leaf: ?_M_insert_overflow@?$vector@VCBlockInfo@CVertexGroupBlockOptimiser@@V?$allocator@VCBlockInfo@CVertexGroupBlockOptimiser@@@std@@@std@@IAEXPAVCBlockInfo@CVertexGroupBlockOptimiser@@ABV34@ABU__false_type@2@I_N@Z
// Identity: proven; own push0xADBFB0 called by CVertexGroupBlockOptimiser::AddBlocks0xADA0E9 on local vector; input BlockIndex,Cost,Benefit stack12-byte record agrees with PDB CBlockInfo; copy0xADA7E0/fill0xADA820.
// Layout: generated outer headers have no nested record declarations; outer declarations provide naming scope only and no outer object is accessed. Retail vector has3pointers.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
class CVertexGroupBlockOptimiser { public: class CBlockInfo { public: unsigned long BlockIndex; long Cost,Benefit; }; };
#ifdef _MSC_VER
#define NOINLINE __declspec(noinline)
extern "C" void* __cdecl malloc(unsigned int); extern "C" void __cdecl free(void*);
#else
#define NOINLINE __attribute__((noinline))
#include <stdlib.h>
#endif
namespace std {
struct __false_type { __false_type() {} };
template<class T> class allocator { public: T* allocate(unsigned int n) { return n?(T*)malloc(n*sizeof(T)):0; } void deallocate(T* p,unsigned int) { if(p) free(p); } };
template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; };
template<class T> inline const T& max(const T& a,const T& b) { return a<b?b:a; }
template<class T,class V> __forceinline void _Construct(T* p,const V& value) { new(p) T(value); }
template<class Input,class Output> NOINLINE Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&) { Output current=result; for(;first!=last;++first,++current) _Construct(&*current,*first); return current; }
template<class Iter,class Size,class T> NOINLINE Iter __fastcall __uninitialized_fill_n(Iter first,Size count,const T& value,const __false_type&) { for(;count>0;--count,++first) _Construct(&*first,value); return first; }
template<class T,class Alloc=allocator<T> > class vector { public: typedef unsigned int size_type; typedef T* pointer; T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage;
protected: void _M_insert_overflow(pointer position,const T& value,const __false_type&,size_type fillLength,bool atEnd) {
 const size_type oldSize=size_type(_M_finish-_M_start); const size_type length=oldSize+max(oldSize,fillLength); pointer start=_M_end_of_storage.allocate(length);
 pointer finish=__uninitialized_copy(_M_start,position,start,__false_type());
 if(fillLength==1) { _Construct(finish,value); ++finish; } else finish=__uninitialized_fill_n(finish,fillLength,value,__false_type());
 if(!atEnd) finish=__uninitialized_copy(position,_M_finish,finish,__false_type());
 _M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start); _M_start=start; _M_finish=finish; _M_end_of_storage._M_data=start+length;
 }
public: void InvokeGrow(pointer p,const T& v,size_type n,bool atEnd) { _M_insert_overflow(p,v,__false_type(),n,atEnd); }
}; }

typedef CVertexGroupBlockOptimiser::CBlockInfo Element;
template void std::vector<Element>::_M_insert_overflow(Element*,const Element&,const std::__false_type&,unsigned int,bool);