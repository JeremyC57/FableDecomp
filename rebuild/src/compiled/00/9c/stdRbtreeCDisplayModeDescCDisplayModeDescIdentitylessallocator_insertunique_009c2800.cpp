// std::_Rb_tree<CDisplayModeDesc, CDisplayModeDesc, _Identity, less, allocator>::insert_unique(const value_type& v) -- STLport unique insert for set<CDisplayModeDesc>: walk down to the insertion parent, step back one node when the walk ended left, insert unless that node's mode equals v
// Leaf: ?insert_unique@?$_Rb_tree@VCDisplayModeDesc@@V1@U?$_Identity@VCDisplayModeDesc@@@std@@U?$less@VCDisplayModeDesc@@@3@V?$allocator@VCDisplayModeDesc@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@VCDisplayModeDesc@@U?$_Nonconst_traits@VCDisplayModeDesc@@@std@@@std@@_N@2@ABVCDisplayModeDesc@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C2800, 248 bytes, thiscall `ret 8` (hidden pair<iterator,bool> result + v).
// Identity: set<CDisplayModeDesc> (proven): the 16-byte key at node +0x10 is compared as four signed longs in order,
//   which is Ego_r.pdb CDisplayModeDesc (Dimensions.X, Dimensions.Y, BitsPerPixel, RefreshRate) with its
//   `??MCDisplayModeDesc@@QBE_NABV0@@Z`; Ego_r.pdb also has set<CDisplayModeDesc> (_Tset_traits<CDisplayModeDesc,
//   less<CDisplayModeDesc>>); callers 0x009C03C1 (landed 0x009C02D0, which binds this address as set<CDisplayModeDesc>
//   insert_unique) and 0x009C2B9D. The manifest label "BinarySearchTree_Insert_4int" is behavioural.
// Callees: _Rb_tree_iterator<CDisplayModeDesc, _Nonconst_traits<CDisplayModeDesc> >::operator--() 0x009C1F70 (out of
//   line, thiscall on the iterator; landed under the placeholder CUnprovenValue_009c1f70, NAME_FIXES.tsv), _M_insert(x,
//   y, v, w) 0x009C2350.
// Shape: the landed pair<long,long> insert_unique 0x00AD4720 model (STLport 4.6 _tree.c verbatim) with the
//   lexicographic four-field operator< inline; operator-- carries a same-TU noinline body.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
inline void* operator new(unsigned int, void* p) { return p; }

class C2DExtentsI {                                     // Ego_r.pdb, 8 bytes
public:
    long X;                                             // +0
    long Y;                                             // +4
};

class CDisplayModeDesc {                                // Ego_r.pdb, 16 bytes
public:
    C2DExtentsI Dimensions;                             // +0x0
    long        BitsPerPixel;                           // +0x8
    long        RefreshRate;                            // +0xC
    bool operator<(const CDisplayModeDesc& other) const // Ego_r.pdb ??MCDisplayModeDesc@@QBE_NABV0@@Z (inline here)
    {
        if (Dimensions.X != other.Dimensions.X)
            return Dimensions.X < other.Dimensions.X;
        if (Dimensions.Y != other.Dimensions.Y)
            return Dimensions.Y < other.Dimensions.Y;
        if (BitsPerPixel != other.BitsPerPixel)
            return BitsPerPixel < other.BitsPerPixel;
        return RefreshRate < other.RefreshRate;
    }
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static void __cdecl _Rebalance(_Base_ptr __x, _Base_ptr& __root);                         // 0x0042971D
    static _Base_ptr __cdecl _Rebalance_for_erase(_Base_ptr __z, _Base_ptr& __root,
                                                  _Base_ptr& __leftmost, _Base_ptr& __rightmost);   // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
    }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    typename _Traits::reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    _Self& operator++() { _M_increment(); return *this; }
    __declspec(noinline) _Self& operator--()           // out of line in retail (0x009C1F70 for this set)
    {
        if (_M_node->_M_color == false && _M_node->_M_parent->_M_parent == _M_node)
            _M_node = _M_node->_M_right;
        else if (_M_node->_M_left != 0) {
            _Base_ptr __y = _M_node->_M_left;
            while (__y->_M_right != 0)
                __y = __y->_M_right;
            _M_node = __y;
        }
        else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_left) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            _M_node = __y;
        }
        return *this;
    }
    bool operator==(const _Self& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Self& __y) const { return _M_node != __y._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type _M_allocate_node() { return (_Link_type)malloc(sizeof(_Node)); }
    void _M_deallocate_node(_Link_type __p) { if (__p != 0) free(__p); }
protected:
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = _M_allocate_node();
        new (&__tmp->_M_value_field) value_type(__x);
        return __tmp;
    }
protected:
    _Link_type _M_clone_node(_Link_type __x)
    {
        _Link_type __tmp = _M_create_node(__x->_M_value_field);
        __tmp->_M_color = __x->_M_color;
        __tmp->_M_left = 0;
        __tmp->_M_right = 0;
        return __tmp;
    }
public:
    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(_M_header._M_data); }

private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
private:
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
public:
    pair<iterator, bool> insert_unique(const value_type& __x);
    _Base_ptr _M_lower_bound(const key_type& __k) const;
    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
            _M_header._M_data->_M_parent, _M_header._M_data->_M_left, _M_header._M_data->_M_right);
        __y->_M_value_field.~value_type();
        _M_deallocate_node(__y);
        --_M_node_count;
    }
};

}   // namespace std

namespace std {
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const value_type& __v)
{
    _Link_type __y = _M_header._M_data;
    _Link_type __x = _M_root();
    bool __comp = true;
    while (__x != 0) {
        __y = __x;
        __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp)
        if (__j == begin())
            return pair<iterator, bool>(_M_insert(/* __x*/ __y, __y, __v), true);
        else
            --__j;
    if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
        return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return pair<iterator, bool>(__j, false);
}
}   // namespace std

typedef std::_Rb_tree<CDisplayModeDesc, CDisplayModeDesc, std::_Identity<CDisplayModeDesc>,
                       std::less<CDisplayModeDesc>, std::allocator<CDisplayModeDesc> > TDisplayModeSetTree;
template std::pair<TDisplayModeSetTree::iterator, bool> TDisplayModeSetTree::insert_unique(const CDisplayModeDesc&);