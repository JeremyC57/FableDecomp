// std::_Rb_tree::_M_create_node(const value_type& x) -- (map<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, vector<pair<float, CWideString> > >) STLport 4.6 node factory: malloc a 0x20-byte node and copy-construct the (key, vector) pair into its value field
// Leaf: ?_M_create_node@?$_Rb_tree@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@V?$vector@U?$pair@MVCWideString@@@std@@V?$allocator@U?$pair@MVCWideString@@@std@@@2@@std@@@std@@U?$_Select1st@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@V?$vector@U?$pair@MVCWideString@@@std@@V?$allocator@U?$pair@MVCWideString@@@std@@@2@@std@@@std@@@4@U?$less@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@@4@V?$allocator@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@V?$vector@U?$pair@MVCWideString@@@std@@V?$allocator@U?$pair@MVCWideString@@@std@@@2@@std@@@std@@@4@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@V?$vector@U?$pair@MVCWideString@@@std@@V?$allocator@U?$pair@MVCWideString@@@std@@@2@@std@@@std@@@2@ABU?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@V?$vector@U?$pair@MVCWideString@@@std@@V?$allocator@U?$pair@MVCWideString@@@std@@@2@@std@@@2@@Z
// Retail: Fable.exe @ 0x00569BC0, 46 bytes, thiscall `ret 4`.
// Identity (probable): an out-of-line copy of the _M_create_node that the landed _M_insert 0x0056A0A0 of this map inlines:
// the same 0x20-byte node (key word +0x10, the 12-byte vector +0x14) and the same value copy, the landed
// vector<pair<float, CWideString> >::vector(const vector&) 0x00566CD0, in the CTCHeroOnlineScoreboard STL run
// (0x00566CD0..0x0056A6D9). No direct caller (the only "reference", 0x0043194C, is the inside of a call rel32): an
// unreferenced COMDAT. STLport 4.6 `_tree.h` _M_create_node has no left/right clears (they live in _M_insert).
// Callees: malloc 0x00BFEA0E, vector<pair<float, CWideString> >::vector(const vector&) 0x00566CD0.
// Shape: `__tmp = allocate(1)`, then the pair's copy constructor as a null-checked placement new on `&__tmp->_M_value_field`
// (`lea eax,[esi+10h] ; test eax,eax`): the key word, then the vector copy on +0x14 with `&x.second`.
// Model: the landed _M_insert 0x0056A0A0's STLport model, with _M_create_node explicitly instantiated.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }
class CWideString {                                  // 4 bytes: shared wide string block
public:
    void* PStringData;
};
class CTCHeroOnlineScoreboard {
public:
    enum EOnlineScoreboardActivity { };
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
protected:                                                     // STLport 4.6: _M_create_node is protected
    _Link_type _M_allocate_node() { return (_Link_type)malloc(sizeof(_Node)); }
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = _M_allocate_node();
        new (&__tmp->_M_value_field) value_type(__x);
        return __tmp;
    }
public:
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // 12 bytes in retail
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector(const vector<_Tp, _Alloc>& __x);                           // out of line (0x00566CD0)
};

}   // namespace std


typedef std::pair<const CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, std::vector<std::pair<float, CWideString> > > TValue;
typedef std::_Rb_tree<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, TValue, std::_Select1st<TValue>, std::less<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity >, std::allocator<TValue> > TTree;
template TTree::_Link_type TTree::_M_create_node(const TValue&);