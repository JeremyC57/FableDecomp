// std::__unguarded_linear_insert(Iter last, T value, Compare comp) -- Shift a sentinel-bounded run using the original comparator result.
// Leaf: ??$__unguarded_linear_insert@PAPAVCThing@@PAV1@VCThingCompare_NearestToPos@@@std@@YIXPAPAVCThing@@PAV1@VCThingCompare_NearestToPos@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags: /Gr
// Identity: own comparator007a2f90 and its full PDB symbol establish CThing* with CThingCompare_NearestToPos.
// Layout: named comparator is empty or contains Pos at0,12bytes; owner is not accessed by this algorithm.
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing;class CThingObject;
#include "engine/C3DVector.h"
namespace std {
template<class Iter,class T,class Compare>void __fastcall __unguarded_linear_insert(Iter last,T value,Compare comp){Iter next=last;--next;while(comp(value,*next)){*last=*next;last=next;--next;}*last=value;}
}
class CThingCompare_NearestToPos {public:C3DVector Pos;template<class T>bool operator()(T*,T*)const;};
typedef CThing* Element;
typedef CThingCompare_NearestToPos Compare;
template void __fastcall std::__unguarded_linear_insert<Element*,Element,Compare>(Element*,Element,Compare);