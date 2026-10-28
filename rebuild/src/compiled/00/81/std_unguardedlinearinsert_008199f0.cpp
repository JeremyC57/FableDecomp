// std::__unguarded_linear_insert(Iter last, T value, Compare comp) -- Insert a complete record after the range sentinel.
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@std@@U12@V?$CKeyPairCompareLess@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@@@std@@YIXPAU?$pair@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@0@U10@V?$CKeyPairCompareLess@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@@@Z
// Identity: Own linear-insert0081be30 and CParticleMorphs::Morphs;40-byte element.
#ifndef _MSC_VER
#define __fastcall
#endif
namespace std {
template<class A,class B> struct pair {A first;B second;pair(){}pair(const pair& other):first(other.first),second(other.second){}};
template<class Iter,class T,class Compare>
void __fastcall __unguarded_linear_insert(Iter last,T value,Compare comp) {
    Iter next=last;--next;
    while(comp(value,*next)){*last=*next;last=next;--next;}
    *last=value;
}
}
template<class K,class V> class CKeyPairCompareLess {public:bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b)const{return a.first<b.first;}};
enum EHeroMorphType {};enum EMeshLocationPoint {};
class CDefString {public:long TablePos;};
// Layout: CParticleMorphs supplies only the nested name scope; CEntry36bytes and pair40bytes. PDB field names and own vector helpers confirm offsets.
class CParticleMorphs {public:class CEntry {public:
    EHeroMorphType MorphType;bool Blending;EMeshLocationPoint Location;CDefString LocationName;long LocationIndex,ParticleIndex,SecondParticleIndex;float StartAtMorphStrength,EndAtMorphStrength;
};};
typedef std::pair<EHeroMorphType,CParticleMorphs::CEntry> Element;
typedef CKeyPairCompareLess<EHeroMorphType,CParticleMorphs::CEntry> Compare;
template void __fastcall std::__unguarded_linear_insert<Element*,Element,Compare>(Element*,Element,Compare);