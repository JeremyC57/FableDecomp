// std::uninitialized_fill_n(Iter first, unsigned int n, const T& value) -- Copy-construct repeated records into raw storage.
// Leaf: ??$uninitialized_fill_n@PAVC3DVector@@IV1@@std@@YIPAVC3DVector@@PAV1@IABV1@@Z
// Identity: proven; Own fill-insert0075bb00 calls the same C3DVector uninitialized-copy0066ddf0 as established C3DVector vector operations;12-byte record.
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
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}};
typedef C3DVector Element;
template Element* __fastcall std::uninitialized_fill_n<Element*,unsigned int,Element>(Element*,unsigned int,const Element&);