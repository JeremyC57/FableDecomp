// std::__introsort_loop(std::pair<long,CIntelligentPointer<CThing> >*, std::pair<long,CIntelligentPointer<CThing> >*, std::pair<long,CIntelligentPointer<CThing> >*, int, CKeyPairCompareLess<long,CIntelligentPointer<CThing> >) -- STLport introsort loop of a (key, thing pointer) vector map: median-of-three partition, recurse on the right part while more than 16 entries remain, partial_sort once the depth limit runs out
// Grade: DIFFER(277v277) -- register colouring only (21 difflines): retail binds first->ebx and last->edi, ours first->edi and last->ebx (c2prio: __first prio 248 > __last 244, both ...ibp, first takes edi in table order; the +24 comes from the two `return __a` median blocks). A visible noinline __unguarded_partition body (probed, opt-in then dropped from hdr/) does not move it. 1 grade.
// Leaf: ??$__introsort_loop@PAU?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@U12@HV?$CKeyPairCompareLess@JV?$CIntelligentPointer@VCThing@@@@@@@std@@YIXPAU?$pair@JV?$CIntelligentPointer@VCThing@@@@@0@00HV?$CKeyPairCompareLess@JV?$CIntelligentPointer@VCThing@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A3E00, 275 bytes (+ int3 padding to 288), /Gr __fastcall (first in ecx, last in edx; the
//   value-type pointer, the depth limit and the 4-byte comparator on the stack), `ret 0Ch`; recurses into itself (0x006A3ED4).
// Identity: the catalog's "Heap_SiftDown" was a descriptive auto label. The caller is std::sort 0x006A3F20 (`__introsort_loop(first,
//   last, (T*)0, __lg(n) * 2, comp)`, then __final_insertion_sort 0x006A3D30). Element: 12-byte pair<long, CIntelligentPointer<CThing> >
//   (the pivot copy is the pair's member-wise copy: key word, then the pointer's base ctor 0x00A01B10, vtable 0x01238C6C, GetPItem /
//   SetPItem); comparator CKeyPairCompareLess<long, CIntelligentPointer<CThing> > (Ego_r.pdb's Dinkumware sibling instantiations
//   _Adjust_heap / _Push_heap / _Make_heap over the same pair and comparator), the signed key compare (jge / jl).
// Callees: __unguarded_partition 0x006A3440, __partial_sort 0x006A3BF0 (partial_sort's out-of-line worker, (T*)0 pushed),
//   CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90.
// Graded with FABLE_HDR_OVERLAY=$A/hdr (stlport/fable_containers.h FABLE_RETAIL_STLP_INTROSORT_LOOP).
// Flags: /Gr
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_STLP_INTROSORT_LOOP
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CVectorMap.h"

class CThing;
typedef std::pair<long, CIntelligentPointer<CThing> > CThingEntry;

template void FABLE_STLP_FASTCALL std::__introsort_loop(CThingEntry*, CThingEntry*, CThingEntry*, int,
                                                        CKeyPairCompareLess<long, CIntelligentPointer<CThing> >);
