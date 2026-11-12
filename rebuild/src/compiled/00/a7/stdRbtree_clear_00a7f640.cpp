// std::_Rb_tree<const CANavQuadTreeNode*, std::pair<const CANavQuadTreeNode* const, long>, std::_Select1st<std::pair<const CANavQuadTreeNode* const, long> >, std::less<const CANavQuadTreeNode*>, std::allocator<std::pair<const CANavQuadTreeNode* const, long> > >::clear() -- STLport 4.6 clear(): when non-empty, _M_erase from the root, header leftmost/rightmost back to the header, root 0, count 0
// Leaf: ?clear@?$_Rb_tree@PBVCANavQuadTreeNode@@U?$pair@QBVCANavQuadTreeNode@@J@std@@U?$_Select1st@U?$pair@QBVCANavQuadTreeNode@@J@std@@@3@U?$less@PBVCANavQuadTreeNode@@@3@V?$allocator@U?$pair@QBVCANavQuadTreeNode@@J@std@@@3@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7F640, 51 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<const CANavQuadTreeNode*, long> (node 0x18 = 0x10 + 4 + 4). Sources: (1) retail: the landed CNavQuadTree::SaveToFile 0x00A78B20
//   builds its local node index table `std::map<const CANavQuadTreeNode*, long> nodeIndexTable` (make_pair(&BlockedNode, 0); insert_unique
//   0x00A819B0) and destroys it through this _M_erase 0x00A7E300, which it binds with exactly these types; the ~_Rb_tree 0x00A80720, ~map
//   0x00A81380 and clear 0x00A7F640 call 0x00A7E300; (2) Ego_r.pdb / FableWin.pdb ?SaveToFile@CNavQuadTreeNode@@MBEXAAVCAFile@@ABV?$map@PBVCANavQuadTreeNode@@J...
//   takes this map, and FableWin.pdb has its _Tree instantiation (_Tmap_traits<const CANavQuadTreeNode*, long>).
//   Was the placeholder std::_Rb_tree<CUnprovenKey_00a7e300,CUnprovenValue_00a7e300,CUnprovenKeyOfValue_00a7e300,std::less<CUnprovenKey_00a7e300>,std::allocator<CUnprovenValue_00a7e300> >::clear (leaf ?clear@?$_Rb_tree@UCUnprovenKey_00a7e300@@UCUnprovenValue_00a7e300@@UCUnprovenKeyOfValue_00a7e300@@U?$less@UCUnprovenKey_00a7e300@@@std@@V?$allocator@UCUnprovenValue_00a7e300@@@5@@std@@QAEXXZ); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
class CANavQuadTreeNode;
#include "stlport/fable_map.h"

template void std::_Rb_tree<const CANavQuadTreeNode*, std::pair<const CANavQuadTreeNode* const, long>, std::_Select1st<std::pair<const CANavQuadTreeNode* const, long> >, std::less<const CANavQuadTreeNode*>, std::allocator<std::pair<const CANavQuadTreeNode* const, long> > >::clear();
