// CHitParameters::IsDrainStrike() const -- true when the hit's DamageMap holds an entry for damage attribute 6 (the drain attribute)
// Leaf: ?IsDrainStrike@CHitParameters@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00868920, 48 bytes (with int3 padding), thiscall, plain `ret`, bool in al.
// Identity (proven): CHitParameters::IsDrainStrike; evidence: the nine CHitParameters strike queries sit back to back in retail at
//   0x00868860 (0x60 bytes: two lookups, keys 0 and 1), then 0x30-byte bodies at 0x008688C0 .. 0x00868A10 with keys 4, 5, 6,
//   7, 8, 2, 9, 10; Ego_r.pdb lays the same nine out in the same order with the same sizes: IsMeleeStrike 0x0083E7D0 (0x60),
//   then IsShotStrike, IsExplosionStrike, IsDrainStrike, IsDrainHealStrike, IsGenericWillStrike, IsLightningStrike,
//   IsDivineWrathStrike, IsUnholyPowerStrike every 0x30 bytes; so this row (slot 4 of that run, key 6) is
//   IsDrainStrike. DamageMap is Ego_r.pdb CHitParameters::DamageMap, map<EDamageAttribute, float> at +0x30 (nothing
//   before it changes size in retail).
// Label: the manifest's "HasGroup" is propagated across family H028 (seven bodies that differ only in the key constant).
// Callees: _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, float>, ...>::find(const EDamageAttribute&) const
//   0x006DAB90 (out of line; result through the hidden pointer, `ret 8`).
// Shape: STLport 4.6 `map::find(k) const` forwarding to the out-of-line tree find with the key in a stack temporary; the
//   iterator is held in a local and tested against `end()` (the header node read back after the call) with an early
//   `return false` -- `return find(k) != end()` reads end() before the call, and a direct `return it != end()` zero-extends
//   the bool (`xor eax,eax`) where retail sets al alone.
enum EDamageAttribute { };                                  // enumerator names not recovered: the key is written as its value

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool        _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
    const_iterator find(const key_type& __x) const;                // 0x006DAB90 (out of line)
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;
    const_iterator end() const { return _M_t.end(); }
    const_iterator find(const _Key& __x) const { return _M_t.find(__x); }
};

} // namespace std

class CHitParameters {
public:
    unsigned char _flags_and_vectors[0x30];                 // Localised .. ShotBuildupTime (Ego_r.pdb)
    std::map<EDamageAttribute, float> DamageMap;            // +0x30
    bool IsDrainStrike() const;
};

bool CHitParameters::IsDrainStrike() const
{
    std::map<EDamageAttribute, float>::const_iterator it = DamageMap.find((EDamageAttribute)6);
    if (it == DamageMap.end())
        return false;
    return true;
}