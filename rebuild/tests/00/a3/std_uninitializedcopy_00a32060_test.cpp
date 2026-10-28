#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// std::__uninitialized_copy(Input first, Input last, Output result, const __false_type&) -- Copy-construct a pair range into raw storage.
// Leaf: ??$__uninitialized_copy@PAU?$pair@KVC3DDummyObject@@@std@@PAU12@@std@@YIPAU?$pair@KVC3DDummyObject@@@0@PAU10@00ABU__false_type@0@@Z
// Identity: Own overflow00a33e80 used by dummy-object vector insert00a349b0 and C3DMesh path00a2fc17;56-byte pair (52-byte Transform/BoneIndex value). Mutable inputs are from original storage in the caller.
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
// Layout: original C3DDummyObject is52bytes: Transform12floats+0, BoneIndex+48. Own vector overflow00a33e80 and PDB confirm56-byte unsigned-key pair.
class CMatrix3x4 {public:float M[12];};
class C3DDummyObject {public:CMatrix3x4 Transform;long BoneIndex;};
typedef std::pair<unsigned long,C3DDummyObject> Element;
template Element* __fastcall std::__uninitialized_copy<Element*,Element*>(Element*,Element*,Element*,const std::__false_type&);
void init(Element& p,int i){p.first=(unsigned long)i;p.second.BoneIndex=100+i;for(int k=0;k<12;++k)p.second.Transform.M[k]=(float)(i+k)*0.25f;}
bool equal(const Element& a,const Element& b){return a.first==b.first && a.second.BoneIndex==b.second.BoneIndex && a.second.Transform.M[0]==b.second.Transform.M[0] && a.second.Transform.M[1]==b.second.Transform.M[1] && a.second.Transform.M[2]==b.second.Transform.M[2] && a.second.Transform.M[3]==b.second.Transform.M[3] && a.second.Transform.M[4]==b.second.Transform.M[4] && a.second.Transform.M[5]==b.second.Transform.M[5] && a.second.Transform.M[6]==b.second.Transform.M[6] && a.second.Transform.M[7]==b.second.Transform.M[7] && a.second.Transform.M[8]==b.second.Transform.M[8] && a.second.Transform.M[9]==b.second.Transform.M[9] && a.second.Transform.M[10]==b.second.Transform.M[10] && a.second.Transform.M[11]==b.second.Transform.M[11];}

int main(){
    Element input[3];for(int i=0;i<3;++i)init(input[i],i+1);
    unsigned char* memory=(unsigned char*)malloc(5*sizeof(Element));if(!memory)return 1;memset(memory,0x5a,5*sizeof(Element));Element* output=(Element*)memory;
    Element* end=std::__uninitialized_copy(input,input+3,output+1,std::__false_type());int ok=end==output+4;
    for(int j=0;j<3;++j)ok=ok && equal(input[j],output[j+1]);
    for(unsigned int q=0;q<sizeof(Element);++q)ok=ok && memory[q]==0x5a && memory[4*sizeof(Element)+q]==0x5a;
    ok=ok && std::__uninitialized_copy(input,input,output,std::__false_type())==output;
    for(int d=1;d<=3;++d)(output+d)->~Element();free(memory);
    puts(ok ? "00a32060_TEST PASS" : "00a32060_TEST FAIL");return ok ? 0 : 1;
}