// std::_Rb_tree<long, std::pair<long const, CANavQuadTreeNode*>, std::_Select1st<std::pair<long const, CANavQuadTreeNode*> >, std::less<long>, std::allocator<std::pair<long const, CANavQuadTreeNode*> > >::_Rb_tree(const std::less<long>&, const std::allocator<std::pair<long const, CANavQuadTreeNode*> >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@JU?$pair@$$CBJPAVCANavQuadTreeNode@@@std@@U?$_Select1st@U?$pair@$$CBJPAVCANavQuadTreeNode@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJPAVCANavQuadTreeNode@@@std@@@2@@std@@QAE@ABU?$less@J@1@ABV?$allocator@U?$pair@$$CBJPAVCANavQuadTreeNode@@@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A80770, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<long, CANavQuadTreeNode*> (node 0x18 = 0x10 + 4 + 4, signed key). Sources: (1) retail: CNavQuadTree::LoadFromFile 0x00A7AF60, the
//   reader of the landed SaveToFile's index table, fills its one local map through insert_unique 0x00A81A70 (-> _M_insert 0x00A80CE0, node
//   0x18, signed `jl/jge` key) and destroys it through this _M_erase 0x00A7E340; the ~_Rb_tree 0x00A807B0, ~map 0x00A81410 and clear
//   0x00A7F6B0 call 0x00A7E340; (2) FableWin.pdb: the map<long, CANavQuadTreeNode*> tree (_Tmap_traits<long, CANavQuadTreeNode*>) with
//   _Buynode<pair<int, CNavBlockedQuadTreeNode*> > -- LoadFromFile's make_pair(0, &BlockedNode), the mirror of SaveToFile's entry.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_00a7e340,CUnprovenValue_00a7e340,CUnprovenKeyOfValue_00a7e340,std::less<CUnprovenKey_00a7e340>,std::allocator<CUnprovenValue_00a7e340> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_00a7e340@@UCUnprovenValue_00a7e340@@UCUnprovenKeyOfValue_00a7e340@@U?$less@UCUnprovenKey_00a7e340@@@std@@V?$allocator@UCUnprovenValue_00a7e340@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_00a7e340@@@1@ABV?$allocator@UCUnprovenValue_00a7e340@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
class CANavQuadTreeNode;
#include "stlport/fable_map.h"

template std::_Rb_tree<long, std::pair<long const, CANavQuadTreeNode*>, std::_Select1st<std::pair<long const, CANavQuadTreeNode*> >, std::less<long>, std::allocator<std::pair<long const, CANavQuadTreeNode*> > >::_Rb_tree(const std::less<long>&, const std::allocator<std::pair<long const, CANavQuadTreeNode*> >&);
