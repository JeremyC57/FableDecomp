// std::__unguarded_linear_insert(Iter last, T value, Compare comp) -- Insert into a sorted run whose minimum is a sentinel.
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@KK@std@@U12@V?$CKeyPairCompareLess@KK@@@std@@YIXPAU?$pair@KK@0@U10@V?$CKeyPairCompareLess@KK@@@Z
// Identity: own linear-insert caller009b5920 and its sorting chain establish the element and comparator.
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
typedef std::pair<unsigned long,unsigned long> Element;
typedef CKeyPairCompareLess<unsigned long,unsigned long> Compare;
template void __fastcall std::__unguarded_linear_insert<Element*,Element,Compare>(Element*,Element,Compare);