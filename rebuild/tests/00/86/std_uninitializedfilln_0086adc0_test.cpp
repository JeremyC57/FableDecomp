#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// std::uninitialized_fill_n(Iter first, unsigned int n, const T& value) -- Copy-construct repeated records into raw storage.
// Leaf: ??$uninitialized_fill_n@PAU?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@IU12@@std@@YIPAU?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@0@PAU10@IABU10@@Z
// Identity: proven; Own fill-insert0086ceb0 shares copy-backward0086a5c0 with known texture-morph linear-insert0086c5c0; PDB NewTexture/GroupID8-byte value and unsigned key.
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
template<class Iter,class Size,class T>
inline Iter __fastcall __uninitialized_fill_n(Iter first,Size n,const T& value,const __false_type&){for(;n>0;--n,++first)_Construct(&*first,value);return first;}
template<class Iter,class Size,class T>
Iter __fastcall uninitialized_fill_n(Iter first,Size n,const T& value){return __uninitialized_fill_n(first,n,value,__false_type());}
}
// Layout: outer class is nested-name scope only; PDB CTextureMorph fields are NewTexture+0 and GroupID+4. No outer instance is accessed.
class CRandomAppearanceMorph {public:class CTextureMorph {public:unsigned long NewTexture,GroupID;};};
typedef std::pair<unsigned long,CRandomAppearanceMorph::CTextureMorph> Element;
template Element* __fastcall std::uninitialized_fill_n<Element*,unsigned int,Element>(Element*,unsigned int,const Element&);

int main(){
    Element value;value.first=7;value.second.NewTexture=9;value.second.GroupID=13;
    unsigned char* memory=(unsigned char*)malloc(5*sizeof(Element));if(!memory)return 1;memset(memory,0x5a,5*sizeof(Element));
    Element* first=(Element*)memory;Element* result=std::uninitialized_fill_n(first+1,3u,value);int ok=result==first+4;
    for(int i=1;i<=3;++i){Element* p=first+i;ok=ok && (p->first==value.first && p->second.NewTexture==value.second.NewTexture && p->second.GroupID==value.second.GroupID);}
    for(unsigned int k=0;k<sizeof(Element);++k)ok=ok && memory[k]==0x5a && memory[4*sizeof(Element)+k]==0x5a;
    ok=ok && std::uninitialized_fill_n(first,0u,value)==first;
    for(int j=1;j<=3;++j)(first+j)->~Element();free(memory);
    puts(ok ? "0086adc0_TEST PASS" : "0086adc0_TEST FAIL");return ok ? 0 : 1;
}