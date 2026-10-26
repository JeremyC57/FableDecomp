// std::_Rb_tree::_M_erase(_Rb_tree_node<value_type>* x) -- (map<unsigned long, CCountedPointer<C3DAnimation2> >) STLport 4.6 recursive subtree release without rebalancing: for each node erase its right subtree, release the value's counted pointer, free the node and continue with its left child
// Leaf: ?_M_erase@?$_Rb_tree@KU?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@std@@U?$_Select1st@U?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2B9D0, 92 bytes (+ int3 padding to 96), thiscall `ret 4`; recursive
//   (calls itself at 0x00A2B9E6); speed-optimised TU.
// Identity (proven): map<unsigned long, CCountedPointer<C3DAnimation2> >; evidence: its first caller is the C3DAnimationInfo::CAnimationData destructor 0x00A26860 (stores vtable 0x0129CE50, RTTI .?AVCAnimationData@C3DAnimationInfo@@; it releases FileData +0x34 and SharedAnimation +0x2C first), clearing the tree at this+0x1C -- Ego_r.pdb's CAnimationData::Animations (+0x1C), map<unsigned long, CCountedPointer<C3DAnimation2> >
// Label: the manifest's "Utility_DestroyRefcountedTreeNode" is propagated (family H079: six _M_erase bodies whose only non-trivial value member
//   is a CCountedPointer, at node +0x14 (4-byte key) or +0x18 (8-byte key); retail has no identical-code folding, so each
//   address is one instantiation); this is STLport 4.6 `_Rb_tree::_M_erase` (`_tree.c`), as H063.
// Callees: the count block's deleter (`call [info+4]`, the object in ecx) and operator delete 0x00BFE9BC (the count block)
//   from the inline ~CCountedPointer at node +0x14, free 0x00BFEA14 (allocator<_Node>::deallocate).
// Model: H063's _M_erase with the landed inline CCountedPointer release (H067).
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
void operator delete(void* block);                      // 0x00BFE9BC

class CCPPointerInfo { public:                                 // the shared count block
    long RefCount;                                      // +0x0
    void (__fastcall *DeleteFunc)(void*);               // +0x4
    void* Data;                                         // +0x8
};

template <class T> class CCountedPointer {             // 8 bytes: object, shared count block
public:
    T* Data;                                            // +0x0
    CCPPointerInfo* Info;                               // +0x4
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
class C3DAnimation2;

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

template class std::_Rb_tree<unsigned long, std::pair<const unsigned long, CCountedPointer<C3DAnimation2> >, std::_Select1st<std::pair<const unsigned long, CCountedPointer<C3DAnimation2> > >, std::less<unsigned long >, std::allocator<std::pair<const unsigned long, CCountedPointer<C3DAnimation2> > > >;