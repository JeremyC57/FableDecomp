// std::__pop_heap_aux<std::pair<CCharString,CSoundBankLH*>*, CKeyPairCompareLess<CCharString,CSoundBankLH*>> (STLport 4.6, element std::pair<CCharString,CSoundBankLH*>) -- SGI pop_heap front end for a heap of (name, unsigned long) pairs: move the back element out, put the root there and sift the old back element down
// Flags: /Gr
// Leaf: ??$__pop_heap_aux@PAU?$pair@VCCharString@@PAVCSoundBankLH@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@PAVCSoundBankLH@@@@@std@@YIXPAU?$pair@VCCharString@@PAVCSoundBankLH@@@0@00V?$CKeyPairCompareLess@VCCharString@@PAVCSoundBankLH@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A41260, length 109 bytes, `ret 8` (the unused T* and the empty comparator on the
// NF85: moved off the row-local CBasicString / CCharString / element / comparator models onto the shared headers:
//   stlport/fable_containers.h (the STLport 4.6 __pop_heap_aux body, explicit instantiation; generator work/gen/algo.py), fable_string.h
//   (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY) and the element's own header; `// Flags: /Gr` (the
//   algorithm TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,CSoundBankLH*>, comparator CKeyPairCompareLess<CCharString,CSoundBankLH*>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_POP_HEAP_AUX_FASTCALL_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
class CSoundBankLH;   // RTTI .?AVCSoundBankLH@@ (no header: declared only)
#include "engine/CVectorMap.h"
template void __fastcall std::__pop_heap_aux(std::pair<CCharString,CSoundBankLH*>*,std::pair<CCharString,CSoundBankLH*>*,std::pair<CCharString,CSoundBankLH*>*,CKeyPairCompareLess<CCharString,CSoundBankLH*>);
