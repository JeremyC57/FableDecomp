// std::multimap<float, const CANavQuadTreeNode*>::~multimap() -- STLport 4.6 container destructor (implicit): the member _Rb_tree's destructor inlined -- clear(), then free the header node
// Leaf: ??1?$multimap@MPBVCANavQuadTreeNode@@U?$less@M@std@@V?$allocator@U?$pair@$$CBMPBVCANavQuadTreeNode@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A220E0, 66 bytes.
// Types (proven, agentFAM11, 2026-11-12): multimap<float, const CANavQuadTreeNode*> (node 0x18 = 0x10 + 4 + 4). Sources: (1) retail: CNavigatorAStar::CalculateNavigableWidthOfLine
//   0x00A1D6E0 fills its local cost-ordered multimap through the landed _Rb_tree<float, pair<const float, const CANavQuadTreeNode*> >::_M_insert
//   0x00A21E80, searches it with the landed __find_if<..., value_equals<float, const CANavQuadTreeNode*> > 0x00A21730, and destroys it
//   through ~multimap 0x00A220E0 -> this _M_erase 0x00A20F50 (the ~_Rb_tree 0x00A21BA0 and clear 0x00A215D0 call it too); (2) Ego_r.pdb
//   find_if<multimap<float, const CANavQuadTreeNode*>::iterator, value_equals<...> > and FableWin.pdb ??0 / ??1 / insert of
//   multimap<float, const CANavQuadTreeNode*>.
//   Was the placeholder CUnprovenContainer_00a20f50::~CUnprovenContainer_00a20f50 (leaf ??1CUnprovenContainer_00a20f50@@QAE@XZ); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / multimap; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
class CANavQuadTreeNode;
#include "stlport/fable_map.h"

// The implicit ~multimap (STLport declares none) inlines ~_Rb_tree: clear(), then free the header node. Retail emitted it out
//   of line; this emitter (not part of the graded body) makes the TU emit that COMDAT.
void DestroyMultimapOfFloatCANavQuadTreeNode(std::multimap<float, const CANavQuadTreeNode*>* container) { container->~multimap(); }
