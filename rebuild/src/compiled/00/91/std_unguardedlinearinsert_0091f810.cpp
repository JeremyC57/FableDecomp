// std::__unguarded_linear_insert(Iter last, T value, Compare comp) -- Shift a sentinel-bounded run using the original comparator result.
// Leaf: ??$__unguarded_linear_insert@PAPBVCThingObject@@PBV1@VCThingCompare_Nearest@@@std@@YIXPAPBVCThingObject@@PBV1@VCThingCompare_Nearest@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags: /Gr
// Identity: own comparator0091f740 and its full PDB symbol establish const CThingObject* with CThingCompare_Nearest.
// Layout: named comparator is empty or contains Pos at0,12bytes; owner is not accessed by this algorithm.
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing;class CThingObject;
#include "engine/C3DVector.h"
namespace std {
template<class Iter,class T,class Compare>void __fastcall __unguarded_linear_insert(Iter last,T value,Compare comp){Iter next=last;--next;while(comp(value,*next)){*last=*next;last=next;--next;}*last=value;}
}
class CThingCompare_Nearest {public:C3DVector Pos;template<class T>T* operator()(T*,T*)const;};
typedef const CThingObject* Element;
typedef CThingCompare_Nearest Compare;
template void __fastcall std::__unguarded_linear_insert<Element*,Element,Compare>(Element*,Element,Compare);