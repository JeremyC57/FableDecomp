// std::_Rb_tree<long, pair<const long, list<long> >, ...>::erase(iterator first, iterator last) -- STLport map erase(first, last): clear() when the range is the whole tree, else erase(first++) node by node (each node's list destroyed and freed)
// Leaf: ?erase@?$_Rb_tree@JU?$pair@$$CBJV?$list@JV?$allocator@J@std@@@std@@@std@@U?$_Select1st@U?$pair@$$CBJV?$list@JV?$allocator@J@std@@@std@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJV?$list@JV?$allocator@J@std@@@std@@@std@@@2@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBJV?$list@JV?$allocator@J@std@@@std@@@std@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051AF80, 288 bytes, thiscall `ret 8` (two 4-byte iterators by value). Callers:
//   0x005034F6 (a local map in the CWorldMap level-loading TU) and 0x0051B8C4 (map::erase(const key&): equal_range,
//   distance, erase(first, last)). The catalog label "TreeNode_EraseRange" is descriptive. Key and list element types are
//   descriptive (4-byte key, trivially destructible list element): only the node layout is evidenced.
// Callees: _M_erase(root) @ 0x00519970 (the recursive clear), _Rb_global<bool>::_Rebalance_for_erase @ 0x0042A34E
//   (__cdecl, the header's parent/left/right links by reference), free @ 0x00BFEA14 (list nodes, the list header, the
//   tree node).
// Layout: node {colour, parent +4, left +8, right +0xC, pair<const long, list<long>> +0x10}: key +0x10, the list's header
//   node pointer +0x14 (STLport list = 4 bytes).
// Shape: STLport 4.6 `erase(first, last)` verbatim: `first == leftmost && last == header` -> clear() (leftmost, root,
//   rightmost, count in source order); else `while (first != last) erase(first++)`. The post-increment inlines the
//   template `_Rb_global<bool>::_M_increment` (its body must be visible in the TU), erase(iterator) inlines
//   `_Destroy(&value)` = ~list (_List_base::clear: free each node, re-link the header; then the null-tested header
//   deallocate) and the null-tested node deallocate.
// Leaf choice: the TU also emits erase(iterator); the range overload is the larger symbol, which the leaf prefix selects.
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

namespace std {
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

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

// ---- list<_Tp> (4 bytes: the header node pointer) ----
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
    void clear()
    {
        _Node* __cur = (_Node*)this->_M_node._M_data->_M_next;
        while (__cur != this->_M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            _Destroy(&__tmp->_M_data);
            this->_M_node.deallocate(__tmp, 1);
        }
        this->_M_node._M_data->_M_next = this->_M_node._M_data;
        this->_M_node._M_data->_M_prev = this->_M_node._M_data;
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };

// ---- _Rb_tree ----
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                               // +0x00
    _Base_ptr _M_parent;                              // +0x04
    _Base_ptr _M_left;                                // +0x08
    _Base_ptr _M_right;                               // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                            // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _Rebalance_for_erase(_Base_ptr __z, _Base_ptr& __root, _Base_ptr& __leftmost,
                                                  _Base_ptr& __rightmost);           // 0x0042A34E
    static _Base_ptr __cdecl _M_increment(_Base_ptr);
};
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    } else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value> _Self;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    _Self operator++(int)
    {
        _Self __tmp = *this;
        _M_node = _Rb_global_inst::_M_increment(_M_node);
        return __tmp;
    }
};

template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;

    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }

private:
    void _M_erase(_Link_type __x);                    // 0x00519970
public:
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
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
    void erase(iterator __first, iterator __last);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::erase(iterator __first, iterator __last)
{
    if (__first._M_node == _M_leftmost() && __last._M_node == this->_M_header._M_data)
        clear();
    else
        while (__first != __last) erase(__first++);
}

typedef pair<const long, list<long> > SListEntry;
template void _Rb_tree<long, SListEntry, _Select1st<SListEntry>, less<long> >::erase(iterator, iterator);
}