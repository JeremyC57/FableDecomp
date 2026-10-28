// std::__uninitialized_copy(iterator first, iterator last_or_count, iterator_or_value result, const __false_type& tag) -- placement-construct the output range.
// Leaf: ??$__uninitialized_copy@PAVCEntry@CParticleMorphs@@PAV12@@std@@YIPAVCEntry@CParticleMorphs@@PAV12@00ABU__false_type@0@@Z
// Identity: proven; own _M_fill_insert0x81D200/resize0x81DB00 called by CParticleMorphs::GetAllMorphsOfType0x81B160, PDB CEntry36; mutable input from vector insertion for copies.
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
enum EHeroMorphType {}; enum EMeshLocationPoint {}; class CDefString { public: long TablePos; }; class CParticleMorphs { public: class CEntry { public: EHeroMorphType MorphType; bool Blending; EMeshLocationPoint Location; CDefString LocationName; long LocationIndex,ParticleIndex,SecondParticleIndex; float StartAtMorphStrength,EndAtMorphStrength; }; };

namespace std { struct __false_type { __false_type() {} }; template<class T,class V> __forceinline void _Construct(T* p,const V& value) { new(p) T(value); } template<class Input,class Output> Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&) { Output current=result; for(;first!=last;++first,++current) _Construct(&*current,*first); return current; } }

typedef CParticleMorphs::CEntry Element;
template Element* __fastcall std::__uninitialized_copy<Element*,Element*>(Element*,Element*,Element*,const std::__false_type&);