// std::_Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const pair<const pair<long,long>, CEngineTextureMorph>& v, _Rb_tree_node_base* w) -- STLport node insert of the surface-composition manager's map<pair<long,long>, CEngineTextureMorph>: create the node, link it left or right of y (keeping leftmost/rightmost), rebalance and count
// Leaf: ?_M_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2F060, length 206 bytes, `ret 14h` (hidden iterator result + four arguments).
// Callers: insert_unique @ 0x00B2F130 (two sites) and the hinted insert @ 0x00B2F240 (six sites).
// Identity: same tree as _M_find 0x00B2E630 / _M_lower_bound 0x00B2E830 / insert_unique 0x00B2F130 (Ego_r.pdb:
//   map<pair<long,long>, CEngineTextureMorph>); the node is 0x2C bytes = 0x10 links + 8-byte key + the 0x14-byte retail
//   CEngineTextureMorph {OriginalTextureIndex, MorphID, vector<CCompositeBlendDesc> Blends}.
// Callees: malloc @ 0x00BFEA0E (the node allocator), vector<CCompositeBlendDesc>'s copy constructor @ 0x005BB346 (out of
//   line; its catalog label "std::vector::operator=" is propagated), _Rb_tree::_M_create_node(v) @ 0x00B2EF70 (this TU's
//   out-of-line copy; catalogued "CCreatureAction_NavToPos::Clone", propagated), _Rb_global<bool>::_Rebalance(node, root&)
//   @ 0x0042971D (__cdecl; catalogued "CMemoryAllocatorVariableSize::BalanceTree", propagated).
// Shape: STLport 4.6 _M_insert -- `y == header || (w == 0 && (x != 0 || comp(key(v), key(y))))` with pair's operator<
//   folded into `jg`/`jl`/`jl`; VC7.1 inlines the first _M_create_node (malloc + placement copy of the pair under its null
//   test, the vector copied by the out-of-line copy constructor) and calls the second one out of line; the right arm is
//   laid out first; `++_M_node_count` is a load/inc/store; the iterator goes out through the hidden pointer.
// Label: the catalog's `CCreatureAction_NavToPos_Insert` is propagated -- this is the STLport tree insert.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);  // 0x00BFEA0E
inline void* operator new(size_t, void* where) { return where; }

struct CCompositeBlendDesc;

namespace std {

template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
};

template <class T1, class T2>
inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template <class T> class allocator { };

template <class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};

template <class T, class Alloc> class vector {   // retail STLport vector, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector(const vector& x);                     // 0x005BB346 (out of line)
};

struct _Rb_tree_node_base {
    bool                _M_color;                // +0x00
    _Rb_tree_node_base* _M_parent;               // +0x04
    _Rb_tree_node_base* _M_left;                 // +0x08
    _Rb_tree_node_base* _M_right;                // +0x0C
};

template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;                        // +0x10
};

template <class Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);   // 0x0042971D
};

template <class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;               // +0x00
    size_t _M_node_count;                        // +0x04
    Compare _M_key_compare;                      // +0x08

    static const Key& _S_key(_Rb_tree_node_base* x) { return KeyOfValue()(((_Node*)x)->_M_value_field); }
    _Node* _M_create_node(const Value& x)
    {
        _Node* tmp = (_Node*)malloc(sizeof(_Node));
        new (&tmp->_M_value_field) Value(x);
        return tmp;
    }
    iterator _M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const Value& v, _Rb_tree_node_base* w_ = 0);
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const Value& v,
                                                           _Rb_tree_node_base* w_)
{
    _Node* w = (_Node*)w_;
    _Node* x = (_Node*)x_;
    _Node* y = (_Node*)y_;
    _Node* z;
    if (y == _M_header || (w == 0 && (x != 0 || _M_key_compare(KeyOfValue()(v), _S_key(y))))) {
        z = _M_create_node(v);
        y->_M_left = z;
        if (y == _M_header) {
            _M_header->_M_parent = z;
            _M_header->_M_right = z;
        }
        else if (y == _M_header->_M_left)
            _M_header->_M_left = z;
    }
    else {
        z = _M_create_node(v);
        y->_M_right = z;
        if (y == _M_header->_M_right)
            _M_header->_M_right = z;
    }
    z->_M_parent = y;
    z->_M_left = 0;
    z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}

}   // namespace std

class CEngineTextureMorph {
public:
    long OriginalTextureIndex;                                           // +0x00
    long MorphID;                                                        // +0x04
    std::vector<CCompositeBlendDesc, std::allocator<CCompositeBlendDesc> > Blends;   // +0x08
};

typedef std::pair<long, long> CMorphKey;
typedef std::pair<const CMorphKey, CEngineTextureMorph> CMorphValue;
typedef std::_Rb_tree<CMorphKey, CMorphValue, std::_Select1st<CMorphValue>, std::less<CMorphKey>,
    std::allocator<CMorphValue> > CMorphTree;

template CMorphTree::iterator CMorphTree::_M_insert(std::_Rb_tree_node_base* x_, std::_Rb_tree_node_base* y_,
                                                    const CMorphValue& v, std::_Rb_tree_node_base* w_);