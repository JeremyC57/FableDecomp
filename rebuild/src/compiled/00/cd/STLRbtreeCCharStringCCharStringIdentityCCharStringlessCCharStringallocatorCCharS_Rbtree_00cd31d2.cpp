// _Rb_tree<CCharString,...>::~_Rb_tree() -- STLport set<CCharString> tree destructor: clear(), then free the header node
// Leaf: ??1?$_Rb_tree@VCCharString@@V1@U?$_Identity@VCCharString@@@std@@U?$less@VCCharString@@@3@V?$allocator@VCCharString@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD31D2, 23 bytes, thiscall, size TU (pragma optimize s):
//   `push esi ; mov esi,ecx ; call clear ; mov esi,[esi] ; test esi,esi ; je ; push esi ; call free ; pop ecx` --
//   ~_Rb_tree_base's `_M_header.deallocate(_M_header._M_data, 1)` through allocator::deallocate's null test.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 452 / hand452 README: the landed "std::list<T_00cd31d2>::~list (fistp pass), before that CFileInstaller::CActiveFile::OnReadFinished" row is the tree ~_Rb_tree):
//   callees clear 0x00CD2D8B (renamed this pass from CTCCarryable::OnKill), free 0x00BFEA14; callers ~set<CCharString> 0x00CD3659 (a bare `jmp 0x00CD31D2`) and the static set's atexit thunk 0x0122AEA0 (`mov ecx,0x0143E910 ; jmp`).
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
    void _M_erase(_Link_type x);                          // out of line (0x00CD2919 / 0x00CD29F0)
    void clear();
    ~_Rb_tree();
};
}

template <class K, class V, class KoV, class C, class A>
std::_Rb_tree<K, V, KoV, C, A>::~_Rb_tree()            // _tree.h: clear(), then ~_Rb_tree_base frees the header node
{
    clear();
}
namespace std {
template _Rb_tree<CCharString, CCharString, _Identity<CCharString>, less<CCharString>, allocator<CCharString> >::~_Rb_tree();
}
