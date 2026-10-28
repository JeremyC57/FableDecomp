#include <stdio.h>
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
void init(Element& p,int i){p.first=(EBodyLocation)i;p.second.Location=(EBodyLocation)(i+1);p.second.ReplacingTextureIndex=100+i;p.second.ReplacingBumpMapIndex=200+i;p.second.WoundTextureIndex=300+i;p.second.WoundBumpMapIndex=400+i;p.second.ScarTextureIndex=500+i;p.second.ScarBumpMapIndex=600+i;p.second.FadeToScarDuration=700+i;p.second.TotalDuration=800+i;p.second.PermanentScarStrength=(float)i*0.25f;}
bool equal(const Element& a,const Element& b){return a.first==b.first && a.second.Location==b.second.Location && a.second.ReplacingTextureIndex==b.second.ReplacingTextureIndex && a.second.ReplacingBumpMapIndex==b.second.ReplacingBumpMapIndex && a.second.WoundTextureIndex==b.second.WoundTextureIndex && a.second.WoundBumpMapIndex==b.second.WoundBumpMapIndex && a.second.ScarTextureIndex==b.second.ScarTextureIndex && a.second.ScarBumpMapIndex==b.second.ScarBumpMapIndex && a.second.FadeToScarDuration==b.second.FadeToScarDuration && a.second.TotalDuration==b.second.TotalDuration && a.second.PermanentScarStrength==b.second.PermanentScarStrength;}

void key(Element& e,long n){e.first=(EBodyLocation)n;}

int main(){
    Element data[6],original[6],incoming;int i;
    for(i=0;i<6;++i)init(data[i],i+1);
    key(data[0],0);key(data[1],2);key(data[2],2);key(data[3],7);key(data[4],9);key(data[5],99);
    for(i=0;i<6;++i)original[i]=data[i];init(incoming,20);key(incoming,2);
    std::__unguarded_linear_insert(data+4,incoming,Compare());
    int ok=equal(data[0],original[0]) && equal(data[1],original[1]) && equal(data[2],original[2]) && equal(data[3],incoming) && equal(data[4],original[3]) && equal(data[5],original[5]);
    for(i=0;i<6;++i)data[i]=original[i];key(incoming,1);
    std::__unguarded_linear_insert(data+4,incoming,Compare());
    ok=ok && equal(data[0],original[0]) && equal(data[1],incoming) && equal(data[2],original[1]) && equal(data[3],original[2]) && equal(data[4],original[3]) && equal(data[5],original[5]);
    key(incoming,10);std::__unguarded_linear_insert(data+5,incoming,Compare());ok=ok && equal(data[5],incoming) && equal(data[4],original[3]);
    puts(ok ? "00758390_TEST PASS" : "00758390_TEST FAIL");return ok ? 0 : 1;
}