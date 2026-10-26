// std::_Rb_tree::_M_erase(_Rb_tree_node<value_type>* x) -- (map<unsigned __int64, pair<CCharString, CWideString> >) STLport 4.6 recursive subtree release without rebalancing: for each node erase its right subtree, destroy the value, free the node and continue with its left child
// Leaf: ?_M_erase@?$_Rb_tree@_KU?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@U?$_Select1st@U?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005286D0, 70 bytes (+ int3 padding), thiscall, `ret 4`; recursive.
// Identity (proven): std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, pair<CCharString, CWideString> >, _Select1st<...>, less<unsigned __int64 >, allocator<...> >::_M_erase;
//   evidence: the 8-byte key at +0x10 is trivially destroyed and the mapped pair holds a CCharString at +0x18 and a CWideString at +0x1C -- Ego_r.pdb's only map of that shape is CThingManager::NPCNameMap (map<unsigned __int64, pair<CCharString, CWideString> >), and every caller (0x00523440, 0x00528FB0, 0x00529FB0, 0x0052AF40) is in the CThingManager TU beside the landed NPCNameMap lower-bound 0x00528610.
// Label: the manifest's "BuildTreeArray" is propagated from a byte-identical-shape body elsewhere (family J090: the 70-byte
//   _M_erase bodies whose value has two out-of-line destructors; retail has no identical-code folding, so each address is its
//   own tree type).
// Callees: itself (the right subtree), ~CWideString 0x0099B510 (??1CWideString@@QAE@XZ) on second.second +0x1C, ~CCharString 0x0099EAE0 (??1CCharString@@QAE@XZ) on second.first +0x18, free 0x00BFEA14 (allocator<_Node>::deallocate).
// Model: agent952 H079's STLport _tree.c `_M_erase` (private); the value's implicit destructor inline.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
class CCharString {                                        // 4 bytes (TLC)
public:
    ~CCharString();                                        // 0x0099EAE0
    void* PStringData;                                     // +0x00
};
class CWideString {                                        // 4 bytes (TLC)
public:
    ~CWideString();                                        // 0x0099B510
    void* PStringData;                                     // +0x00
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
    _Color_type _M_color;                                  // +0x00
    _Base_ptr   _M_parent;                                 // +0x04
    _Base_ptr   _M_left;                                   // +0x08
    _Base_ptr   _M_right;                                  // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                 // +0x10
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                            // +0x04
    _Compare     _M_key_compare;                           // +0x08
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
} // namespace std
typedef std::pair<unsigned __int64 const, std::pair<CCharString, CWideString> > TValue_005286d0;
template class std::_Rb_tree<unsigned __int64, TValue_005286d0, std::_Select1st<TValue_005286d0 >, std::less<unsigned __int64 >, std::allocator<TValue_005286d0 > >;