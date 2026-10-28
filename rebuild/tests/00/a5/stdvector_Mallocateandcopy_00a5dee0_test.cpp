#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
enum EVertexStreamDataType {};
class CVertexStreamFormat { public: class CDataElement { public: unsigned long Register; EVertexStreamDataType DataType; }; };
#ifdef _MSC_VER
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
#else
#include <stdlib.h>
#endif
namespace std {
struct __false_type { __false_type() {} };
template<class T> class allocator { public: T* allocate(unsigned int n) { return n?(T*)malloc(n*sizeof(T)):0; } };
template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; };
template<class T,class V> __forceinline void _Construct(T* p,const V& value) { new(p) T(value); }
template<class Input,class Output> __forceinline Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&) { Output current=result; for(;first!=last;++first,++current) _Construct(&*current,*first); return current; }
template<class T,class Alloc=allocator<T> > class vector { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; template<class Input> T* _M_allocate_and_copy(unsigned int n,Input first,Input last) { T* result=_M_end_of_storage.allocate(n); __uninitialized_copy(first,last,result,__false_type()); return result; } };
}

typedef CVertexStreamFormat::CDataElement Element;
template Element* std::vector<Element>::_M_allocate_and_copy<const Element*>(unsigned int,const Element*,const Element*);
int main() { Element src[3]; int i; for(i=0;i<3;++i) { src[i].Register=i*7+2; src[i].DataType=(EVertexStreamDataType)0; } std::vector<Element> v;
 Element* result=v._M_allocate_and_copy(5,(const Element*)src,(const Element*)src+3); if(!result || result==src) goto fail;
 for(i=0;i<3;++i) if(result[i].Register!=src[i].Register || result[i].DataType!=src[i].DataType) goto fail; free(result);
 result=v._M_allocate_and_copy(0,(const Element*)src,(const Element*)src); if(result) { free(result); goto fail; }
 result=v._M_allocate_and_copy(2,(const Element*)src,(const Element*)src); if(!result) goto fail; free(result);
 puts("00a5dee0_TEST PASS"); return 0; fail: puts("00a5dee0_TEST FAIL"); return 1;
}