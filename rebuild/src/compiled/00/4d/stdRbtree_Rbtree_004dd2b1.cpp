// std::_Rb_tree<long, std::pair<long const, long>, std::_Select1st<std::pair<long const, long> >, std::less<long>,
//   std::allocator<std::pair<long const, long> > >::~_Rb_tree() -- STLport 4.6 _tree.h destructor: clear() (0x004DB91F, out of line),
//   then the _Rb_tree_base destructor frees the header node
// Leaf: ??1?$_Rb_tree@JU?$pair@$$CBJJ@std@@U?$_Select1st@U?$pair@$$CBJJ@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DD2B1, 23 bytes, size TU (`pop ecx`).
// NF64 (round 7, q10672): an explicit instantiation of stlport/fable_map.h's ~_Rb_tree for the map<long,long> tree whose clear 0x004DB91F
//   the landed 0x004E52F0 binds under the same tree spelling. The landed caller 0x004DF885 and NF61's 0x005907AB bind this destructor.
//   Was the stand-in `std::list_T_004dd2b1::~list_T_004dd2b1` (the propagated label claimed a list; the body is the tree's clear + header
//   free). Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("s", on)
#include "stlport/fable_map.h"

template std::_Rb_tree<long, std::pair<long const, long>, std::_Select1st<std::pair<long const, long> >, std::less<long>, std::allocator<std::pair<long const, long> > >::~_Rb_tree();
