// CNavQuadTree::SetSwitchableNodesAsBlocked(unsigned __int64 uniqueID, bool blocked) -- pass a switch's open/blocked state to every switchable leaf node registered under its unique ID (the equal range of the SwitchableNodes map)
// Leaf: ?SetSwitchableNodesAsBlocked@CNavQuadTree@@QAEX_K_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A79240, 77 bytes (+ int3 padding to 80), thiscall `ret 0Ch`.
// Identity: Ego_r.pdb `?SetSwitchableNodesAsBlocked@CNavQuadTree@@QAEX_K_N@Z`; SwitchableNodes (CVectorMap<unsigned __int64,
//   CANavQuadTreeNode*>, 16-byte entries) at PDB +0x4c -> retail +0x40 (three PDB vectors 16 -> 12; the landed
//   0x00A78DD0 UnblockAllSwitchableNodes uses the same offset). The catalog's "CNavSwitchableLeafNode_BlockRange" is
//   descriptive. Engine header CNavQuadTree.h carries the PDB offsets, so CNavQuadTree is modelled locally; the node
//   class uses engine/CNavSwitchableLeafNode.h (the generated headers carry no base relation, hence the
//   reinterpret_cast of the stored CANavQuadTreeNode*, as in 0x00A78DD0).
// Callees: CVectorMap::UpperBound 0x00A7DCC0 (landed), CVectorMap::LowerBound 0x00A7DC60, CNavSwitchableLeafNode::
//   SetAsBlocked(unsigned __int64, bool) 0x00A78800 (landed).
// Shape: UpperBound is called first (the end in edi), then LowerBound starts a plain `for` over the equal range; retail
//   keeps the 64-bit ID in ebx/ebp while the bool is re-read from its slot every iteration.
// Lever: LowerBound / UpperBound get same-TU `__declspec(noinline)` bodies under `auto_inline(off)` (their landed
//   binary searches): VC7.1 then treats the by-value ID as uncaptured and keeps it in ebx:ebp across the loop's calls
//   (the declared-only bounds make it re-read both halves and keep the bool in ebx instead, 80 v 77).
// Name (round 7, NF53): the callee binds ?SetAsBlocked@CNavSwitchableLeafNode@@QAEX_K_N@Z 0x00A78800 (Ego_r.pdb 0x00BBE6F0 / FableWin.pdb)
//   on the retail engine/CNavSwitchableLeafNode.h (FABLE_RETAIL_CNAVSWITCHABLELEAFNODE: CNavSwitchableLeafNode : CNavNavigableLeafNode :
//   CANavQuadTreeNode at offset 0), reached by a static_cast of the stored node pointer; was the derived CNavSwitchableLeafNode_Calls view. The local std::pair /
//   CKeyPairCompareLess / CVectorMap models are replaced by engine/CVectorMap.h (non-const UpperBound: _PODIUM_UPPERBOUND).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CNAVSWITCHABLELEAFNODE
#define FABLE_RETAIL_CVECTORMAP_PODIUM_UPPERBOUND
#include "stlport/fable_containers.h"
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavSwitchableLeafNode.h"

class CNavQuadTree {
public:
    unsigned char _pad00[0x40];                             // vptr, flags, map position/size, three 12-byte vectors
    CVectorMap<unsigned __int64, CANavQuadTreeNode*, CKeyPairCompareLess<unsigned __int64, CANavQuadTreeNode*> >
                  SwitchableNodes;                          // +0x40
    void SetSwitchableNodesAsBlocked(unsigned __int64 uniqueID, bool blocked);
};

// The two bounds (0x00A7DC60 / 0x00A7DCC0) are engine/CVectorMap.h's own same-TU bodies (read-only binary searches): VC7.1 then knows
// neither keeps the key's address, so the by-value ID stays in registers across the loop's calls.

void CNavQuadTree::SetSwitchableNodesAsBlocked(unsigned __int64 uniqueID, bool blocked)
{
    std::pair<unsigned __int64, CANavQuadTreeNode*>* last = SwitchableNodes.UpperBound(uniqueID);
    for (std::pair<unsigned __int64, CANavQuadTreeNode*>* it = SwitchableNodes.LowerBound(uniqueID); it != last; ++it)
        static_cast<CNavSwitchableLeafNode*>(it->second)->SetAsBlocked(uniqueID, blocked);
}