// std::__unguarded_linear_insert(Iter last, T value, Compare comp) -- Shift a sentinel-bounded run using the original comparator result.
// Leaf: ??$__unguarded_linear_insert@PAPBVCThing@@PBV1@VCCompare_FurthestFromInitPos2D@?A0x
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags: /Gr
// Identity: own comparator007483e0 and its full PDB symbol establish const CThing* with CCompare_FurthestFromInitPos2D.
// Layout: named comparator is empty or contains Pos at0,12bytes; owner is not accessed by this algorithm.
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing;class CThingObject;
#include "engine/C3DVector.h"
namespace std {
template<class Iter,class T,class Compare>void __fastcall __unguarded_linear_insert(Iter last,T value,Compare comp){Iter next=last;--next;while(comp(value,*next)){*last=*next;last=next;--next;}*last=value;}
}
namespace {class CCompare_FurthestFromInitPos2D {public:bool operator()(const CThing*,const CThing*)const;};}
typedef const CThing* Element;
typedef CCompare_FurthestFromInitPos2D Compare;
template void __fastcall std::__unguarded_linear_insert<Element*,Element,Compare>(Element*,Element,Compare);