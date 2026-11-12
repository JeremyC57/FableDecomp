// std::map<long, CANavQuadTreeNode*>::map() -- STLport 4.6 container default constructor: the member _Rb_tree built with _Compare() and allocator_type() (header node allocated, red, self-linked)
// Leaf: ??0?$map@JPAVCANavQuadTreeNode@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJPAVCANavQuadTreeNode@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A813D0, 54 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<long, CANavQuadTreeNode*> (node 0x18 = 0x10 + 4 + 4, signed key). Sources: (1) retail: CNavQuadTree::LoadFromFile 0x00A7AF60, the
//   reader of the landed SaveToFile's index table, fills its one local map through insert_unique 0x00A81A70 (-> _M_insert 0x00A80CE0, node
//   0x18, signed `jl/jge` key) and destroys it through this _M_erase 0x00A7E340; the ~_Rb_tree 0x00A807B0, ~map 0x00A81410 and clear
//   0x00A7F6B0 call 0x00A7E340; (2) FableWin.pdb: the map<long, CANavQuadTreeNode*> tree (_Tmap_traits<long, CANavQuadTreeNode*>) with
//   _Buynode<pair<int, CNavBlockedQuadTreeNode*> > -- LoadFromFile's make_pair(0, &BlockedNode), the mirror of SaveToFile's entry.
//   Was the placeholder CUnprovenContainer_00a7e340::CUnprovenContainer_00a7e340 (leaf ??0CUnprovenContainer_00a7e340@@QAE@XZ); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
class CANavQuadTreeNode;
#include "stlport/fable_map.h"

template std::map<long, CANavQuadTreeNode*>::map();
