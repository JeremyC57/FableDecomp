#include <stdio.h>
// std::__unguarded_linear_insert(Iter last, T value, Compare comp) -- Insert into a sorted run whose minimum is a sentinel.
// Leaf: ??$__unguarded_linear_insert@PAVCSortElement@CEngineSubPrimitiveRenderer@@V12@U?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@std@@@std@@YIXPAVCSortElement@CEngineSubPrimitiveRenderer@@V12@U?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@0@@Z
// Identity: own linear-insert caller00b85870 and its sorting chain establish the element and comparator.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
namespace std {
template<class A,class B> struct pair {
    A first;B second;
    pair() {}
    pair(const pair& other):first(other.first),second(other.second){}
};
template<class T> struct less {bool operator()(const T& a,const T& b)const{return a<b;}};
template<class Iter,class T,class Compare>
void __fastcall __unguarded_linear_insert(Iter last,T value,Compare comp) {
    Iter next=last;--next;
    while(comp(value,*next)) {*last=*next;last=next;--next;}
    *last=value;
}
}
template<class K,class V> class CKeyPairCompareLess {
public:bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b)const{return a.first<b.first;}
};
class CEngineSubPrimitiveBase;
// Layout: outer class provides a name scope only. CSortElement is unsigned SortValue+0 and SubPrimitive pointer+4, confirmed by PDB and own sort caller. No outer object is accessed.
class CEngineSubPrimitiveRenderer {public:class CSortElement {public:
    unsigned long SortValue;CEngineSubPrimitiveBase* SubPrimitive;
    CSortElement(){}
    bool operator<(const CSortElement& other)const{return SortValue<other.SortValue;}
};};
typedef CEngineSubPrimitiveRenderer::CSortElement Element;
typedef std::less<CEngineSubPrimitiveRenderer::CSortElement> Compare;
template void __fastcall std::__unguarded_linear_insert<Element*,Element,Compare>(Element*,Element,Compare);
class CEngineSubPrimitiveBase {};
static CEngineSubPrimitiveBase object1,object2;
void set(Element& e,unsigned long key,int value){e.SortValue=key;e.SubPrimitive=value==1?&object1:&object2;}
unsigned long key(const Element& e){return e.SortValue;}
int value(const Element& e){return e.SubPrimitive==&object1?1:2;}

int main() {
    Element data[6],incoming;int ok=1;
    set(data[0],0,1);set(data[1],4,1);set(data[2],4,2);set(data[3],9,1);set(data[4],99,1);set(data[5],123,2);set(incoming,4,2);
    std::__unguarded_linear_insert(data+4,incoming,Compare());
    ok=ok && key(data[0])==0 && key(data[1])==4 && value(data[1])==1 && key(data[2])==4 && value(data[2])==2 && key(data[3])==4 && value(data[3])==2 && key(data[4])==9 && key(data[5])==123;
    set(data[1],4,1);set(data[2],4,2);set(data[3],9,1);set(incoming,1,2);
    std::__unguarded_linear_insert(data+4,incoming,Compare());
    ok=ok && key(data[0])==0 && key(data[1])==1 && key(data[2])==4 && value(data[2])==1 && key(data[3])==4 && value(data[3])==2 && key(data[4])==9;
    set(incoming,0x80000000UL,2);std::__unguarded_linear_insert(data+5,incoming,Compare());
    ok=ok && key(data[4])==9 && key(data[5])==0x80000000UL && value(data[5])==2;
    puts(ok ? "00b84f40_TEST PASS" : "00b84f40_TEST FAIL");return ok ? 0 : 1;
}