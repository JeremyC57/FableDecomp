// _Rb_tree<ECombatAbilityDataType, pair<const ECombatAbilityDataType, pair<CCountedPointer<CCombatAbilityData>, CCombatAbility> > >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport tree node insertion under parent y (left when y is the header, w is null and x is set or v's key sorts before y's), maintaining leftmost/rightmost, then rebalance and count
// Leaf: ?_M_insert@CCombatAbilityTree
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00739670, 235 bytes (240 with padding), thiscall `ret 14h`.
// Label correction: catalog "CMemoryAllocatorVariableSize_InsertNode" -- the STLport tree _M_insert of
//   CTCCombat::CombatAbilities (multimap<ECombatAbilityDataType, pair<CCountedPointer<CCombatAbilityData>,
//   CCombatAbility> >, 0x24-byte nodes); callers its insert_equal 0x00739B00 (this chunk) and 0x007399BE.
// Callees: malloc 0x00BFEA0E (node allocate(1)), _Rb_global<bool>::_Rebalance(node, root&) 0x0042971D (__cdecl).
// Layout: node {colour, parent +4, left +8, right +0xc, value +0x10}; value {key, CCountedPointer {Object, Count*},
//   CCombatAbility {RepeatTimer, ActiveTimer}}; tree {header node pointer, node count}.
// Shape: STLport 4.6 _tree.c text: `_M_create_node` inlined in both arms (allocate(1), the placement `_Construct`
//   with its null test, the memberwise value copy with the counted pointer's `if (Count) ++*Count`); the iterator
//   (user constructor) is returned through the hidden pointer.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) throw() { return p; }

template <class T> class CCountedPointer { public:     // 8 bytes: object + shared count
    T*    Object;
    long* Count;
    CCountedPointer(const CCountedPointer& o) : Object(o.Object), Count(o.Count)
    {
        if (Count)
            ++*Count;
    }
};
class CCombatAbilityData;
class CCombatAbility { public:                         // Ego_r.pdb, 8 bytes
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

CCombatAbilityTree::iterator CCombatAbilityTree::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const value_type& v, _Rb_tree_node_base* w_)
{
    _Link_type w = (_Link_type)w_;
    _Link_type x = (_Link_type)x_;
    _Link_type y = (_Link_type)y_;
    _Link_type z;

    if (y == _M_header || (w == 0 && (x != 0 || v.first < _S_key(y)))) {
        z = _M_create_node(v);
        _S_left(y) = z;                         // also makes _M_leftmost() = z when y == _M_header
        if (y == _M_header) {
            _M_root() = z;
            _M_rightmost() = z;
        }
        else if (y == _M_leftmost())
            _M_leftmost() = z;                  // maintain _M_leftmost() pointing to min node
    }
    else {
        z = _M_create_node(v);
        _S_right(y) = z;
        if (y == _M_rightmost())
            _M_rightmost() = z;                 // maintain _M_rightmost() pointing to max node
    }
    _S_parent(z) = y;
    _S_left(z) = 0;
    _S_right(z) = 0;
    _Rb_global::_Rebalance(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}