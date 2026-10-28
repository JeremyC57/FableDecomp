// std::__uninitialized_fill_n(iterator first, iterator last_or_count, iterator_or_value result, const __false_type& tag) -- placement-construct the output range.
// Leaf: ??$__uninitialized_fill_n@PAVCTextureManagerPoolInfo@@IV1@@std@@YIPAVCTextureManagerPoolInfo@@PAV1@IABV1@ABU__false_type@0@@Z
// Identity: proven; own CTextureManagerPool Initialize/reallocation callers0xB3E96C/B3EB0F, PDB Width/Height/PixelFormat/UseMipmaps/Count20; mutable input from vector insertion for copies.
// Layout: no record header. PixelFormat header is reused unchanged where present; CScriptedMapBrush outer header lacks the nested cell, so its declaration is naming scope only. Actual PDB field types/keys preserve sizes and copy semantics.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
#include "engine/CPixelFormat.h"
 class CTextureManagerPoolInfo { public: unsigned long Width,Height; CPixelFormat PixelFormat; bool UseMipmaps; unsigned long Count; };
namespace std { struct __false_type { __false_type() {} }; template<class T,class V> __forceinline void _Construct(T* p,const V& value) { new(p) T(value); } template<class Iter,class Size,class T> Iter __fastcall __uninitialized_fill_n(Iter first,Size count,const T& value,const __false_type&) { Iter current=first; for(;count>0;--count,++current) _Construct(&*current,value); return current; } }

typedef CTextureManagerPoolInfo Element;
template Element* __fastcall std::__uninitialized_fill_n<Element*,unsigned int,Element>(Element*,unsigned int,const Element&,const std::__false_type&);