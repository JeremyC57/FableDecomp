// CNavQuadTree::SetSwitchableNodesAsBlocked(unsigned __int64 uniqueID, bool blocked) -- pass a switch's open/blocked state to every switchable leaf node registered under its unique ID (the equal range of the SwitchableNodes map)
// Grade: DIFFER(80v77) -- register allocation: retail loads the 64-bit ID into ebx:ebp after the bound calls (pushes inside the loop region) and re-reads the bool each iteration; ours keeps the bool in ebx and re-reads both ID halves
// Leaf: ?SetSwitchableNodesAsBlocked@
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
// Tried: an inline EqualRange pair (same bytes as this), a named ID copy (ID in registers but loaded at entry, halves
//   swapped), a by-value key in EqualRange (extra frame copy).
// Next: give LowerBound 0x00A7DC60, UpperBound 0x00A7DCC0 and SetAsBlocked 0x00A78800 same-TU noinline bodies (their
//   landed sources): in this chunk that lever alone fixed three "key kept in registers across calls" rows (0x00A81E20,
//   0x00A82B10, 0x00A74EA0) -- VC7.1 then treats the by-value ID as uncaptured.
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavSwitchableLeafNode.h"

struct CNavSwitchableLeafNode_Calls : CNavSwitchableLeafNode {
    void SetAsBlocked(unsigned __int64 uniqueID, bool blocked);  // 0x00A78800
};

namespace _STL {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
};

template <class K, class V, class C> class CVectorMap {
public:
    typedef _STL::pair<K, V> value_type;
    typedef value_type* iterator;
    value_type* First;                                      // +0x00
    value_type* Last;                                       // +0x04
    value_type* EndOfStorage;                               // +0x08
    C           Compare;                                    // +0x0c
    bool        Dirty;                                      // +0x0d
    iterator LowerBound(const K& key);                      // 0x00A7DC60
    iterator UpperBound(const K& key);                      // 0x00A7DCC0
};

class CNavQuadTree {
public:
    unsigned char _pad00[0x40];                             // vptr, flags, map position/size, three 12-byte vectors
    CVectorMap<unsigned __int64, CANavQuadTreeNode*, CKeyPairCompareLess<unsigned __int64, CANavQuadTreeNode*> >
                  SwitchableNodes;                          // +0x40
    void SetSwitchableNodesAsBlocked(unsigned __int64 uniqueID, bool blocked);
};

void CNavQuadTree::SetSwitchableNodesAsBlocked(unsigned __int64 uniqueID, bool blocked)
{
    _STL::pair<unsigned __int64, CANavQuadTreeNode*>* last = SwitchableNodes.UpperBound(uniqueID);
    for (_STL::pair<unsigned __int64, CANavQuadTreeNode*>* it = SwitchableNodes.LowerBound(uniqueID); it != last; ++it)
        reinterpret_cast<CNavSwitchableLeafNode_Calls*>(it->second)->SetAsBlocked(uniqueID, blocked);
}
