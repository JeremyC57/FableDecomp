// _Rb_tree<ECombatAbilityDataType, pair<const ECombatAbilityDataType, pair<CCountedPointer<CCombatAbilityData>, CCombatAbility> > >::insert_equal(const value_type& v) -- STLport multimap insertion: descend to the leaf where v belongs (equal keys go right) and hand the slot to _M_insert
// Leaf: ?insert_equal@CCombatAbilityTree
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00739B00, 58 bytes (64 with padding), thiscall `ret 8`.
// Label correction: catalog "RBTree_Find" -- it is the CTCCombat::CombatAbilities tree's insert_equal (multimap::insert):
//   nine callers in the CTCCombat ability set-up (0x0073574E .. 0x00735F56), callee its _M_insert 0x00739670 (this
//   chunk).
// Callees: _Rb_tree::_M_insert(x, y, v, w = 0) 0x00739670.
// Layout: tree {header node pointer, node count}; node key +0x10.
// Shape: STLport `insert_equal`: y = header, x = root, `while (x) { y = x; x = v.key < key(x) ? left(x) : right(x); }`
//   with v's key read once (esi); `return _M_insert(x, y, v, 0);` forwards the caller's hidden result pointer.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) throw() { return p; }

template <class T> struct CCountedPointer {     // 8 bytes: object + shared count
    T*    Object;
    long* Count;
    CCountedPointer(const CCountedPointer& o) : Object(o.Object), Count(o.Count)
    {
        if (Count)
            ++*Count;
    }
};
struct CCombatAbilityData;
struct CCombatAbility {                         // Ego_r.pdb, 8 bytes
    long RepeatTimer;
    long ActiveTimer;
};
struct SCombatAbilityPair {                     // pair<CCountedPointer<CCombatAbilityData>, CCombatAbility>
    CCountedPointer<CCombatAbilityData> first;  // +0x00
    CCombatAbility                      second; // +0x08
};
struct SCombatAbilityValue {                    // pair<const ECombatAbilityDataType, SCombatAbilityPair>, 20 bytes
    long               first;                   // +0x00
    SCombatAbilityPair second;                  // +0x04
};
template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

struct _Rb_tree_node_base {
    long                _M_color;               // +0x00
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};
struct _Rb_tree_node : _Rb_tree_node_base {     // 0x24 bytes
    SCombatAbilityValue _M_value_field;         // +0x10
};
struct _Rb_global {                             // STLport 4.6 _Rb_global<bool>
    static void __cdecl _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);   // 0x0042971D
};
struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
};

// STLport _Rb_tree<ECombatAbilityDataType, SCombatAbilityValue, _Select1st, less<>, allocator<> > (CTCCombat::CombatAbilities)
struct CCombatAbilityTree {
    typedef _Rb_tree_node*     _Link_type;
    typedef _Rb_tree_iterator  iterator;
    typedef SCombatAbilityValue value_type;
    _Rb_tree_node_base* _M_header;              // +0x00 (_STLP_alloc_proxy: the header node pointer)
    unsigned long       _M_node_count;          // +0x04
    unsigned long       _M_key_compare;         // +0x08 (empty less<>, padded)

    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }
    static _Rb_tree_node_base*& _S_left(_Rb_tree_node_base* x) { return x->_M_left; }
    static _Rb_tree_node_base*& _S_right(_Rb_tree_node_base* x) { return x->_M_right; }
    static _Rb_tree_node_base*& _S_parent(_Rb_tree_node_base* x) { return x->_M_parent; }
    static const long& _S_key(_Rb_tree_node_base* x) { return ((_Link_type)x)->_M_value_field.first; }
    static _Link_type allocate(unsigned int n) { return n != 0 ? (_Link_type)malloc(n * sizeof(_Rb_tree_node)) : 0; }
    _Link_type _M_create_node(const value_type& x)
    {
        _Link_type tmp = allocate(1);
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
    iterator _M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const value_type& v, _Rb_tree_node_base* w_ = 0);
    iterator insert_equal(const value_type& v);
};

CCombatAbilityTree::iterator CCombatAbilityTree::insert_equal(const value_type& v)
{
    _Rb_tree_node_base* y = _M_header;
    _Rb_tree_node_base* x = _M_root();
    while (x != 0) {
        y = x;
        x = (v.first < _S_key(x)) ? _S_left(x) : _S_right(x);
    }
    return _M_insert(x, y, v);
}