// std::_Rb_tree<EHeroAbility, pair<const EHeroAbility, SExpSpendUndo> >::erase(iterator pos) -- STLport map erase of one node: unlink and rebalance, destroy the undo record, free the node
// Leaf: ?erase@?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00579D55, 61 bytes, thiscall `ret 4` (the iterator by value). The
//   CTCInventoryExperience::UndoableAbilityBuys map (Ego_r.pdb: map<EHeroAbility, SExpSpendUndo>), erased from
//   CTCInventoryExperience::UndoBuyAbility (0x00576C4F). The catalog label is descriptive.
// Callees: _Rb_global<bool>::_Rebalance_for_erase @ 0x0042A34E (static cdecl: node, root&, leftmost&, rightmost&),
//   pair<const EHeroAbility, SExpSpendUndo>::`scalar deleting destructor' @ 0x005799FD (given 0: the value's
//   implicit destructor; it runs the out-of-line ~SExpSpendUndo 0x005797FA on +4), free @ 0x00BFEA14.
// Shape: STLport 4.x verbatim: `_Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(
//   __position._M_node, _M_header._M_data->_M_parent, _M_header._M_data->_M_left, _M_header._M_data->_M_right);
//   _Destroy(&__y->_M_value_field); _M_header.deallocate(__y, 1); --_M_node_count;` -- an explicit destructor call
//   on a pair whose destructor is IMPLICIT and non-trivial goes through its `??_G` with 0 (the pair has no `??1`).
// Layout: node colour +0, parent +4, left +8, right +0xC, key +0x10, SExpSpendUndo +0x14.
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14

enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
class SExpSpendUndo {                               // Ego_r.pdb: four list<long> + nine longs (retail lists 12 bytes)
public:
    unsigned char Lists[0x30];                      // Generals, Strengths, Skills, Wills (+0x00..+0x24)
    long Levels;                                    // +0x30
    long General, Strength, Skill, Will;            // +0x34..+0x40
    long aGeneral, aStrength, aSkill, aWill;        // +0x44..+0x50
    ~SExpSpendUndo();                               // 0x005797FA
};

namespace std {

template <class _T1, class _T2> struct pair {
    _T1 first;                                      // +0x00
    _T2 second;                                     // +0x04
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _Rb_tree_node_base {
    bool                _M_color;                   // +0x00
    _Rb_tree_node_base* _M_parent;                  // +0x04
    _Rb_tree_node_base* _M_left;                    // +0x08
    _Rb_tree_node_base* _M_right;                   // +0x0c
};

template <class _Dummy> struct _Rb_global {
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                                          _Rb_tree_node_base*& __leftmost,
                                                          _Rb_tree_node_base*& __rightmost);   // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                          // +0x10
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
};

template <class _Key, class _Value> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value>, allocator<_Rb_tree_node<_Value> > > _M_header;   // +0x00
    unsigned int _M_node_count;                                                                        // +0x04
    unsigned int _M_key_compare;                                                                       // +0x08

    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
                                                                          this->_M_header._M_data->_M_parent,
                                                                          this->_M_header._M_data->_M_left,
                                                                          this->_M_header._M_data->_M_right);
        _Destroy(&__y->_M_value_field);
        this->_M_header.deallocate(__y, 1);
        --_M_node_count;
    }
};

// map::erase(iterator) is the inline `_M_t.erase(__position)`: the emitted body is the tree's.
template class _Rb_tree<EHeroAbility, pair<const EHeroAbility, SExpSpendUndo> >;

}