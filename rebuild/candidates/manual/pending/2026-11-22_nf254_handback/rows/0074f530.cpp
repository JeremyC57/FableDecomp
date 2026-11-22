// std::__uninitialized_fill_n<std::pair<long, CCharString>*, unsigned int, std::pair<long, CCharString> >(first, n, x, const __false_type&) -- STLport 4.6: copy-construct n copies of x into raw storage at first, return the advanced pointer
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@JVCCharString@@@std@@IU12@@std@@YIPAU?$pair@JVCCharString@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074F530, 50 bytes, `ret 8` (first in ecx, n in edx; x + the tag on the stack). Callee: CCharString copy ctor 0x0099EC30
//   (the pair's copy: first word moved, then the string's copy ctor at +4, behind the placement new's null test).
// Identity: the out-of-line __fastcall COMDAT of the STLport template that vector<pair<long, CCharString> >::_M_insert_overflow 0x0074FCE0 calls for a
//   fill longer than one (0x0074FD74; its only caller). Descriptive-by-library: retail is STLport 4.6 (both PDBs are Dinkumware), element type from the
//   vector (FableWin.pdb / Ego_r.pdb GetRewardItems / CTCContainer::ContainerItems: pair<long, CCharString>).
// NF254 (2026-11-22, L5): was the UninitFillN_SPairLongCSecond_0099ec30_0074f530 stand-in (_global::, Dinkumware _Uninit_fill_n wording, no Leaf line).
//   Re-authored as the explicit instantiation of the stlport/fable_containers.h body (FABLE_STLP_OUTOFLINE_UNINIT_FILL_N), as 0x0074FCE0 instantiates it.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#include "fable_string.h"
#include "stlport/fable_containers.h"
template std::pair<long, CCharString>* __fastcall std::__uninitialized_fill_n<std::pair<long, CCharString>*, unsigned int, std::pair<long, CCharString> >(std::pair<long, CCharString>*, unsigned int, const std::pair<long, CCharString>&, const std::__false_type&);
