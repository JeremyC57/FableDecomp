// CTCPhysicalShieldSpell::AllowReactionsOnHitFromAttacker(CThing* attacker, bool allow) -- add the attacker to (or remove it from) the set of attackers whose hits still cause a reaction through the shield
// Leaf: ?AllowReactionsOnHitFromAttacker@CTCPhysicalShieldSpell@@QAEXPAVCThing@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079E580, 106 bytes (+ int3 padding to 112), __fastcall `ret 8`.
// Identity: Ego_r.pdb `?AllowReactionsOnHitFromAttacker@CTCPhysicalShieldSpell@@QAEXPAVCThing@@_N@Z` (CThing*, bool); the
//   nine callers (creature actions 0x008C431E.., 0x0097C073) fetch interface 0x93 and pass a GetPItem() thing and a bool;
//   the body sits in the CTCPhysicalShieldSpell TU (0x0079DCC0 OnHitAtPos .. 0x0079EC80 FinalCleanup, landed). The
//   catalog's "CCreatureAction_SetDirection" is propagated.
// Callees: STLport `_Rb_tree::insert_unique(const value_type&)` 0x0079F330 (pair<iterator, bool> through a hidden pointer),
//   `_Rb_tree::find(const key_type&)` 0x0079F210 (iterator through a hidden pointer), `_Rb_global<bool>::
//   _Rebalance_for_erase` 0x0042A34E (__cdecl), free 0x00BFEA14.
// Layout: Ego_r.pdb CTCPhysicalShieldSpell::AllowReactionAttackers set<CThing*> +0x48 (retail STLport: header pointer, node
//   count, empty comparator).
// Shape: `if (allow) AllowReactionAttackers.insert(attacker); else { it = find(attacker); if (it != end()) erase(it); }` --
//   the key is passed by reference from its own argument slot, the insert result lands in an 8-byte frame temporary and is
//   dropped, the find result is homed in the dead `allow` slot, and STLport's `erase(iterator)` is inlined (rebalance with
//   the header's parent/left/right addresses, `if (p) free(p)`, `--_M_node_count`). The pair and the iterator need their
//   STLport constructors: as PODs VC7.1 returns them in edx:eax / eax instead of through the hidden pointer.
extern "C" void __cdecl free(void* block);               // 0x00BFEA14

class CThing;

struct _Rb_tree_node_base {
    long                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

struct _Rb_global {
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
                                                            _Rb_tree_node_base*& leftmost,
                                                            _Rb_tree_node_base*& rightmost);   // 0x0042A34E
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> inline void _Destroy(T*) { }

struct CThingSet {                                       // STLport set<CThing*> (12 bytes)
    typedef CThing* value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;                       // +0x10
    };
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator() { }
        iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned long       _M_key_compare;

    pair<iterator, bool> insert_unique(const value_type& v);   // 0x0079F330
    iterator find(const value_type& k);                        // 0x0079F210
    iterator end() { return iterator(_M_header); }
    pair<iterator, bool> insert(const value_type& v) { return insert_unique(v); }
    void erase(iterator position)
    {
        _Node* y = static_cast<_Node*>(_Rb_global::_Rebalance_for_erase(position._M_node, _M_header->_M_parent,
                                                                         _M_header->_M_left, _M_header->_M_right));
        _Destroy(&y->_M_value_field);
        if (y)
            free(y);
        --_M_node_count;
    }
};

class CTCPhysicalShieldSpell {
public:
    unsigned char _pad00[0x48];
    CThingSet     AllowReactionAttackers;                // +0x48

    void AllowReactionsOnHitFromAttacker(CThing* attacker, bool allow);
};

void CTCPhysicalShieldSpell::AllowReactionsOnHitFromAttacker(CThing* attacker, bool allow)
{
    if (allow)
        AllowReactionAttackers.insert(attacker);
    else {
        CThingSet::iterator it = AllowReactionAttackers.find(attacker);
        if (it != AllowReactionAttackers.end())
            AllowReactionAttackers.erase(it);
    }
}