// std::uninitialized_fill_n(Iter first, unsigned int n, const T& value) -- Copy-construct repeated records into raw storage.
// Leaf: ??$uninitialized_fill_n@PAU?$pair@JVCUnprovenMapped8_0086f340@@@std@@IU12@@std@@YIPAU?$pair@JVCUnprovenMapped8_0086f340@@@0@PAU10@IABU10@@Z
// Identity: descriptive; Own fill-insert0086fd20 shares copy-backward0086eff0 with signed-key linear-insert0086fa00. Long key and8-byte POD value established; mapped class identity not proven. Do not infer CSkeletalMorph from adjacency (donor value is12 bytes).
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
// Layout: retail mapped value is8bytes copied in two words; original type and field names unresolved.
class CUnprovenMapped8_0086f340 {public:unsigned long _word0,_word4;};
typedef std::pair<long,CUnprovenMapped8_0086f340> Element;
template Element* __fastcall std::uninitialized_fill_n<Element*,unsigned int,Element>(Element*,unsigned int,const Element&);