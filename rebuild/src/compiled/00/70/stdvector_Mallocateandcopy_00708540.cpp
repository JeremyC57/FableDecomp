// std::vector::_M_allocate_and_copy(unsigned int n, Input first, Input last) -- allocate space and copy-construct the record range.
// Leaf: ??$_M_allocate_and_copy@PBVCModifierEntry@CTCHeroAttachableAppearanceModifiers@@@?$vector@VCModifierEntry@CTCHeroAttachableAppearanceModifiers@@V?$allocator@VCModifierEntry@CTCHeroAttachableAppearanceModifiers@@@std@@@std@@QAEPAVCModifierEntry@CTCHeroAttachableAppearanceModifiers@@IPBV23@0@Z
// Identity: proven; own allocation0x708540 <- inner vector op=0x708CA0 <- vector-of-vector __copy0x709090 <- outer op=0x709660 <- CTCHeroAttachableAppearanceModifiers::Copy0x706EC0; PDB AppearanceModifiers vector<vector<CModifierEntry>>; own8-byte stride; const source from vector assignment, malloc0xBFEA0E.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
// Layout: engine/CTCHeroAttachableAppearanceModifiers.h lacks nested CModifierEntry. The outer declaration is naming scope only; the PDB8-byte record has DefIndex+0 and Rendered+4. No outer instance is accessed.
class CTCHeroAttachableAppearanceModifiers { public: class CModifierEntry { public: long DefIndex; bool Rendered; }; };
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

typedef CTCHeroAttachableAppearanceModifiers::CModifierEntry Element;
template Element* std::vector<Element>::_M_allocate_and_copy<const Element*>(unsigned int,const Element*,const Element*);