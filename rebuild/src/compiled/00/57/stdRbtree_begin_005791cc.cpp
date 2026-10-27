// std::_Rb_tree::begin() -- (CUnprovenKey_005791cc tree) STLport: return an iterator on the leftmost node (the header's left link)
// Leaf: ?begin@?$_Rb_tree@UCUnprovenKey_005791cc@@UCUnprovenValue_005791cc@@U?$_Select1st@UCUnprovenValue_005791cc@@@std@@U?$less@UCUnprovenKey_005791cc@@@4@V?$allocator@UCUnprovenValue_005791cc@@@4@@std@@QAE?AU?$_Rb_tree_iterator@UCUnprovenValue_005791cc@@U?$_Nonconst_traits@UCUnprovenValue_005791cc@@@std@@@2@XZ
// Retail: Fable.exe @ 0x005791CC, 14 bytes, thiscall `ret 4` (hidden iterator result pointer); /Os TU (packed address).
// Identity (descriptive types): the STLport 4.6 `_Rb_tree::begin()` shape (`return iterator(_M_leftmost())`: header node
// pointer +0, its _M_left +8, the 4-byte iterator returned through the hidden pointer because it has user constructors).
// It sits in the size-optimised CTCInventoryExperience TU among that class's map pieces (_M_find 0x00579114 /
// 0x0057914C for UndoableStatBuys / UndoableAbilityBuys, list<long>::pop_back 0x005791B5); it has no direct caller
// (the xref at 0x004889C0 is the inside of a call rel32), so which of the class's five maps it belongs to is unproven:
// CUnprovenKey_005791cc / CUnprovenValue_005791cc.
// Callees: none.
// Shape: the node load first (`mov eax,[ecx] ; mov ecx,[eax+8]`), then the result pointer, the store, `ret 4` with the
// result pointer left in eax.
#pragma optimize("s", on)
struct CUnprovenKey_005791cc {
    long Word;
};
struct CUnprovenValue_005791cc {
    typedef CUnprovenKey_005791cc first_type;
    CUnprovenKey_005791cc first;
    long second;
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x.Word < __y.Word; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                                   // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
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
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare _M_key_compare;                                       // +0x08

protected:
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
public:
    iterator begin() { return iterator(_M_leftmost()); }
};
}

typedef std::_Rb_tree<CUnprovenKey_005791cc, CUnprovenValue_005791cc, std::_Select1st<CUnprovenValue_005791cc>,
                      std::less<CUnprovenKey_005791cc>, std::allocator<CUnprovenValue_005791cc> > TTree_005791cc;
template TTree_005791cc::iterator TTree_005791cc::begin();