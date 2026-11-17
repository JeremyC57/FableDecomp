// std::__make_heap<CCharString*, std::less<CCharString>> (STLport 4.6, element CCharString) -- STLport make_heap core: sift every parent of a CCharString range down, last parent first
// Flags: /Gr
// Leaf: ??$__make_heap@PAVCCharString@@U?$less@VCCharString@@@std@@V1@H@std@@YIXPAVCCharString@@0U?$less@VCCharString@@@0@0PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E1BE0, 73 bytes (+ padding to 80), `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
// NF85: moved off the row-local CBasicString / CCharString / element / comparator models onto the shared headers:
//   stlport/fable_containers.h (the STLport 4.6 __make_heap body, explicit instantiation; generator work/gen/algo.py), fable_string.h
//   (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY) and the element's own header; `// Flags: /Gr` (the
//   algorithm TU; the old row spelled __fastcall by hand). Element CCharString, comparator std::less<CCharString>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_MAKE_HEAP_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__make_heap(CCharString*,CCharString*,std::less<CCharString>,CCharString*,int*);
