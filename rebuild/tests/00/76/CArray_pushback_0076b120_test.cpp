#include <stdio.h>
#include <stdlib.h>
// std::vector::push_back(const T& value) -- Construct in spare capacity or delegate growth.
// Leaf: ?push_back@?$vector@U?$pair@KVC3DVector@@@std@@V?$allocator@U?$pair@KVC3DVector@@@std@@@2@@std@@QAEXABU?$pair@KVC3DVector@@@2@@Z
// Identity: Own overflow0076ada0 and CParticleUpdateParameters<unsigned long,unsigned long> RTTI0125ffdc, VectorParams member establish unsigned-long/C3DVector pairs.
#ifndef _MSC_VER
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p){return p;}
// Layout: original C3DVector has X,Y,Z at0,4,8; no active shared header exists.
class C3DVector {public:float X,Y,Z;C3DVector(){} C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}};
class CThingObject;
// Layout: original KnickKnack is Pos+0, Thing+12,16bytes on x86.
struct KnickKnack {C3DVector Pos;CThingObject* Thing;KnickKnack(){} KnickKnack(const KnickKnack& v):Pos(v.Pos),Thing(v.Thing){}};
namespace QuadricOptimiserInternals {
// Layout: original COptimisedEdge fields are at0,4,8,12,16bytes on x86.
class COptimisedEdge {public:unsigned long FaceA,FaceB,UniqueVertex1,UniqueVertex2;};
}
namespace std {
template<class F,class S>struct pair {F first;S second;pair(){}pair(const pair& p):first(p.first),second(p.second){}};
struct __false_type {__false_type(){}};
template<class T>class allocator {};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A=allocator<T> >class vector {
public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
void push_back(const T& value){if(_M_finish!=_M_end_of_storage._M_data){new(_M_finish) T(value);++_M_finish;}else _M_insert_overflow(_M_finish,value,__false_type(),1,true);}
protected:void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool);
};
}
typedef std::pair<unsigned long,C3DVector> Element;
template void std::vector<Element>::push_back(const Element&);

class CThingObject {};
static int grows,bad;static const void* expectedValue;static const void* expectedPosition;static const void* expectedVector;
namespace std {template<class T,class A>void vector<T,A>::_M_insert_overflow(T* pos,const T& value,const __false_type&,unsigned int n,bool atEnd){++grows;if(this!=expectedVector || pos!=expectedPosition || &value!=expectedValue || n!=1 || !atEnd)bad=1;}}
static bool same(const Element& p,const Element& input){return p.first==input.first && p.second.X==input.second.X && p.second.Y==input.second.Y && p.second.Z==input.second.Z;}
int main(){
 Element input;CThingObject thing;input.first=7;input.second.X=1.5f;input.second.Y=-2.0f;input.second.Z=4.0f;
 Element* data=(Element*)malloc(2*sizeof(Element));if(!data)return 1;
 std::vector<Element> v;v._M_start=data;v._M_finish=data;v._M_end_of_storage._M_data=data+2;
 v.push_back(input);int ok=v._M_finish==data+1 && same(data[0],input) && !grows;Element saved(input);
 input.first=19;input.second.Y=8.0f;
 v.push_back(input);ok=ok && v._M_finish==data+2 && same(data[1],input) && same(data[0],saved) && !grows;
 expectedValue=&input;expectedPosition=data+2;expectedVector=&v;v.push_back(input);
 ok=ok && grows==1 && !bad && v._M_finish==data+2 && same(data[1],input) && same(data[0],saved);
 data[0].~Element();data[1].~Element();free(data);
 puts(ok ? "0076b120_TEST PASS" : "0076b120_TEST FAIL");return ok?0:1;
}