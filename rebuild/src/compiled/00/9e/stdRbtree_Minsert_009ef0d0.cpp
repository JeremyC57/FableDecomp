// std::_Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- (map<char, CCharString>) STLport 4.6 node insert: create a node for v, link it left or right of y, fix root / leftmost / rightmost, rebalance, count, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@DU?$pair@$$CBDVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBDVCCharString@@@std@@@2@U?$less@D@2@V?$allocator@U?$pair@$$CBDVCCharString@@@std@@@2@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBDVCCharString@@@std@@U?$_Nonconst_traits@U?$pair@$$CBDVCCharString@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBDVCCharString@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EF0D0, 202 bytes (+ int3 padding to 208), thiscall `ret 14h` (the hidden
//   iterator result pointer first, then x, y, &v, w).
// Identity (proven): map<char, CCharString>; evidence: Ego_r.pdb CConsole::KeyBindingMap is map<char, CCharString> (the only map<char, ..> member in Ego_r); this body compares keys as signed bytes (`mov al,[ebp]; cmp al,[ebx+10h]; jl`), mallocs 0x18-byte nodes and copies the CCharString through 0x0099EC30; its callers are the landed map<char, CCharString>::insert_unique 0x009EF1A0 and 0x009EF4D0, reached from CConsole::Initialise; FableWin.pdb names map<char, CCharString>.
// Label: the manifest's "RBTree_Insert" is descriptive (family H011: seven rows that differ only in the key compare width,
//   the node size and the value copy call; retail has no identical-code folding, so each address is one instantiation);
//   this is STLport 4.6 `_Rb_tree::_M_insert` (`_tree.c`).
// Callees (this instantiation): malloc 0x00BFEA0E (inline allocate, 0x18-byte node), the value copy CCharString::CCharString(const CCharString&)
//   @ 0x0099EC30 (thiscall on node+0x14, &v.second), _Rb_global<bool>::_Rebalance 0x0042971D (__cdecl).
// Model: the landed _M_insert 0x006BD140 (STLport 4.6 verbatim) over the five-parameter _Rb_tree; the pair copy
//   constructor inlines into _M_create_node as the null-checked placement copy (key word, then the out-of-line value copy).
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }
class CCharString {                                  // 4 bytes: shared string block
public:
    void* PStringData;
    CCharString(const CCharString& other);            // 0x0099EC30
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
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
};

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

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
    typedef _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc> _Self;
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Rb_tree(const _Self& __x);                                    // out of line where used (not here)
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
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Base_ptr __x_, _Base_ptr __y_, const value_type& __v,
                                                                 _Base_ptr __w_)
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
}   // namespace std


typedef std::pair<const char, CCharString > TValue;
typedef std::_Rb_tree<char, TValue, std::_Select1st<TValue>, std::less<char >, std::allocator<TValue> > TTree;
template TTree::iterator TTree::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const TValue&,
                                          std::_Rb_tree_node_base*);