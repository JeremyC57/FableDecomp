// std::_Rb_tree<CCharString, pair<const CCharString, long>, _Select1st, less<CCharString> >::_M_upper_bound(const CCharString& k) const -- walk from the root to the first node whose key sorts after k, the header when none
// Grade: DIFFER(73v73) -- register allocation only: retail keeps y (the result) in edi and k's hoisted data pointer in ebx (pushed after the root test); this source gives y ebx and the key edi. With the compare 0x00411570 only declared, y gets edi but k's data pointer is re-read per iteration (the hoist needs the body in the TU); body after the caller and a register-resident key copy changed nothing.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00475D50, 73 bytes, `ret 4`, the node pointer in eax.
// Callers: equal_range @ 0x00475CDD (call 0x00475CE5), itself called by erase(key) 0x00475BF6, which the landed
// CPersistContext::TransferMap<CCharString, long> 0x0047550F calls -- so the tree is map<CCharString, long>.
// Callees: the CBasicString<char> data compare @ 0x00411570 (__fastcall (const char*, const char*), landed).
// Shape: STLport 4.x `y = header; x = root; while (x) if (comp(k, key(x))) y = x, x = left(x); else x = right(x);`
// with less<CCharString> inline: CCharString::operator< (equal data pointers -> false, null left -> true, null right
// -> false, else the data compare `< 0`, materialised as a bool: `setl al ; test al,al`). k's data pointer is read
// once before the loop (the compare body is in the TU). Speed form: top-guarded loop, no frame pointer.
// Label correction: catalog "RBTree_FindNode_Reverse" is a guess.
// Leaf: _M_upper_bound

// 0x00411570 (the landed body), defined here and kept out of line: VC7.1 then sees it writes no memory and hoists
// k's data pointer out of the loop (retail's copy is folded from every TU that instantiates it).
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

template <class T> class CBasicString {
public:
    T* PData;                                               // +0x00
    bool operator<(const CBasicString& other) const { return Compare_00411570(PData, other.PData) < 0; }
};

class CCharString {
public:
    CBasicString<char>* PStringData;
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return *PStringData < *other.PStringData;
    }
};

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
};

template <class _Value, class _Tp> class _STLP_alloc_proxy {
public:
    _Value _M_data;
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Key, class _Value, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value> > _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04
    _Compare _M_key_compare;                                            // +0x08

    static const _Key& _S_key(_Link_type __x) { return __x->_M_value_field.first; }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    _Link_type _M_upper_bound(const _Key& __k) const;
};

template <class _Key, class _Value, class _Compare>
_Rb_tree_node<_Value>* _Rb_tree<_Key, _Value, _Compare>::_M_upper_bound(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;               // last node which is greater than __k
    _Link_type __x = _M_root();                             // current node
    while (__x != 0)
        if (_M_key_compare(__k, _S_key(__x)))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    return __y;
}

template _Rb_tree_node<pair<const CCharString, long> >*
_Rb_tree<CCharString, pair<const CCharString, long>, less<CCharString> >::_M_upper_bound(const CCharString&) const;
}
