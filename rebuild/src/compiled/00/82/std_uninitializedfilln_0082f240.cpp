// std::__uninitialized_fill_n(iterator first, iterator last_or_count, iterator_or_value result, const __false_type& tag) -- placement-construct the output range.
// Leaf: ??$__uninitialized_fill_n@PAVCScriptedMapCell@CScriptedMapBrush@@IV12@@std@@YIPAVCScriptedMapCell@CScriptedMapBrush@@PAV12@IABV12@ABU__false_type@0@@Z
// Identity: proven; own CScriptedMapBrush ctor0x82E600 -> vectorctor0x82F630, Cells20-byte Height/Theme/ThemeBlend; existing landed ctor proves fill helper; mutable input from vector insertion for copies.
// Layout: engine/CScriptedMapBrush.h contains opaque Cells bytes but has no nested CScriptedMapCell declaration. The local outer declaration supplies naming scope only; the target constructs the independently proved20-byte nested cell and never accesses a brush instance.
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
class CScriptedMapBrush { public: class CScriptedMapCell { public: float Height; unsigned long Theme[3]; unsigned char ThemeBlend[2]; }; };
namespace std { struct __false_type { __false_type() {} }; template<class T,class V> __forceinline void _Construct(T* p,const V& value) { new(p) T(value); } template<class Iter,class Size,class T> Iter __fastcall __uninitialized_fill_n(Iter first,Size count,const T& value,const __false_type&) { Iter current=first; for(;count>0;--count,++current) _Construct(&*current,value); return current; } }

typedef CScriptedMapBrush::CScriptedMapCell Element;
template Element* __fastcall std::__uninitialized_fill_n<Element*,unsigned int,Element>(Element*,unsigned int,const Element&,const std::__false_type&);