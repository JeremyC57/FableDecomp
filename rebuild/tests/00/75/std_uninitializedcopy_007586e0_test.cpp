#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
void init(Element& p,int i){p.first=(EBodyLocation)i;p.second.Location=(EBodyLocation)(i+1);p.second.ReplacingTextureIndex=100+i;p.second.ReplacingBumpMapIndex=200+i;p.second.WoundTextureIndex=300+i;p.second.WoundBumpMapIndex=400+i;p.second.ScarTextureIndex=500+i;p.second.ScarBumpMapIndex=600+i;p.second.FadeToScarDuration=700+i;p.second.TotalDuration=800+i;p.second.PermanentScarStrength=(float)i*0.25f;}
bool equal(const Element& a,const Element& b){return a.first==b.first && a.second.Location==b.second.Location && a.second.ReplacingTextureIndex==b.second.ReplacingTextureIndex && a.second.ReplacingBumpMapIndex==b.second.ReplacingBumpMapIndex && a.second.WoundTextureIndex==b.second.WoundTextureIndex && a.second.WoundBumpMapIndex==b.second.WoundBumpMapIndex && a.second.ScarTextureIndex==b.second.ScarTextureIndex && a.second.ScarBumpMapIndex==b.second.ScarBumpMapIndex && a.second.FadeToScarDuration==b.second.FadeToScarDuration && a.second.TotalDuration==b.second.TotalDuration && a.second.PermanentScarStrength==b.second.PermanentScarStrength;}

int main(){
    Element input[3];for(int i=0;i<3;++i)init(input[i],i+1);
    unsigned char* memory=(unsigned char*)malloc(5*sizeof(Element));if(!memory)return 1;memset(memory,0x5a,5*sizeof(Element));Element* output=(Element*)memory;
    Element* end=std::__uninitialized_copy(input,input+3,output+1,std::__false_type());int ok=end==output+4;
    for(int j=0;j<3;++j)ok=ok && equal(input[j],output[j+1]);
    for(unsigned int q=0;q<sizeof(Element);++q)ok=ok && memory[q]==0x5a && memory[4*sizeof(Element)+q]==0x5a;
    ok=ok && std::__uninitialized_copy(input,input,output,std::__false_type())==output;
    for(int d=1;d<=3;++d)(output+d)->~Element();free(memory);
    puts(ok ? "007586e0_TEST PASS" : "007586e0_TEST FAIL");return ok ? 0 : 1;
}