// std::_Rb_tree<CPendingObjectSortKey>::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport node insert for a device-queue map: allocate a node, copy the pair in, link it left or right of y (keeping leftmost/rightmost), rebalance and count
// Leaf: ?_M_insert@?$_Rb_tree@VCPendingObjectSortKey@CThreadedFileManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00990830, length 208 bytes (201 + int3 padding), `ret 14h` (hidden iterator
// result + four arguments).
// Callees: less<CThreadedFileManager::CPendingObjectSortKey>::operator() const @ 0x0098F570 (thiscall on the tree's
// _M_key_compare +0x08, not inlined), malloc @ 0x00BFEA0E (the 0x18-byte node: 0x10 links + the 8-byte pair),
// _Rb_global<bool>::_Rebalance(node, root&) @ 0x0042971D (__cdecl; the manifest's "CMemoryAllocatorVariableSize::
// BalanceTree" is propagated; landed as Rbglobalbool_Rebalance_0042971d).
// Callers: nine sites @ 0x009909D8 .. 0x00990FE2 (the CPendingObjectMap inserts of CThreadedFileManager's device queues).
// Identity: FableWin.pdb CThreadedFileManager::CPendingObjectMap::Objects = map<CPendingObjectSortKey, CFileObject*>;
// CPendingObjectSortKey is 4 bytes {CFileObject* Object}; the compare at 0x0098F570 orders by the file's physical sort
// key, then the file position, then the object address. The manifest's "RBTree_Insert" is descriptive.
// Shape: STLport 4.6 _M_insert -- `y == header || (w == 0 && (x != 0 || comp(key(v), key(y))))`; _M_create_node
// (malloc + placement-new copy of the trivially copyable pair under its null test) duplicated in both arms; the iterator
// returned through the hidden pointer.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);  // 0x00BFEA0E
inline void* operator new(size_t, void* where) { return where; }

class CThreadedFileManager {
public:
    class CFileObject;
    class CPendingObjectSortKey {
    public:
        CFileObject* Object;                    // +0x00
    };
};

namespace std {

template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
};

template <class T> struct less {
    bool operator()(const T& a, const T& b) const;   // 0x0098F570 for CPendingObjectSortKey
};

template <class T> class allocator { };

template <class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};

template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;                       // +0x10
};

struct _Rb_global {
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
    _Rb_tree_node_base* _M_header;              // +0x00
    size_t _M_node_count;                       // +0x04
    Compare _M_key_compare;                     // +0x08

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
    _Rb_global::_Rebalance(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}

}   // namespace std

typedef std::pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> CPendingObjectValue;

template class std::_Rb_tree<CThreadedFileManager::CPendingObjectSortKey, CPendingObjectValue,
    std::_Select1st<CPendingObjectValue>, std::less<CThreadedFileManager::CPendingObjectSortKey>,
    std::allocator<CPendingObjectValue> >;