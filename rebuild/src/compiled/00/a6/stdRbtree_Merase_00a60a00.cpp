// std::_Rb_tree::_M_erase(_Rb_tree_node<value_type>* x) -- (map<CWideString, CChecksumCacheFile::CEntry>) STLport 4.6 recursive subtree release without rebalancing: for each node erase its right subtree, destroy the value, free the node and continue with its left child
// Leaf: ?_M_erase@?$_Rb_tree@VCWideString@@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@U?$_Select1st@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@@3@U?$less@VCWideString@@@3@V?$allocator@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A60A00, 59 bytes (+ int3 padding to 64), thiscall `ret 4`; recursive
//   (calls itself at 0x00A60A16); speed-optimised TU.
// Identity (proven): map<CWideString, CChecksumCacheFile::CEntry>; evidence: it destroys a CWideString at +0x10; its callers are CChecksumCacheFile::Close 0x00A60C30, the destructor 0x00A60CE0 and Open 0x00A60D40 (landed), whose map is Ego_r.pdb CChecksumCacheFile::Entries, map<CWideString, CChecksumCacheFile::CEntry> (0x34-byte nodes in the landed _M_insert 0x00A61270)
// Label: the manifest's "BuildTreeArray" is propagated (family H063: six _M_erase bodies that differ only in the
//   destructor called and its offset in the node; retail has no identical-code folding, so each address is one
//   instantiation); this is STLport 4.6 `_Rb_tree::_M_erase` (`_tree.c`).
// Callees: the pair destructor inline: CWideString::~CWideString 0x0099B510 on the key (+0x10; the 0x20-byte CEntry is trivial), free 0x00BFEA14 (allocator<_Node>::deallocate, no null test: x was dereferenced).
// Model: the landed _M_erase 0x007AE0E0 over the five-parameter _Rb_tree of the landed _M_insert 0x006BD140.
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
class CWideString {                                  // 4 bytes: shared wide string block
public:
    void* PStringData;
    ~CWideString();                                   // 0x0099B510
    bool operator<(const CWideString& other) const;   // 0x0099AF70
};
class CChecksumCacheFile {
public:
    struct CEntry {                                   // 0x20 bytes, trivially destructible (FableWin.pdb: UCEntry)
        unsigned char Data[0x20];
    };
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

typedef std::_Rb_tree<CWideString, std::pair<const CWideString, CChecksumCacheFile::CEntry>, std::_Select1st<std::pair<const CWideString, CChecksumCacheFile::CEntry> >, std::less<CWideString >, std::allocator<std::pair<const CWideString, CChecksumCacheFile::CEntry> > > TTree;
template class std::_Rb_tree<CWideString, std::pair<const CWideString, CChecksumCacheFile::CEntry>, std::_Select1st<std::pair<const CWideString, CChecksumCacheFile::CEntry> >, std::less<CWideString >, std::allocator<std::pair<const CWideString, CChecksumCacheFile::CEntry> > >;