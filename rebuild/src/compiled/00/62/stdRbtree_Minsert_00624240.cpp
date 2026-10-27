// std::_Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- (map<CThing*, NUISystem::CComponent*>) STLport 4.6 node insert
// Leaf: ?_M_insert@?$_Rb_tree@PAVCThing@@U?$pair@QAVCThing@@PAVCComponent@NUISystem@@@std@@U?$_Select1st@U?$pair@QAVCThing@@PAVCComponent@NUISystem@@@std@@@3@U?$less@PAVCThing@@@3@V?$allocator@U?$pair@QAVCThing@@PAVCComponent@NUISystem@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@QAVCThing@@PAVCComponent@NUISystem@@@std@@U?$_Nonconst_traits@U?$pair@QAVCThing@@PAVCComponent@NUISystem@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@QAVCThing@@PAVCComponent@NUISystem@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00624240, 191 bytes, thiscall, `ret 14h` (the hidden iterator result pointer, then x, y, &v, w).
// Identity (proven): bound ?_M_insert@?$_Rb_tree@PAVCThing@@U?$pair@QAVCThing@@PAVCComponent@NUISystem@@@std@@...; called by that map's insert_unique 0x00624620.
// Callees: malloc 0x00BFEA0E (inline allocate, 0x18-byte node), std::_Rb_global<bool>::_Rebalance 0x0042971D (__cdecl).
// Shape (family M137, representative 0x0056D5C0): the landed STLport-verbatim _M_insert (0x00835270 / H011 model) over a map's
//   8-byte value (two dword moves behind placement new's null test); keys compare unsigned (`jb`).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }
class CThing;
namespace NUISystem { class CComponent; }
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static void __cdecl _Rebalance(_Base_ptr __x, _Base_ptr& __root);           // 0x0042971D
};
struct _Rb_tree_base_iterator { typedef _Rb_tree_node_base* _Base_ptr; _Base_ptr _M_node; };
template <class _Tp> struct _Nonconst_traits { };
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
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    _Link_type _M_allocate_node() { return (_Link_type)malloc(sizeof(_Node)); }
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = _M_allocate_node();
        new (&__tmp->_M_value_field) value_type(__x);
        return __tmp;
    }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Base_ptr __x_, _Base_ptr __y_, const value_type& __v, _Base_ptr __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;
    if (__y == this->_M_header._M_data ||
        (__w == 0 && (__x != 0 || _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        __y->_M_left = __z;
        if (__y == this->_M_header._M_data) {
            this->_M_header._M_data->_M_parent = __z;
            this->_M_header._M_data->_M_right = __z;
        }
        else if (__y == this->_M_header._M_data->_M_left)
            this->_M_header._M_data->_M_left = __z;
    }
    else {
        __z = _M_create_node(__v);
        __y->_M_right = __z;
        if (__y == this->_M_header._M_data->_M_right)
            this->_M_header._M_data->_M_right = __z;
    }
    __z->_M_parent = __y;
    __z->_M_left = 0;
    __z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++this->_M_node_count;
    return iterator(__z);
}
}
typedef std::pair<CThing* const, NUISystem::CComponent*> TValue;
typedef std::_Rb_tree<CThing*, TValue, std::_Select1st<TValue>, std::less<CThing*>, std::allocator<TValue> > TTree;
template TTree::iterator TTree::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const TValue&, std::_Rb_tree_node_base*);