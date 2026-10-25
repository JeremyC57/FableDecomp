// NGraphicAppearance::CTCGraphicAppearance::GetPMeshEffect() -- the mesh effect with the highest priority among the registered ones (a counted copy), or a null pointer when none is registered
// Grade: RELOCATION_MATCH (agent902, 1 grade this pass) -- the parked flat `CMeshEffectMap` (invented iterator with its own _M_increment) coloured header/best as edi/esi; the real STLport 4.6 map<void*, CMeshEffectPriorityEntry> (_Rb_tree with the alloc-proxy header, _Rb_tree_iterator<V, _Nonconst_traits<V> > with its declared copy ctor, postfix ++ through the static _Rb_global<bool>::_M_increment) gives retail's header in esi and the shrink-wrapped best in edi.
// Leaf: GetPMeshEffect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BCF50, length 141 bytes, `ret 4` (hidden CIVCountedPointer result).
// Ego_r.pdb: `?GetPMeshEffect@CTCGraphicAppearance@NGraphicAppearance@@QAE?AV?$CIVCountedPointer@VCEngineMeshEffect@@@@XZ`.
// Callees: the MeshEffects iterator's out-of-line `operator++()` @ 0x004C2560 (thiscall on the iterator, returns it; the
// catalog "RBTree_FindSuccessor"; the hint insert 0x004C5480 calls it too).
// Label: the catalog "Appearance_OnAppearanceDrawHelper" is behavioural.
// Layout: as the landed ClearMeshEffect 0x004BFE20 / SetPMeshEffect 0x004C0240: MeshEffects +0x0c (STLport map, header
// pointer first); a node holds the owner key at +0x10 and CMeshEffectPriorityEntry at +0x14 (PEffect +0, Priority +4).
// Model: map type from struct_layouts_egor.tsv (CTCGraphicAppearance::MeshEffects = map<void*, CMeshEffectPriorityEntry>).
// Shape: `it = begin(); if (it == end()) return 0; best = &it->second; ++it` (the prefix operator out of line, so the
// iterator lives in the frame) then `for (; it != end(); it++)` with the postfix increment's _M_increment inlined; a
// later entry wins only on a strictly greater priority (`jle`); the result is the counted copy of best->PEffect
// (null test + `inc [p+4]`).
class CEngineMeshEffect {
public:
    virtual void v000();
    long RefCount;                                          // +0x04
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    __forceinline CIVCountedPointer() : Object(0) {}
    __forceinline CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
};

namespace _STL {
template <class _Tp> struct less { };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value, class _Traits>
struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef typename _Traits::pointer pointer;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }
    pointer operator->() const { return &((_Link_type)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++();                        // 0x004C2560 (out of line in this TU)
    _Rb_tree_iterator operator++(int)
    {
        _Rb_tree_iterator __tmp = *this;
        _M_node = _Rb_global_inst::_M_increment(_M_node);
        return __tmp;
    }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                       // +0x04
    _Compare _M_key_compare;                                          // +0x08
    _Node* _M_leftmost() const { return (_Node*)_M_header._M_data->_M_left; }
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(_M_header._M_data); }
};
template <class _Pair> struct _Select1st { };

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
};
}

namespace NGraphicAppearance {

struct CTCGraphicAppearance {
    struct CMeshEffectPriorityEntry {
        CIVCountedPointer<CEngineMeshEffect> PEffect;      // +0x0
        long          Priority;                             // +0x4 EAppearanceMeshEffectPriority
        bool          EffectsCarriedItems;                  // +0x8
        bool          EffectsWeapons;                       // +0x9
        bool          InheritedFromCarrier;                 // +0xa
    };
    typedef _STL::map<void*, CMeshEffectPriorityEntry> CMeshEffectMap;

    void*          __vftable;                               // +0x00 (CTCBase)
    void*          ParentThing;                             // +0x04
    unsigned char  _pad08[0x0c - 0x08];
    CMeshEffectMap MeshEffects;                             // +0x0c
    CIVCountedPointer<CEngineMeshEffect> GetPMeshEffect();
};

CIVCountedPointer<CEngineMeshEffect> CTCGraphicAppearance::GetPMeshEffect()
{
    CMeshEffectMap::iterator it = MeshEffects.begin();
    CMeshEffectMap::iterator last = MeshEffects.end();
    if (it == last)
        return CIVCountedPointer<CEngineMeshEffect>();
    CMeshEffectPriorityEntry* best = &it->second;
    ++it;
    for (CMeshEffectMap::iterator i = it; i != last; i++)
        if (i->second.Priority > best->Priority)
            best = &i->second;
    return best->PEffect;
}

}