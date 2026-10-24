// std::_Rb_tree<CCharString,pair<const CCharString,long>>::erase(iterator first, iterator last) -- STLport: [begin, end) clears the whole tree, any other range is erased node by node
// Leaf: ?erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@std@@@std@@QAEXU_Rb_tree_iterator@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B9C10, length 198 bytes, `ret 8` (thiscall: first, last).
// Callees: _M_erase (recursive subtree free, shared by many CCharString-keyed maps) @ 0x004ABA70,
//   _Rb_global<bool>::_Rebalance_for_erase @ 0x0042A34E (landed, __cdecl), ~CCharString @ 0x0099EAE0, free @ 0x00BFEA14.
// Label: the catalog "LTextTreeWalkThrough_Process" is propagated; the body is STLport 4.x
// `_Rb_tree::erase(iterator, iterator)` with clear() and erase(iterator) inlined: the header node is reached through
// the alloc-proxy pointer at +0 and the count is at +4; nodes are {colour, parent, left, right, value@+0x10} and the
// value's key is a CCharString (destroyed in place before the free). The 4-byte mapped value is not observable here
// (identical-code folded over map<CCharString, 4-byte value> instantiations); `long` stands for it.
// Shape: `erase(first++)` = the in-order successor computed inline (`_M_increment`) with the victim kept in ecx;
// clear() zeroes parent and count from one `xor esi,esi` register.
extern "C" void __cdecl free(void* block);   // 0x00BFEA14

struct CCharStringData;
class CCharString {
public:
    ~CCharString();                          // 0x0099EAE0
private:
    CCharStringData* m_pData;
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

struct _Rb_tree_node_base {
    bool                _M_color;            // +0x00
    _Rb_tree_node_base* _M_parent;           // +0x04
    _Rb_tree_node_base* _M_left;             // +0x08
    _Rb_tree_node_base* _M_right;            // +0x0c
};

template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                        // +0x10
};

template <class _Dummy> struct _Rb_global {
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
                                                            _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost);   // 0x0042A34E
};

template <class T> __forceinline void _Destroy(T* p) { p->~T(); }

struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    __forceinline _Rb_tree_iterator() {}
    __forceinline _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
    __forceinline void _M_increment()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) {
                _M_node = y;
                y = y->_M_parent;
            }
            if (_M_node->_M_right != y)
                _M_node = y;
        }
    }
    __forceinline _Rb_tree_iterator operator++(int) { _Rb_tree_iterator tmp = *this; _M_increment(); return tmp; }
    __forceinline bool operator==(const _Rb_tree_iterator& x) const { return _M_node == x._M_node; }
    __forceinline bool operator!=(const _Rb_tree_iterator& x) const { return _M_node != x._M_node; }
};

template <class K, class V> class _Rb_tree {
public:
    typedef _Rb_tree_node<V> _Node;
    typedef _Rb_tree_iterator iterator;
    _Rb_tree_node_base* _M_header;           // +0x00 _STLP_alloc_proxy::_M_data
    unsigned int        _M_node_count;       // +0x04

    void _M_erase(_Node* x);                 // 0x004ABA70
    __forceinline iterator begin() { return iterator(_M_header->_M_left); }
    __forceinline iterator end() { return iterator(_M_header); }
    __forceinline void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(static_cast<_Node*>(_M_header->_M_parent));
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    __forceinline void erase(iterator position)
    {
        _Node* y = static_cast<_Node*>(_Rb_global<bool>::_Rebalance_for_erase(position._M_node, _M_header->_M_parent,
                                                                              _M_header->_M_left, _M_header->_M_right));
        _Destroy(&y->_M_value_field);
        if (y)
            free(y);
        --_M_node_count;
    }
    void erase(iterator first, iterator last);
};

template <class K, class V> void _Rb_tree<K, V>::erase(iterator first, iterator last)
{
    if (first == begin() && last == end())
        clear();
    else
        while (first != last)
            erase(first++);
}

template class _Rb_tree<CCharString, pair<const CCharString, long> >;
}