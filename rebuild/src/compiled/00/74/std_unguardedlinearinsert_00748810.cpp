// std::__unguarded_linear_insert(Iter last, T value, Compare comp) -- Shift a sentinel-bounded run using the original comparator result.
// Leaf: ??$__unguarded_linear_insert@PAPBVCThing@@PBV1@VCCompare_FurthestFromInitPos2D@?A0x
// Flags: /Gr
// Identity: own comparator007483e0 and its full PDB symbol establish const CThing* with CCompare_FurthestFromInitPos2D.
// Layout: named comparator is empty or contains Pos at0,12bytes; owner is not accessed by this algorithm.
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing;class CThingObject;
class C3DVector {public:float X,Y,Z;};
namespace std {
template<class Iter,class T,class Compare>void __fastcall __unguarded_linear_insert(Iter last,T value,Compare comp){Iter next=last;--next;while(comp(value,*next)){*last=*next;last=next;--next;}*last=value;}
}
namespace {class CCompare_FurthestFromInitPos2D {public:bool operator()(const CThing*,const CThing*)const;};}
typedef const CThing* Element;
typedef CCompare_FurthestFromInitPos2D Compare;
template void __fastcall std::__unguarded_linear_insert<Element*,Element,Compare>(Element*,Element,Compare);