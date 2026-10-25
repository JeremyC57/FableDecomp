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
public:
    bool operator()(const _STL::pair<K, V>& entry, const K& key) const { return entry.first < key; }
    bool operator()(const K& key, const _STL::pair<K, V>& entry) const { return key < entry.first; }
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

// Same-TU bodies of the two bounds (0x00A7DC60 / 0x00A7DCC0, read-only binary searches): VC7.1 then knows neither keeps
// the key's address, so the by-value ID stays in registers across the loop's calls.
#pragma auto_inline(off)
template <class K, class V, class C>
__declspec(noinline) typename CVectorMap<K, V, C>::iterator CVectorMap<K, V, C>::LowerBound(const K& key)
{
    value_type* first = First;
    int count = Last - First;
    C comp;
    while (count > 0) {
        int half = count / 2;
        value_type* middle = first + half;
        if (comp(*middle, key)) {
            first = middle + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return first;
}
template <class K, class V, class C>
__declspec(noinline) typename CVectorMap<K, V, C>::iterator CVectorMap<K, V, C>::UpperBound(const K& key)
{
    value_type* first = First;
    int count = Last - First;
    C comp;
    while (count > 0) {
        int half = count / 2;
        value_type* middle = first + half;
        if (!comp(key, *middle)) {
            first = middle + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return first;
}
#pragma auto_inline(on)

void CNavQuadTree::SetSwitchableNodesAsBlocked(unsigned __int64 uniqueID, bool blocked)
{
    _STL::pair<unsigned __int64, CANavQuadTreeNode*>* last = SwitchableNodes.UpperBound(uniqueID);
    for (_STL::pair<unsigned __int64, CANavQuadTreeNode*>* it = SwitchableNodes.LowerBound(uniqueID); it != last; ++it)
        reinterpret_cast<CNavSwitchableLeafNode_Calls*>(it->second)->SetAsBlocked(uniqueID, blocked);
}