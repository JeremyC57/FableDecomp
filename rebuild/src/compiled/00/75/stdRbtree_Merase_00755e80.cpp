// std::_Rb_tree::_M_erase(_Rb_tree_node<value_type>* x) -- (map<CCharString, pair<long, long> >) STLport 4.6 recursive subtree release without rebalancing: for each node erase its right subtree, destroy the value, free the node and continue with its left child
// Leaf: ?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00755E80, 59 bytes (+ int3 padding to 64), thiscall `ret 4`; recursive
//   (calls itself at 0x00755E96); speed-optimised TU.
// Identity (proven): map<CCharString, pair<long, long> >; evidence: its callers are the landed map erase(first, last) 0x00756420 (CTCHeroRememberCrimes::RegionNamesToCrimeDurationAndFine, 0x1C-byte nodes) and GFSerialiseMapBinaryIn<pair<long, long>, CCharString> 0x007565E0 (the map's clear() before reading); Ego_r.pdb CTCHeroRememberCrimes::RegionNamesToCrimeDurationAndFine is map<CCharString, pair<long, long> >
// Label: the manifest's "BuildTreeArray" is propagated (family H063: six _M_erase bodies that differ only in the
//   destructor called and its offset in the node; retail has no identical-code folding, so each address is one
//   instantiation); this is STLport 4.6 `_Rb_tree::_M_erase` (`_tree.c`).
// Callees: the pair destructor inline: CCharString::~CCharString 0x0099EAE0 on the key (+0x10; the pair<long, long> value is trivial), free 0x00BFEA14 (allocator<_Node>::deallocate, no null test: x was dereferenced).
// Model: the landed _M_erase 0x007AE0E0 over the five-parameter _Rb_tree of the landed _M_insert 0x006BD140.
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
class CCharString {                                  // 4 bytes: shared string block
public:
    void* PStringData;
    ~CCharString();                                   // 0x0099EAE0
    bool operator<(const CCharString& other) const;
};

namespace std {

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
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

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

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

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
private:
    void _M_erase(_Link_type __x);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x)
{
    // erase without rebalancing
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        this->_M_header.deallocate(__x, 1);
        __x = __y;
    }
}
}   // namespace std

typedef std::_Rb_tree<CCharString, std::pair<const CCharString, std::pair<long, long> >, std::_Select1st<std::pair<const CCharString, std::pair<long, long> > >, std::less<CCharString >, std::allocator<std::pair<const CCharString, std::pair<long, long> > > > TTree;
template class std::_Rb_tree<CCharString, std::pair<const CCharString, std::pair<long, long> >, std::_Select1st<std::pair<const CCharString, std::pair<long, long> > >, std::less<CCharString >, std::allocator<std::pair<const CCharString, std::pair<long, long> > > >;