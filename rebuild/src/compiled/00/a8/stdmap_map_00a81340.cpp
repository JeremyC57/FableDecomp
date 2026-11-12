// std::map<const CANavQuadTreeNode*, long>::map() -- STLport 4.6 container default constructor: the member _Rb_tree built with _Compare() and allocator_type() (header node allocated, red, self-linked)
// Leaf: ??0?$map@PBVCANavQuadTreeNode@@JU?$less@PBVCANavQuadTreeNode@@@std@@V?$allocator@U?$pair@QBVCANavQuadTreeNode@@J@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A81340, 54 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<const CANavQuadTreeNode*, long> (node 0x18 = 0x10 + 4 + 4). Sources: (1) retail: the landed CNavQuadTree::SaveToFile 0x00A78B20
//   builds its local node index table `std::map<const CANavQuadTreeNode*, long> nodeIndexTable` (make_pair(&BlockedNode, 0); insert_unique
//   0x00A819B0) and destroys it through this _M_erase 0x00A7E300, which it binds with exactly these types; the ~_Rb_tree 0x00A80720, ~map
//   0x00A81380 and clear 0x00A7F640 call 0x00A7E300; (2) Ego_r.pdb / FableWin.pdb ?SaveToFile@CNavQuadTreeNode@@MBEXAAVCAFile@@ABV?$map@PBVCANavQuadTreeNode@@J...
//   takes this map, and FableWin.pdb has its _Tree instantiation (_Tmap_traits<const CANavQuadTreeNode*, long>).
//   Was the placeholder CUnprovenContainer_00a7e300::CUnprovenContainer_00a7e300 (leaf ??0CUnprovenContainer_00a7e300@@QAE@XZ); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
class CANavQuadTreeNode;
#include "stlport/fable_map.h"

template std::map<const CANavQuadTreeNode*, long>::map();
