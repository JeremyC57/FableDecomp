// std::__uninitialized_copy(Input first, Input last, Output result, const __false_type&) -- Copy-construct a pair range into raw storage.
// Leaf: ??$__uninitialized_copy@PAU?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@std@@PAU12@@std@@YIPAU?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@0@PAU10@00ABU__false_type@0@@Z
// Identity: CWoundMorphs::Morphs and own vector overflow00759190/fill-insert00759290;44-byte pair (40-byte value). Mutable inputs are from original storage in the caller.
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
enum EBodyLocation {};
// Layout: CWoundMorphs supplies only the nested name scope; original CEntry is40bytes, with the named fields below from PDB and retail pair stride44.
class CWoundMorphs {public:class CEntry {public:
    EBodyLocation Location;long ReplacingTextureIndex,ReplacingBumpMapIndex,WoundTextureIndex,WoundBumpMapIndex,ScarTextureIndex,ScarBumpMapIndex,FadeToScarDuration,TotalDuration;float PermanentScarStrength;
};};
typedef std::pair<EBodyLocation,CWoundMorphs::CEntry> Element;
template Element* __fastcall std::__uninitialized_copy<Element*,Element*>(Element*,Element*,Element*,const std::__false_type&);