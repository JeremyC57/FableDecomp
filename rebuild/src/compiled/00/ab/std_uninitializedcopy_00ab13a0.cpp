// std::__uninitialized_copy(iterator first, iterator last_or_count, iterator_or_value result, const __false_type& tag) -- placement-construct the output range.
// Leaf: ??$__uninitialized_copy@PAVCSequenceInfo@N3DAnimationBuilder@@PAV12@@std@@YIPAVCSequenceInfo@N3DAnimationBuilder@@PAV12@00ABU__false_type@0@@Z
// Identity: proven; own _M_fill_insert0xAB2E40/resize0xAB3290 -> C3DAnimationBuilder::BuildMeshAnimation0xAB2520; COutputAnimation::Sequences, PDB CSequenceInfo52; mutable input from vector insertion for copies.
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
class CMatrix3x4 { public: float M[12]; }; namespace N3DAnimationBuilder { class CSequenceInfo { public: long OriginalIndex; CMatrix3x4 GeneratedTransform; }; }
namespace std { struct __false_type { __false_type() {} }; template<class T,class V> __forceinline void _Construct(T* p,const V& value) { new(p) T(value); } template<class Input,class Output> Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&) { Output current=result; for(;first!=last;++first,++current) _Construct(&*current,*first); return current; } }

typedef N3DAnimationBuilder::CSequenceInfo Element;
template Element* __fastcall std::__uninitialized_copy<Element*,Element*>(Element*,Element*,Element*,const std::__false_type&);