#include <stdio.h>
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
void init(Element& p,int i){p.first=(EHeroMorphType)i;p.second.MorphType=(EHeroMorphType)(i+1);p.second.Blending=(i%2)!=0;p.second.Location=(EMeshLocationPoint)(i+2);p.second.LocationName.TablePos=100+i;p.second.LocationIndex=200+i;p.second.ParticleIndex=300+i;p.second.SecondParticleIndex=400+i;p.second.StartAtMorphStrength=(float)i*0.25f;p.second.EndAtMorphStrength=(float)i*0.5f;}
bool equal(const Element& a,const Element& b){return a.first==b.first && a.second.MorphType==b.second.MorphType && a.second.Blending==b.second.Blending && a.second.Location==b.second.Location && a.second.LocationName.TablePos==b.second.LocationName.TablePos && a.second.LocationIndex==b.second.LocationIndex && a.second.ParticleIndex==b.second.ParticleIndex && a.second.SecondParticleIndex==b.second.SecondParticleIndex && a.second.StartAtMorphStrength==b.second.StartAtMorphStrength && a.second.EndAtMorphStrength==b.second.EndAtMorphStrength;}

void key(Element& e,long n){e.first=(EHeroMorphType)n;}

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
    puts(ok ? "008199f0_TEST PASS" : "008199f0_TEST FAIL");return ok ? 0 : 1;
}