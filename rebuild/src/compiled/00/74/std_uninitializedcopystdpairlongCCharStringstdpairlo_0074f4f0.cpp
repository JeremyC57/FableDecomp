// std::__uninitialized_copy<std::pair<long, CCharString>*, std::pair<long, CCharString>*>(first, last, result, const __false_type&) -- STLport 4.6: copy-construct [first, last) into raw storage at result, return the advanced result
// Leaf: ??$__uninitialized_copy@PAU?$pair@JVCCharString@@@std@@PAU12@@std@@YIPAU?$pair@JVCCharString@@@0@PAU10@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074F4F0, 54 bytes, `ret 8` (first in ecx, last in edx; result + the tag on the stack). Callee: CCharString copy ctor 0x0099EC30
//   (the pair's copy: first word moved, then the string's copy ctor at +4, behind the placement new's null test).
// Identity: the out-of-line __fastcall COMDAT of the STLport template that vector<pair<long, CCharString> >::_M_insert_overflow 0x0074FCE0 calls twice
//   (0x0074FD3D head, 0x0074FD8E tail; its only callers). Descriptive-by-library: retail is STLport 4.6 (both PDBs are Dinkumware), element type
//   from the vector (FableWin.pdb / Ego_r.pdb GetRewardItems / CTCContainer::ContainerItems: pair<long, CCharString>).
// NF254 (2026-11-22, L4): was the UninitCopy_SPairLongCSecond_0099ec30_0074f4f0 stand-in (_global::, Dinkumware _Uninit_copy wording, no Leaf line).
//   Re-authored as the explicit instantiation of the stlport/fable_containers.h body (FABLE_STLP_OUTOFLINE_UNINIT_COPY), the landed 0x00AD7B50 recipe.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#include "fable_string.h"
#include "stlport/fable_containers.h"
template std::pair<long, CCharString>* __fastcall std::__uninitialized_copy<std::pair<long, CCharString>*, std::pair<long, CCharString>*>(std::pair<long, CCharString>*, std::pair<long, CCharString>*, std::pair<long, CCharString>*, const std::__false_type&);
