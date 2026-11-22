// std::__find<unsigned long*, unsigned long>(first, last, value, random_access_iterator_tag) -- STLport 4.6 _algobase.c: four-way
// unrolled scan with a switch on the 0..3 remaining elements (__fastcall: first in ecx, last in edx; the value by reference and the
// empty tag on the stack, `ret 8`).
// Leaf: ??$__find@PAKK@std@@YIPAKPAK0ABKABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005348E0, 112 bytes.
// Name: STLport 4.6 _algobase.c __find (random-access overload); the landed binders 0x00531090 / 0x00531560 / 0x00533BC0
// (NUISystem::CComponent GetBoundingBox / GetDynamicSize / RemoveChild, find over vector<unsigned long>) already call it by this
// spelling. Descriptive-by-library (the PDBs carry Dinkumware); the element type is the binders' vector<unsigned long>.
// Body: the shared stlport/fable_algorithm.h FABLE_RETAIL_STLP_FIND_NOINLINE_BODY template (out of line, as retail's COMDAT).
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"

template unsigned long* FABLE_FASTCALL std::__find<unsigned long*, unsigned long>(unsigned long*, unsigned long*, const unsigned long&,
                                                                                 const std::random_access_iterator_tag&);
