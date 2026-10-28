// std::vector::push_back(const T& value) -- Construct in spare capacity or delegate growth.
// Leaf: ?push_back@?$vector@UKnickKnack@@V?$allocator@UKnickKnack@@@std@@@std@@QAEXABUKnickKnack@@@Z
// Identity: Own overflow007d9c90, CTCTavernGameSpotTheAddition::KnickKnacks and PDB struct/copy constructor establish KnickKnack.
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
typedef KnickKnack Element;
template void std::vector<Element>::push_back(const Element&);