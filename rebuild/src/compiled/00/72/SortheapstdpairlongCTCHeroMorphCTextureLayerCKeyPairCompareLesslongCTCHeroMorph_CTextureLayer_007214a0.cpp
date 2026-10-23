// 007214a0 N007214a0::sort_heap<Element,CKeyPairCompareLess> (STLport; catalog: _Sort_heap<std::pair<long,CTCHeroMorph::CTextureLayer>*,CKeyPairCompareLess<long,CTCHeroMorph::_Sort_heap<std::pair<long,CTCHeroMorph::CTextureLayer>*,CKeyPairCompareLess<long,CTCHeroMorph::CTextureLa)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: sort_heap
// Layout: PDB8268-8275 physical Colour B0,G1,R2,A3; retail721561-7215a0 copies components at2,1,0,3. Stride24; enclosing payload identity descriptive because manifest type conflicts with observed colors.
#include <new>
namespace N007214a0 {

struct CKeyPairCompareLess {unsigned int State;};

struct Colour {unsigned char B,G,R,A;Colour(){}__forceinline void Set(unsigned char red,unsigned char green,unsigned char blue,unsigned char alpha){R=red;G=green;B=blue;A=alpha;}__forceinline Colour(const Colour& o){Set(o.R,o.G,o.B,o.A);}};
struct Element {int Key;unsigned int A,B,C;Colour D,E;Element(){} __forceinline Element(const Element& o):Key(o.Key),A(o.A),B(o.B),C(o.C),D(o.D),E(o.E) {}};
template<class T,class C> void __fastcall __adjust_heap(T*,int,int,T,C);
template<class T,class C> __forceinline void __pop_heap(T* first,T* last,C comp){T* result=last-1;T value=*result;*result=*first;__adjust_heap(first,0,int(result-first),value,comp);}
template<class T,class C> void __fastcall sort_heap(T* first,T* last,C comp) {
 while(last-first>1) __pop_heap(first,last--,comp);
}
template void __fastcall sort_heap<Element,CKeyPairCompareLess>(Element*,Element*,CKeyPairCompareLess);

}