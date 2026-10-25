// CTCCombat::GetCombatAbilityData(const_iterator& it, ECombatAbilityInterfaceType) const -- the const overload: run the non-const lookup on a null-started
// iterator and, on success, hand the found node back through the caller's const_iterator
// Leaf: ?GetCombatAbilityData@CTCCombat@@ABE_N
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733750, length 49 bytes (+ int3 padding), `ret 8`.
// Identity (FINDINGS chunk 391: 0x00733620 is the non-const CTCCombat::GetCombatAbilityData(iterator&, ECombatAbilityInterfaceType); landed here as
//   CTCCombat::GetBlockAbilityIterator(CBlockAbilityIterator*, long) over a "block-ability finder"): Ego_r.pdb has exactly two overloads, `?GetCombatAbilityData@CTCCombat@@AAE_NAAV?$_Tree_iterator...` (private) and
//   `?GetCombatAbilityData@CTCCombat@@ABE_NAAV?$_Tree_const_iterator...` (private const); this body is the const one -- it
//   forwards its second argument unchanged to 0x00733620 (so the argument is the same ECombatAbilityInterfaceType), passes a local
//   iterator by reference and copies the node into the caller's iterator, the usual const -> non-const forwarder
//   (an STLport iterator converts to a const_iterator, never back). Callers: CTCCombat::SetStruck 0x00734C60 (0x00734CE6).
// Shape: the one-word iterator local lives in the `push ecx` frame slot and is constructed from a null node (the
//   store scheduled after the pushes); `if (lookup(found, type)) { it = found; return true; } return false;` with
//   the found node re-read from the frame.
// Model: the iterators are STLport's one-pointer _Rb_tree_iterator over CTCCombat::CombatAbilities; no member is
//   touched, so the class is declared locally (engine/CTCCombat.h is the flattened PDB record with Dinkumware sizes).
struct _Rb_tree_node_base;

struct CCombatAbilityIterator {                 // multimap<ECombatAbilityDataType, pair<...> >::iterator
    _Rb_tree_node_base* _M_node;
    CCombatAbilityIterator() {}
    CCombatAbilityIterator(_Rb_tree_node_base* __x) { _M_node = __x; }
};

struct CCombatAbilityConstIterator {            // ...::const_iterator (constructible from an iterator)
    _Rb_tree_node_base* _M_node;
    CCombatAbilityConstIterator() {}
    CCombatAbilityConstIterator(const CCombatAbilityIterator& __it) { _M_node = __it._M_node; }
};

enum ECombatAbilityInterfaceType { ECombatAbilityInterfaceType_0 };

class CTCCombat {
    bool GetCombatAbilityData(CCombatAbilityIterator& it, ECombatAbilityInterfaceType type);                 // 0x00733620
    bool GetCombatAbilityData(CCombatAbilityConstIterator& it, ECombatAbilityInterfaceType type) const;
};

bool CTCCombat::GetCombatAbilityData(CCombatAbilityConstIterator& it, ECombatAbilityInterfaceType type) const
{
    CCombatAbilityIterator found(0);
    if (const_cast<CTCCombat*>(this)->GetCombatAbilityData(found, type)) {
        it = found;
        return true;
    }
    return false;
}
