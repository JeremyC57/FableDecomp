// std::_Rb_tree::_M_erase(_Rb_tree_node<value_type>* x) -- (map<unsigned long, CDefPointer<CDefClassBase> >) STLport 4.6 recursive subtree release: erase the right subtree, release the counted def/effect pointer, free the node, continue left
// Leaf: ?_M_erase@?$_Rb_tree@KU?$pair@$$CBKV?$CDefPointer@VCDefClassBase@@@@@std@@U?$_Select1st@U?$pair@$$CBKV?$CDefPointer@VCDefClassBase@@@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKV?$CDefPointer@VCDefClassBase@@@@@std@@@2@@std@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBKV?$CDefPointer@VCDefClassBase@@@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B40F0, 75 bytes, thiscall `ret 4`; recursive; speed TU.
// Identity (proven): CDefinitionManager::~CDefinitionManager 0x009B02E0 and the tree's clear / destructors 0x009B4D20, 0x009B6010, 0x009B6F80 call it: Ego_r.pdb CDefinitionManager::RecompiledDefs is map<unsigned long, CDefPointer<CDefClassBase> > (the def pointer at node +0x14).
// Callees: itself (right subtree), the pointee's vtable slot 1 (Destroy) on its last reference, free 0x00BFEA14.
// Shape (family M090, representative 0x004C4490): the landed STLport 4.6 `_tree.c` _M_erase model (0x00AD5240); the value's
//   implicit destructor is the intrusive pointer's `if (p) { if (--p->RefCount == 0) p->Destroy(); p = 0; }` at node +0x14.
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
class CDefClassBase {
public:
    virtual void VirtualSlot00();
    virtual void Destroy();                               // slot 1
    mutable long RefCount;                                // +0x04
    void Release() const { if (--RefCount == 0) const_cast<CDefClassBase*>(this)->Destroy(); }
};
template <class T> class CDefPointer {
public:
    ~CDefPointer() { if (m_p) { m_p->Release(); m_p = 0; } }
    T* m_p;
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
typedef pair<const unsigned long, CDefPointer<CDefClassBase> > _V;
template class _Rb_tree<unsigned long, _V, _Select1st<_V>, less<unsigned long>, allocator<_V> >;
}   // namespace std