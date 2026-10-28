#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
class CBankFile { public: class CRuntimeData { public: unsigned long DataOffset,DataSize; unsigned char DataType; bool Valid; }; };
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

typedef CBankFile::CRuntimeData Element;
template void std::vector<Element>::_M_insert_overflow(Element*,const Element&,const std::__false_type&,unsigned int,bool);
static bool equal(const Element& a,const Element& b) { return a.DataOffset==b.DataOffset && a.DataSize==b.DataSize && a.DataType==b.DataType && a.Valid==b.Valid; }
static bool runCase(unsigned n,bool atEnd) { std::vector<Element> v; Element x; memset(&x,0,sizeof(x)); x.DataOffset=7; x.DataSize=7; x.DataType=7; x.Valid=true; v._M_start=(Element*)malloc(2*sizeof(Element)); v._M_finish=v._M_start+2; v._M_end_of_storage._M_data=v._M_finish; memset(v._M_start,0,2*sizeof(Element)); Element zero=v._M_start[0]; v.InvokeGrow(v._M_start+(atEnd?2:1),x,n,atEnd); bool ok=v._M_finish-v._M_start==int(2+n) && v._M_end_of_storage._M_data-v._M_start==int(2+(n>2?n:2)) && equal(v._M_start[0],zero) && equal(v._M_start[atEnd?1:n+1],zero); for(unsigned i=0;i<n;++i) if(!equal(v._M_start[(atEnd?2:1)+i],x)) ok=false; free(v._M_start); return ok; }
int main() { if(!runCase(1,false) || !runCase(3,false) || !runCase(2,true)) { puts("009d3580_TEST FAIL"); return 1; } puts("009d3580_TEST PASS"); return 0; }