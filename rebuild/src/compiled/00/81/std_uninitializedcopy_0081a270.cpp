// std::__uninitialized_copy(Input first, Input last, Output result, const __false_type&) -- Copy-construct a pair range into raw storage.
// Leaf: ??$__uninitialized_copy@PAU?$pair@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@std@@PAU12@@std@@YIPAU?$pair@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@0@PAU10@00ABU__false_type@0@@Z
// Identity: CParticleMorphs::Morphs and own overflow0081c990/fill-insert0081cd00;40-byte pair (36-byte value). Mutable inputs are from original storage in the caller.
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p){return p;}
namespace std {
template<class A,class B> struct pair {A first;B second;pair(){}pair(const pair& other):first(other.first),second(other.second){}};
struct __false_type {};
template<class T,class V> __forceinline void _Construct(T* p,const V& value){new(p) T(value);}
template<class Input,class Output>
Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&) {
    Output current=result;for(;first!=last;++first,++current)_Construct(&*current,*first);return current;
}
}
enum EHeroMorphType {};enum EMeshLocationPoint {};
class CDefString {public:long TablePos;};
// Layout: CParticleMorphs supplies only the nested name scope; CEntry36bytes and pair40bytes. PDB field names and own vector helpers confirm offsets.
class CParticleMorphs {public:class CEntry {public:
    EHeroMorphType MorphType;bool Blending;EMeshLocationPoint Location;CDefString LocationName;long LocationIndex,ParticleIndex,SecondParticleIndex;float StartAtMorphStrength,EndAtMorphStrength;
};};
typedef std::pair<EHeroMorphType,CParticleMorphs::CEntry> Element;
template Element* __fastcall std::__uninitialized_copy<Element*,Element*>(Element*,Element*,Element*,const std::__false_type&);