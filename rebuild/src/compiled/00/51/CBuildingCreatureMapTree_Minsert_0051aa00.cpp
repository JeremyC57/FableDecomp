// CBuildingCreatureMapTree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport _Rb_tree::_M_insert of map<CThingBuilding*, list<CThingAICreature*> >: hang a new node off y (left or right), fix the header links, rebalance, count it
// Leaf: ?_M_insert
//
// Retail 0x0051AA00, 180 bytes (+ int3 padding to 192), thiscall `ret 14h` (hidden iterator result, x, y, v, w).  Callees: 0x00BFEA0E malloc
//   (0x18-byte node), 0x00517590 pair<CThingBuilding* const, list<CThingAICreature*> > copy constructor (this chunk),
//   0x0042971D _Rb_global<bool>::_Rebalance(node, root&) (__cdecl).
// Identity: see 0x00517590 / 0x00519970 -- pointer key (`jb`), 0x18-byte nodes with a list at +0x14.
// Shape: STLport _tree.c -- `if (y == header || (w == 0 && (x != 0 || comp(KeyOfValue()(v), _S_key(y))))) { z =
//   _M_create_node(v); _S_left(y) = z; if (y == header) { _M_root() = z; _M_rightmost() = z; } else if (y ==
//   _M_leftmost()) _M_leftmost() = z; } else { z = _M_create_node(v); _S_right(y) = z; if (y == _M_rightmost())
//   _M_rightmost() = z; } _S_parent(z) = y; _S_left(z) = 0; _S_right(z) = 0; _Rebalance(z, root); ++_M_node_count;
//   return iterator(z);` with _M_create_node inline twice (malloc + placement copy with its null test on node+0x10); the
//   pair's copy constructor is only declared here, so it stays a call (as retail).
// Label: catalog "ListNode_InsertWithClone" (propagated).
extern "C" void* __cdecl malloc(unsigned int size);
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class CThingBuilding;
class CThingAICreature;

struct _Rb_tree_node_base {
    char                _M_color;              // +0x0
    _Rb_tree_node_base* _M_parent;             // +0x4
    _Rb_tree_node_base* _M_left;               // +0x8
    _Rb_tree_node_base* _M_right;              // +0xC
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                          // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    static void __cdecl _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);   // 0x0042971D
};

namespace std {
struct _List_node_base;
template <class T> class list {
public:
    _List_node_base* _M_node;                  // sentinel (alloc proxy data)
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o);               // 0x00517590 (out of line)
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
}

class CBuildingCreatureMapTree {               // STLport _Rb_tree<CThingBuilding*, pair<CThingBuilding* const, list<CThingAICreature*> >, ...>
public:
    typedef std::pair<CThingBuilding* const, std::list<CThingAICreature*> > value_type;
    typedef _Rb_tree_node<value_type> _Node;
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* x) : _M_node(x) {}
    };
    _Node*                          _M_header;       // +0x0
    unsigned long                   _M_node_count;   // +0x4
    std::less<CThingBuilding*>      _M_key_compare;  // +0x8

    static _Node* allocate(unsigned int n) { return (_Node*)malloc(n * sizeof(_Node)); }
    _Node* _M_create_node(const value_type& x)
    {
        _Node* tmp = allocate(1);
        std::_Construct(&tmp->_M_value_field, x);
        return tmp;
    }
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }
    static CThingBuilding* const& _S_key(_Rb_tree_node_base* x) { return ((_Node*)x)->_M_value_field.first; }

    iterator _M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const value_type& v, _Rb_tree_node_base* w_);
};

CBuildingCreatureMapTree::iterator CBuildingCreatureMapTree::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_,
                                                                       const value_type& v, _Rb_tree_node_base* w_)
{
    _Node* w = (_Node*)w_;
    _Node* x = (_Node*)x_;
    _Node* y = (_Node*)y_;
    _Node* z;

    if (y == _M_header || (w == 0 && (x != 0 || _M_key_compare(v.first, _S_key(y))))) {
        z = _M_create_node(v);
        y->_M_left = z;
        if (y == _M_header) {
            _M_root() = z;
            _M_rightmost() = z;
        }
        else if (y == _M_leftmost())
            _M_leftmost() = z;
    }
    else {
        z = _M_create_node(v);
        y->_M_right = z;
        if (y == _M_rightmost())
            _M_rightmost() = z;
    }
    z->_M_parent = y;
    z->_M_left = 0;
    z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}