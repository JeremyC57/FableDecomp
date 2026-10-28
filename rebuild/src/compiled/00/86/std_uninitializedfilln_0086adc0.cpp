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