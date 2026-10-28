#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
void init(Element& p,int i){p.first=(EHeroMorphType)i;p.second.MorphType=(EHeroMorphType)(i+1);p.second.Blending=(i%2)!=0;p.second.Location=(EMeshLocationPoint)(i+2);p.second.LocationName.TablePos=100+i;p.second.LocationIndex=200+i;p.second.ParticleIndex=300+i;p.second.SecondParticleIndex=400+i;p.second.StartAtMorphStrength=(float)i*0.25f;p.second.EndAtMorphStrength=(float)i*0.5f;}
bool equal(const Element& a,const Element& b){return a.first==b.first && a.second.MorphType==b.second.MorphType && a.second.Blending==b.second.Blending && a.second.Location==b.second.Location && a.second.LocationName.TablePos==b.second.LocationName.TablePos && a.second.LocationIndex==b.second.LocationIndex && a.second.ParticleIndex==b.second.ParticleIndex && a.second.SecondParticleIndex==b.second.SecondParticleIndex && a.second.StartAtMorphStrength==b.second.StartAtMorphStrength && a.second.EndAtMorphStrength==b.second.EndAtMorphStrength;}

int main(){
    Element input[3];for(int i=0;i<3;++i)init(input[i],i+1);
    unsigned char* memory=(unsigned char*)malloc(5*sizeof(Element));if(!memory)return 1;memset(memory,0x5a,5*sizeof(Element));Element* output=(Element*)memory;
    Element* end=std::__uninitialized_copy(input,input+3,output+1,std::__false_type());int ok=end==output+4;
    for(int j=0;j<3;++j)ok=ok && equal(input[j],output[j+1]);
    for(unsigned int q=0;q<sizeof(Element);++q)ok=ok && memory[q]==0x5a && memory[4*sizeof(Element)+q]==0x5a;
    ok=ok && std::__uninitialized_copy(input,input,output,std::__false_type())==output;
    for(int d=1;d<=3;++d)(output+d)->~Element();free(memory);
    puts(ok ? "0081a270_TEST PASS" : "0081a270_TEST FAIL");return ok ? 0 : 1;
}