// _Rb_tree::_M_erase(_Rb_tree_node* x) -- STLport red-black tree erase without rebalancing for map<long, CCharString>: free the subtree under x (right subtrees recursively, left children iteratively), destroying each value
#pragma optimize("s", on)
#pragma inline_recursion(on)
// Leaf: ?_M_erase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD29F0, 129 bytes, `__fastcall` (tree in ecx, the node on the stack: `ret 4`);
//   callers: its clear() @ 0x00CD2DDC (landed as CTCCarryable::OnKill_00cd2ddc), the tree destructor @ 0x00CD3211.
// Label: the catalog name "CLandscapeBackgroundPatch_DestroyRecursiveTree" is descriptive; the body is the STLport `_Rb_tree<...>::_M_erase` instantiation
//   (node: colour +0, parent +4, left +8, right +0xC, value +0x10).
// Callees: itself (the innermost right subtree), the pair's scalar deleting destructor @ 0x00CD22C0 called with 0 (catalogued as CLandscapeBackgroundPatch::`vector deleting destructor'), free @ 0x00BFEA14 (the nodes).
// Value: its pair<const long, CCharString> has an implicit destructor that VC7.1 reaches through `??_G(0)` (`push 0 ; call`) for a trivial first member; the ??_G destroys the string at +4.
// Shape: the STLport 4.6 `_Rb_tree::_M_erase` (_tree.c) as written, in a size TU with `#pragma inline_recursion(on)`:
//   VC7.1 expands the right-subtree recursion twice inside the out-of-line copy (three nested loops; the innermost
//   level calls this function). The expansion count is set by the inline budget: with the real allocator chain
//   (`_STLP_alloc_proxy::deallocate` -> `allocator::deallocate` (null test, folded) -> `__malloc_alloc::deallocate`
//   -> free) the recursion stops at two levels; a direct `free(x)` lets it expand three times (180 v 123), and
//   `inline_depth(2)` stops the helpers inlining at the innermost level. `this` is spilled to [ebp-4] for the
//   recursive calls and the left child is parked in the dead argument slot [ebp+8]; the frame comes from the size TU.
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
template <class T> inline void _Destroy(T* p) { p->~T(); }
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
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class K, class V, class KoV, class C, class A>
class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    unsigned int _M_node_count;
    C            _M_key_compare;
    static _Link_type& _S_left(_Link_type x) { return (_Link_type&)(x->_M_left); }
    static _Link_type& _S_right(_Link_type x) { return (_Link_type&)(x->_M_right); }
private:
    void _M_erase(_Link_type x);
};
template <class K, class V, class KoV, class C, class A>
void _Rb_tree<K, V, KoV, C, A>::_M_erase(_Link_type x)    // erase without rebalancing (_tree.c)
{
    while (x != 0) {
        _M_erase(_S_right(x));
        _Link_type y = _S_left(x);
        _Destroy(&x->_M_value_field);
        this->_M_header.deallocate(x, 1);
        x = y;
    }
}
template class _Rb_tree<long, pair<const long, CCharString>, _Select1st<pair<const long, CCharString> >, less<long>, allocator<pair<const long, CCharString> > >;
}