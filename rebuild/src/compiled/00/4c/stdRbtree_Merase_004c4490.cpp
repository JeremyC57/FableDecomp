// std::_Rb_tree::_M_erase(_Rb_tree_node<value_type>* x) -- (map<void*, NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry>) STLport 4.6 recursive subtree release: erase the right subtree, release the counted def/effect pointer, free the node, continue left
// Leaf: ?_M_erase@?$_Rb_tree@PAXU?$pair@QAXUCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@@std@@U?$_Select1st@U?$pair@QAXUCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@@std@@@2@U?$less@PAX@2@V?$allocator@U?$pair@QAXUCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@@std@@@2@@std@@QAEXPAU?$_Rb_tree_node@U?$pair@QAXUCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C4490, 75 bytes, thiscall `ret 4`; recursive; speed TU.
// Identity (proven): the destructor of NGraphicAppearance::CTCGraphicAppearance 0x004C5DA0 and the tree's clear 0x004C4CD0 / destructors 0x004C53B0, 0x004C5840 call it: Ego_r.pdb CTCGraphicAppearance's first member (+12) is map<void*, CMeshEffectPriorityEntry>, whose CIVCountedPointer<CEngineMeshEffect> PEffect is the released word at node +0x14.
// Callees: itself (right subtree), the pointee's vtable slot 1 (Destroy) on its last reference, free 0x00BFEA14.
// Shape (family M090, representative 0x004C4490): the landed STLport 4.6 `_tree.c` _M_erase model (0x00AD5240); the value's
//   implicit destructor is the intrusive pointer's `if (p) { if (--p->RefCount == 0) p->Destroy(); p = 0; }` at node +0x14.
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
class CEngineMeshEffect {
public:
    virtual void VirtualSlot00();
    virtual void Destroy();                               // slot 1
    long RefCount;                                        // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CIVCountedPointer {
public:
    ~CIVCountedPointer() { if (Object) { Object->Release(); Object = 0; } }
    T* Object;
};
namespace NGraphicAppearance {
enum EAppearanceMeshEffectPriority { APPEARANCE_MESH_EFFECT_PRIORITY_NONE = 0 };
class CTCGraphicAppearance {
public:
    struct CMeshEffectPriorityEntry {                      // Ego_r.pdb, 12 bytes
    public:
        CIVCountedPointer<CEngineMeshEffect> PEffect;     // +0x0
        EAppearanceMeshEffectPriority Priority;           // +0x4
        bool EffectsCarriedItems;                         // +0x8
        bool EffectsWeapons;                              // +0x9
        bool InheritedFromCarrier;                        // +0xa
    };
};
}
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
typedef pair<void* const, NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry> _V;
template class _Rb_tree<void*, _V, _Select1st<_V>, less<void*>, allocator<_V> >;
}   // namespace std