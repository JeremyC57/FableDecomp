// std::__unguarded_linear_insert(Iter last, T value, Compare comp) -- Insert a complete record after the range sentinel.
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@std@@U12@V?$CKeyPairCompareLess@W4EBodyLocation@@VCEntry@CWoundMorphs@@@@@std@@YIXPAU?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@0@U10@V?$CKeyPairCompareLess@W4EBodyLocation@@VCEntry@CWoundMorphs@@@@@Z
// Identity: CWoundMorphs::Morphs; own linear-insert00758e00 and matching44-byte pair helpers.
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
enum EBodyLocation {};
// Layout: CWoundMorphs supplies only the nested name scope; original CEntry is40bytes, with the named fields below from PDB and retail pair stride44.
class CWoundMorphs {public:class CEntry {public:
    EBodyLocation Location;long ReplacingTextureIndex,ReplacingBumpMapIndex,WoundTextureIndex,WoundBumpMapIndex,ScarTextureIndex,ScarBumpMapIndex,FadeToScarDuration,TotalDuration;float PermanentScarStrength;
};};
typedef std::pair<EBodyLocation,CWoundMorphs::CEntry> Element;
typedef CKeyPairCompareLess<EBodyLocation,CWoundMorphs::CEntry> Compare;
template void __fastcall std::__unguarded_linear_insert<Element*,Element,Compare>(Element*,Element,Compare);