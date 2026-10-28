#include <stdio.h>
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
void init(Element& p,int n){p.key=n;p.hit_location=0;p.intersection_pos.X=(float)n;p.intersection_pos.Y=(float)n*2.0f;p.intersection_pos.Z=(float)n*4.0f;p.outward_normal.X=1.0f;p.outward_normal.Y=0.0f;p.outward_normal.Z=-1.0f;}
bool equal(const Element& a,const Element& b){return a.key==b.key && a.hit_location==b.hit_location && a.intersection_pos.X==b.intersection_pos.X && a.intersection_pos.Y==b.intersection_pos.Y && a.intersection_pos.Z==b.intersection_pos.Z && a.outward_normal.X==b.outward_normal.X && a.outward_normal.Y==b.outward_normal.Y && a.outward_normal.Z==b.outward_normal.Z;}
void key(Element& e,long n){e.key=n;}

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
    puts(ok ? "00817070_TEST PASS" : "00817070_TEST FAIL");return ok ? 0 : 1;
}