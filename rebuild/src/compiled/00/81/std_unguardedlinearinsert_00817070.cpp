// std::__unguarded_linear_insert(Iter last, T value, Compare comp) -- Insert a complete record after the range sentinel.
// Leaf: ??$__unguarded_linear_insert@PAUTHitLocationIntersectionElement@?A0x
// Identity: Own linear-insert00818240/PDB anonymous record fields key,hit_location,intersection_pos,outward_normal;32-byte element.
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
struct CHitLocation;
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}};
namespace {
// Layout: original anonymous hit record is32bytes: key+0,hit_location+4,intersection_pos+8,outward_normal+20; own caller00818240 and PDB types confirm.
struct THitLocationIntersectionElement {long key;CHitLocation* hit_location;C3DVector intersection_pos,outward_normal;};
struct THitLocationIntersectionElementPred {bool operator()(const THitLocationIntersectionElement& a,const THitLocationIntersectionElement& b)const{return a.key<b.key;}};
}
typedef THitLocationIntersectionElement Element;
typedef THitLocationIntersectionElementPred Compare;
template void __fastcall std::__unguarded_linear_insert<Element*,Element,Compare>(Element*,Element,Compare);