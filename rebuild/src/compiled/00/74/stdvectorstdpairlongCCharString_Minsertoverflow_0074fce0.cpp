// std::vector<std::pair<long, CCharString> >::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fillLength, bool atEnd) -- STLport 4.6 growth path: allocate old + max(old, fillLength) elements, copy the head, construct the fill, copy the tail unless inserting at the end, destroy + free the old block and adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@JVCCharString@@@std@@V?$allocator@U?$pair@JVCCharString@@@std@@@2@@std@@IAEXPAU?$pair@JVCCharString@@@2@ABU32@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074FCE0, 249 bytes, thiscall `ret 14h`, /Gr TU (CompilerFlags). Callers: push_back 0x0074FE50, CGameScriptInterface::MsgOnBoastsMade
//   0x008A9AE0, CTCContainer::InsertItem 0x008ED1F0, CCreateIndicesFromItemNames::operator() 0x008ED440.
// Element: std::pair<long, CCharString> (8 bytes): FableWin.pdb / Ego_r.pdb ?GetRewardItems@CTCContainerRewardHero@@IAEXAAV?$vector@U?$pair@JVCCharString@@@std@@...
//   and CTCContainer::ContainerItems (Ego_r.pdb) are vector<pair<long, CCharString> > (J = long; the old pair<int> stand-in was wrong); the
//   landed sibling push_back 0x0074FE50 binds this protected STLport spelling (descriptive-by-library: both PDBs are Dinkumware builds).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, the out-of-line __fastcall __uninitialized_copy 0x0074F4F0 and __uninitialized_fill_n
//   0x0074F530 (FABLE_STLP_OUTOFLINE_UNINIT_COPY / _FILL_N), CCharString copy ctor 0x0099EC30 / dtor 0x0099EAE0.
// Body: the shared STLport 4.6 vector::_M_insert_overflow (stlport/fable_containers.h), explicitly instantiated (the landed 0x00CBC130 recipe).
// NF254 (2026-11-22, L4 / L5): was the FableVector_0074fce0 local model (pair<int, CCharString>, local CCharString / vector / helpers).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_STLP_DESTROY_AUX_PAIR_LONG_CCHARSTRING_SIZE_BODY
#include "fable_string.h"
#include "stlport/fable_containers.h"
template void std::vector<std::pair<long, CCharString> >::_M_insert_overflow(std::pair<long, CCharString>*, const std::pair<long, CCharString>&, const std::__false_type&, unsigned int, bool);
