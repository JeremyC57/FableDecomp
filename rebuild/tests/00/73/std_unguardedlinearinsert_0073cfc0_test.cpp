#include <stdio.h>
// std::__unguarded_linear_insert(Iter last, T value, Compare comp) -- Insert into a sorted run whose minimum is a sentinel.
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@KVCRegisteredModeInfo@CTCCreatureModeManager@@@std@@U12@V?$CKeyPairCompareLess@KVCRegisteredModeInfo@CTCCreatureModeManager@@@@@std@@YIXPAU?$pair@KVCRegisteredModeInfo@CTCCreatureModeManager@@@0@U10@V?$CKeyPairCompareLess@KVCRegisteredModeInfo@CTCCreatureModeManager@@@@@Z
// Identity: own linear-insert caller00742360 and its sorting chain establish the element and comparator.
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
class CThing;class CModeBase;
// Layout: outer class supplies only the nested type scope. PDB CRegisteredModeInfo has one AllocFunc pointer; the retail pair stride is8. No outer object is accessed.
class CTCCreatureModeManager {public:class CRegisteredModeInfo {public:CModeBase* (__cdecl* AllocFunc)(CThing*);};};
typedef std::pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo> Element;
typedef CKeyPairCompareLess<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo> Compare;
template void __fastcall std::__unguarded_linear_insert<Element*,Element,Compare>(Element*,Element,Compare);
CModeBase* __cdecl ModeA(CThing*) {return 0;}
CModeBase* __cdecl ModeB(CThing*) {return 0;}
void set(Element& e,unsigned long key,int value){e.first=key;e.second.AllocFunc=value==1?ModeA:ModeB;}
unsigned long key(const Element& e){return e.first;}
int value(const Element& e){return e.second.AllocFunc==ModeA?1:2;}

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
    puts(ok ? "0073cfc0_TEST PASS" : "0073cfc0_TEST FAIL");return ok ? 0 : 1;
}