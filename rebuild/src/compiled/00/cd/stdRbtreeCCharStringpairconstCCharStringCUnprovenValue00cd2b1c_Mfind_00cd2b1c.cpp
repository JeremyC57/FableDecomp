// std::_Rb_tree<CCharString, pair<const CCharString, CUnprovenValue_00cd2b1c>, ...>::_M_find(const CCharString& k) const -- STLport exact-key lookup: the last node whose key is not less than k, unless it is the header or k is less than its key (then the header)
// Leaf: ?_M_find@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@ECUnprovenValue_00cd2b1c@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD2B1C, 92 bytes, thiscall `ret 4`, frame pointer (size TU); returns the node.
// Identity (descriptive): a CCharString-keyed tree of the cutscene code (caller 0x00CBF7FE, the cutscene sound player); probably one of RunCutsceneMacro's flag/argument maps, not proven.
// Callees: std::less<CCharString>::operator() 0x0042AAB3 (out of line in this TU, `this` = the tree's comparator at +8).
// Shape (family K085, representative 0x0042B467): the landed _M_find 0x00756350 text under `optimize("y", off)` +
//   `("s", on)`: the comparator call is not inlined, the candidate lives in the one frame word and the header, read
//   into a local declared after the walk cursors, stays in edi for the final test.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public: void* PStringData; };
struct CUnprovenValue_00cd2b1c { };                     // mapped type not recoverable (only the key is read here)
namespace std {

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const;           // 0x0042AAB3 for CCharString
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};

template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    static const _Key& _S_key(_Link_type __x) { return _KeyOfValue()(__x->_M_value_field); }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
    protected:
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    public:
    private:
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;
    public:
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node<_Value>* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_find(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;      // last node which is not less than __k
    _Link_type __x = _M_root();                    // current node
    _Link_type __header = this->_M_header._M_data; // declared last: VC7.1 gives it edi (retail), __y the frame word
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    if (__y == __header || _M_key_compare(__k, _S_key(__y)))
        __y = __header;
    return __y;
}

typedef pair<const CCharString, CUnprovenValue_00cd2b1c> _Val;
template _Rb_tree_node<_Val>* _Rb_tree<CCharString, _Val, _Select1st<_Val>, less<CCharString>, allocator<_Val> >::_M_find(const CCharString&) const;

} // namespace std