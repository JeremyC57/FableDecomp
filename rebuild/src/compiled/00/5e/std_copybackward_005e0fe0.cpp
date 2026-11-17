// std::copy_backward<CCharString*, std::less<CCharString>> (STLport 4.6, element CCharString) -- STLport copy_backward of a CCharString range (the inlined random-access __copy_backward), returns the moved-back result
// Flags: /Gr
// Leaf: ??$copy_backward@PAVCCharString@@PAV1@@std@@YIPAVCCharString@@PAV1@00@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0FE0, 49 bytes (+ padding to 64), `ret 4` (__fastcall /Gr algorithm TU: first in ecx,
// NF85: moved off the row-local CBasicString / CCharString / element / comparator models onto the shared headers:
//   stlport/fable_containers.h (the STLport 4.6 copy_backward body, explicit instantiation; generator work/gen/algo.py), fable_string.h
//   (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY) and the element's own header; `// Flags: /Gr` (the
//   algorithm TU; the old row spelled __fastcall by hand). Element CCharString, comparator std::less<CCharString>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_COPY_BACKWARD_AUX_INLINE
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#define FABLE_RETAIL_STLP_SORT_COPY_BACKWARD_NOINLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CVectorMap.h"
template CCharString* __fastcall std::copy_backward(CCharString*,CCharString*,CCharString*);
