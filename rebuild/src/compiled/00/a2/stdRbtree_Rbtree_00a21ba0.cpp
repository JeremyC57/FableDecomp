// std::_Rb_tree<float, std::pair<float const, const CANavQuadTreeNode*>, std::_Select1st<std::pair<float const, const CANavQuadTreeNode*> >, std::less<float>, std::allocator<std::pair<float const, const CANavQuadTreeNode*> > >::~_Rb_tree() -- STLport 4.6 tree destructor: clear() (out-of-line _M_erase from the root, header relinked, count zeroed), then _Rb_tree_base frees the header node
// Leaf: ??1?$_Rb_tree@MU?$pair@$$CBMPBVCANavQuadTreeNode@@@std@@U?$_Select1st@U?$pair@$$CBMPBVCANavQuadTreeNode@@@std@@@2@U?$less@M@2@V?$allocator@U?$pair@$$CBMPBVCANavQuadTreeNode@@@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A21BA0, 66 bytes.
// Types (proven, agentFAM11, 2026-11-12): multimap<float, const CANavQuadTreeNode*> (node 0x18 = 0x10 + 4 + 4). Sources: (1) retail: CNavigatorAStar::CalculateNavigableWidthOfLine
//   0x00A1D6E0 fills its local cost-ordered multimap through the landed _Rb_tree<float, pair<const float, const CANavQuadTreeNode*> >::_M_insert
//   0x00A21E80, searches it with the landed __find_if<..., value_equals<float, const CANavQuadTreeNode*> > 0x00A21730, and destroys it
//   through ~multimap 0x00A220E0 -> this _M_erase 0x00A20F50 (the ~_Rb_tree 0x00A21BA0 and clear 0x00A215D0 call it too); (2) Ego_r.pdb
//   find_if<multimap<float, const CANavQuadTreeNode*>::iterator, value_equals<...> > and FableWin.pdb ??0 / ??1 / insert of
//   multimap<float, const CANavQuadTreeNode*>.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_00a20f50,CUnprovenValue_00a20f50,CUnprovenKeyOfValue_00a20f50,std::less<CUnprovenKey_00a20f50>,std::allocator<CUnprovenValue_00a20f50> >::~_Rb_tree (leaf ??1?$_Rb_tree@UCUnprovenKey_00a20f50@@UCUnprovenValue_00a20f50@@UCUnprovenKeyOfValue_00a20f50@@U?$less@UCUnprovenKey_00a20f50@@@std@@V?$allocator@UCUnprovenValue_00a20f50@@@5@@std@@QAE@XZ); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / multimap; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
class CANavQuadTreeNode;
#include "stlport/fable_map.h"

template std::_Rb_tree<float, std::pair<float const, const CANavQuadTreeNode*>, std::_Select1st<std::pair<float const, const CANavQuadTreeNode*> >, std::less<float>, std::allocator<std::pair<float const, const CANavQuadTreeNode*> > >::~_Rb_tree();
