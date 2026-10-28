// std::__unguarded_linear_insert(Iter last, T value, Compare comp) -- Shift a sentinel-bounded run using the original comparator result.
// Leaf: ??$__unguarded_linear_insert@PAPAVCThing@@PAV1@VCThingCompare_NearestToPos@@@std@@YIXPAPAVCThing@@PAV1@VCThingCompare_NearestToPos@@@Z
// Flags: /Gr
// Identity: own comparator007a2f90 and its full PDB symbol establish CThing* with CThingCompare_NearestToPos.
// Layout: named comparator is empty or contains Pos at0,12bytes; owner is not accessed by this algorithm.
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing;class CThingObject;
class C3DVector {public:float X,Y,Z;};
namespace std {
template<class Iter,class T,class Compare>void __fastcall __unguarded_linear_insert(Iter last,T value,Compare comp){Iter next=last;--next;while(comp(value,*next)){*last=*next;last=next;--next;}*last=value;}
}
class CThingCompare_NearestToPos {public:C3DVector Pos;template<class T>bool operator()(T*,T*)const;};
typedef CThing* Element;
typedef CThingCompare_NearestToPos Compare;
template void __fastcall std::__unguarded_linear_insert<Element*,Element,Compare>(Element*,Element,Compare);