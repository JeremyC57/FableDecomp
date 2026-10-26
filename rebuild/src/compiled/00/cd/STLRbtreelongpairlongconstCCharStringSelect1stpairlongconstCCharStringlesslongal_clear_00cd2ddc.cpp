// _Rb_tree<long,pair<const long,CCharString>,...>::clear() -- STLport map<long,CCharString> clear: erase every node, then reset the header to the empty state
// Leaf: ?clear@?$_Rb_tree@JU?$pair@$$CBJVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBJVCCharString@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCCharString@@@std@@@2@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD2DDC, 41 bytes, thiscall, size TU (pragma optimize s):
//   `push esi ; mov esi,ecx ; cmp [esi+4],0 ; je ; mov eax,[esi] ; push [eax+4] ; call _M_erase ; [eax+8] = header ;
//   [eax+4] = 0 ; [eax+0xC] = header ; [esi+4] = 0` -- the header pointer is reloaded for each store (the stores go through it).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 452 / hand452 README: the landed "CTCCarryable::OnKill" row is the tree clear):
//   callees 0x00CD29F0 (the map<long,CCharString> _M_erase, landed Rbtree_Merase_00cd29f0); callers its tree destructor 0x00CD3211 (call at 0x00CD3214, the only caller).
//   Node {colour +0, parent +4, left +8, right +0xC}; tree {header +0, node count +4, compare +8}: STLport 4.6 _tree.h,
//   the same model as the landed _M_erase rows of this TU (reloc_bindings names them ?_M_erase@?$_Rb_tree@...@std@@).
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
class CCharString {
public:
    void* m_pData;
    ~CCharString();                                     // 0x0099EAE0
};
namespace std {
template <int __inst> class __malloc_alloc {             // _STLP_USE_MALLOC: the node allocator frees straight to the CRT
public:
    static void deallocate(void* p, unsigned int) { free(p); }
};
typedef __malloc_alloc<0> __sgi_alloc;
template <class T> struct less {
    bool operator()(const T& a, const T& b) const;
};
template <class T> class allocator {
public:
    void deallocate(T* p, unsigned int n) { if (p != 0) __sgi_alloc::deallocate((void*)p, n * sizeof(T)); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    void deallocate(_Tp* p, unsigned int n) { _MaybeReboundAlloc::deallocate(p, n); }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> struct _Identity { };
template <class P> struct _Select1st { };
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                   // +0x00
    _Base_ptr _M_parent;                                  // +0x04
    _Base_ptr _M_left;                                    // +0x08
    _Base_ptr _M_right;                                   // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                     // +0x10
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};
template <class K, class V, class KoV, class C, class A>
class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<V>* _Link_type;
    unsigned int _M_node_count;                           // +0x04
    C            _M_key_compare;                          // +0x08
    _Base_ptr& _M_root() const { return (_Base_ptr&)(this->_M_header._M_data->_M_parent); }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)(this->_M_header._M_data->_M_left); }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)(this->_M_header._M_data->_M_right); }
private:
    void _M_erase(_Link_type x);                          // out of line (0x00CD2919 / 0x00CD29F0)
public:
    void clear();
    ~_Rb_tree();
};
}

template <class K, class V, class KoV, class C, class A>
void std::_Rb_tree<K, V, KoV, C, A>::clear()           // _tree.h
{
    if (_M_node_count != 0) {
        _M_erase((_Link_type)_M_root());
        _M_leftmost() = this->_M_header._M_data;
        _M_root() = 0;
        _M_rightmost() = this->_M_header._M_data;
        _M_node_count = 0;
    }
}
namespace std {
template void _Rb_tree<long, pair<const long, CCharString>, _Select1st<pair<const long, CCharString> >, less<long>, allocator<pair<const long, CCharString> > >::clear();
}
