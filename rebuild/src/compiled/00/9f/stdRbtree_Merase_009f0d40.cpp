// std::_Rb_tree::_M_erase(_Rb_tree_node<value_type>* x) -- (map<unsigned long, CCodeSectionManager::CSectionDesc >) STLport 4.6 recursive subtree release without rebalancing: for each node erase its right subtree, destroy the value, free the node and continue with its left child
// Leaf: ?_M_erase@?$_Rb_tree@KU?$pair@$$CBKUCSectionDesc@CCodeSectionManager@@@std@@U?$_Select1st@U?$pair@$$CBKUCSectionDesc@CCodeSectionManager@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKUCSectionDesc@CCodeSectionManager@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBKUCSectionDesc@CCodeSectionManager@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F0D40, 70 bytes (+ int3 padding), thiscall, `ret 4`; recursive.
// Identity (proven): std::_Rb_tree<unsigned long, pair<const unsigned long, CCodeSectionManager::CSectionDesc >, _Select1st<...>, less<unsigned long >, allocator<...> >::_M_erase;
//   evidence: its caller 0x009F1560 is the CCodeSectionManager teardown (binding ?DestroyCodeSectionManager@@YIXPAVCCodeSectionManager@@@Z), with the clear / erase helpers 0x009F0F30, 0x009F1130, 0x009F1400; Ego_r.pdb CCodeSectionManager::SectionDesc is map<unsigned long, CCodeSectionManager::CSectionDesc> (ShortName, LongName, Loaded, Size), and the node destroys two CCharStrings at +0x14 / +0x18 after the 4-byte key.
// Label: the manifest's "BuildTreeArray" is propagated from a byte-identical-shape body elsewhere (family J090: the 70-byte
//   _M_erase bodies whose value has two out-of-line destructors; retail has no identical-code folding, so each address is its
//   own tree type).
// Callees: itself (the right subtree), ~CCharString 0x0099EAE0 (??1CCharString@@QAE@XZ) on LongName +0x18, then on ShortName +0x14, free 0x00BFEA14 (allocator<_Node>::deallocate).
// Model: agent952 H079's STLport _tree.c `_M_erase` (private); the value's implicit destructor inline.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
class CCharString {                                        // 4 bytes (TLC)
public:
    ~CCharString();                                        // 0x0099EAE0
    void* PStringData;                                     // +0x00
};
class CCodeSectionManager {                                // partial view: only the nested type is used here
public:
    struct CSectionDesc {                                  // 16 bytes (Ego_r.pdb)
        CCharString ShortName;                             // +0x00
        CCharString LongName;                              // +0x04
        bool Loaded;                                       // +0x08
        long Size;                                         // +0x0C
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
typedef std::pair<unsigned long const, CCodeSectionManager::CSectionDesc > TValue_009f0d40;
template class std::_Rb_tree<unsigned long, TValue_009f0d40, std::_Select1st<TValue_009f0d40 >, std::less<unsigned long >, std::allocator<TValue_009f0d40 > >;