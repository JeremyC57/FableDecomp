// std::_Rb_tree<float, pair<const float, const CANavQuadTreeNode*>, ...>::insert_equal(const value_type&) -- multimap insert
// Retail: Fable.exe 0x00A222B0, 64 bytes: STLport 4.6 insert_equal -- y = header, x = root; walk down with
// `less<float>(v.first, key(x)) ? left : right` (`fld [v] ; fcomp [x+10h] ; test ah,5 ; jp`), then
// _M_insert(x, y, v, 0) (0x00A21E80) into the hidden iterator result; `ret 8`.
// Identity: the walk compares a float key at node +0x10 and the only caller 0x00A22440 is the one-line
// multimap::insert(const value_type&) returning the iterator; _M_insert 0x00A21E80 allocates 0x18-byte nodes and copies
// an 8-byte value (float key + one pointer) -- a multimap<float, 4-byte>. The PDB name sets hold exactly one such
// multimap, multimap<float, const CANavQuadTreeNode*> (Ego_r/FableWin insert/ctor/dtor), and its callers here are the
// CNavigatorAStar routines (0x00A1D8D4 / 0x00A1DC14 call _M_insert). STLport 4.6 _Rb_tree layout: _M_header (node
// pointer) +0, _M_node_count +4, _M_key_compare +8; node: color, parent +4, left +8, right +0xC, value +0x10.
// Leaf: ?insert_equal@?$_Rb_tree@MU?$pair@$$CBMPBVCANavQuadTreeNode@@@std@@
class CANavQuadTreeNode;

namespace std {
    template <class _Tp> class allocator { public: };
    template <class _T1, class _T2> struct pair { typedef _T1 first_type; typedef _T2 second_type; _T1 first; _T2 second; };
    template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
    template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };

    struct _Rb_tree_node_base {
        typedef _Rb_tree_node_base* _Base_ptr;
        bool      _M_color;                        // +0x00
        _Base_ptr _M_parent;                       // +0x04
        _Base_ptr _M_left;                         // +0x08
        _Base_ptr _M_right;                        // +0x0C
    };
    template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
        _Value _M_value_field;                     // +0x10
    };
    template <class _Value> struct _Nonconst_traits { typedef _Value value_type; typedef _Value& reference; typedef _Value* pointer; };
    struct _Rb_tree_base_iterator {
        _Rb_tree_node_base* _M_node;
    };
    template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
        _Rb_tree_iterator() { }
        _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
    };

    template <class _Tp, class _Alloc> class _Rb_tree_base {
    public:
        _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
    };

    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
    class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
    public:
        typedef _Rb_tree_node_base* _Base_ptr;
        typedef _Rb_tree_node<_Value>* _Link_type;
        typedef _Value value_type;
        typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    protected:
        unsigned int _M_node_count;                // +0x04
        _Compare _M_key_compare;                   // +0x08
        _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
        static _Link_type& _S_left(_Link_type __x) { return (_Link_type&)(__x->_M_left); }
        static _Link_type& _S_right(_Link_type __x) { return (_Link_type&)(__x->_M_right); }
        static const _Key& _S_key(_Link_type __x) { return _KeyOfValue()(__x->_M_value_field); }
    private:
        iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x00A21E80
    public:
        iterator insert_equal(const value_type& __v);
    };

    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_equal(const _Value& __v)
    {
        _Link_type __y = this->_M_header._M_data;
        _Link_type __x = _M_root();
        while (__x != 0) {
            __y = __x;
            __x = _M_key_compare(_KeyOfValue()(__v), _S_key(__x)) ? _S_left(__x) : _S_right(__x);
        }
        return _M_insert(__x, __y, __v);
    }
}

namespace std {
    template class _Rb_tree<float, pair<const float, const CANavQuadTreeNode*>,
                            _Select1st<pair<const float, const CANavQuadTreeNode*> >, less<float>,
                            allocator<pair<const float, const CANavQuadTreeNode*> > >;
}