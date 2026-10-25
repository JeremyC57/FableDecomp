// _STL::map::operator[](void* const& k) -- STLport subscript of CTCGraphicAppearance's map<void*, CMeshEffectPriorityEntry>: lower_bound the key, insert {key, CMeshEffectPriorityEntry()} at that hint when absent, return the entry
// Grade: DIFFER(170v161) -- everything up to the insert call matches (incl. the overlaid default/value_type copy); the value_type's counted-pointer release differs: retail `mov ecx,[esp+10h] ; test ecx,ecx ; je ; dec dword [ecx+4] ; jne ; mov eax,[ecx] ; call [eax+4]`, ours loads the pointer into eax, `dec [eax+4]`, then re-reads the pointer and the count (`mov ecx,[esp+10h] ; mov eax,[ecx+4] ; test eax,eax`).
// Leaf: ??A?$map@PAXUCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C5B90, 161 bytes, __thiscall `ret 4`.
// Identity: only caller NGraphicAppearance::CTCGraphicAppearance::SetPMeshEffect (0x004C0240); the 12-byte entry is the
//   Ego_r.pdb CMeshEffectPriorityEntry {CIVCountedPointer<CEngineMeshEffect> PEffect; EAppearanceMeshEffectPriority
//   Priority; bool EffectsCarriedItems, EffectsWeapons, InheritedFromCarrier}, as landed CTCGraphicAppearance::
//   ClearMeshEffect 0x004BFE20 models it. The manifest type agrees.
// Callees: insert_unique(iterator, const value_type&) @ 0x004C5480 (the hint insert; `this` still in ecx, iterator built
//   in its slot, result through the dead `k` slot); the effect's virtual slot 1 in the value_type's counted-pointer release.
// Shape: STLport 4.6 map::operator[] with _M_lower_bound inlined (the landed twin 0x009913D0 template, unsigned pointer
//   compares); the entry's implicit default ctor only nulls PEffect, and VC7.1 overlays the default temporary with the
//   value_type temporary, so the implicit memberwise copy reads the (uninitialised) priority and flags back from the
//   just-written words (`mov eax,[esp+10h] ; mov [esp+14h],eax`, then the three bool bytes); the default's counted-pointer
//   release folds away and only the value_type's PEffect is released.
#include <stddef.h>

class CEngineMeshEffect {
public:
    virtual void v000();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(const CIVCountedPointer& o) : Object(o.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CIVCountedPointer()
    {
        if (Object) {
            if (--Object->RefCount == 0)
                Object->Destroy();
        }
    }
};

enum EAppearanceMeshEffectPriority { MESH_EFFECT_PRIORITY_NONE };

namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    struct CMeshEffectPriorityEntry {           // 12 bytes (Ego_r.pdb)
        CIVCountedPointer<CEngineMeshEffect> PEffect;               // +0x0
        EAppearanceMeshEffectPriority        Priority;              // +0x4
        bool                                 EffectsCarriedItems;   // +0x8
        bool                                 EffectsWeapons;        // +0x9
        bool                                 InheritedFromCarrier;  // +0xa
    };
};
}

namespace _STL {

template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template <class T> class allocator { };

template <class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};

template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;                       // +0x10
};

template <class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) { }
    Value& operator*() const { return static_cast<_Rb_tree_node<Value>*>(_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& other) const { return _M_node == other._M_node; }
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;              // +0x00
    size_t _M_node_count;                       // +0x04
    Compare _M_key_compare;                     // +0x08

    static const Key& _S_key(_Rb_tree_node_base* x) { return KeyOfValue()(((_Node*)x)->_M_value_field); }
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    iterator end() { return iterator(_M_header); }
    Compare key_comp() const { return _M_key_compare; }
    iterator lower_bound(const Key& k)
    {
        _Rb_tree_node_base* y = _M_header;      // last node which is not less than k
        _Rb_tree_node_base* x = _M_root();      // current node
        while (x != 0)
            if (!_M_key_compare(_S_key(x), k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return iterator(y);
    }
    iterator insert_unique(iterator position, const Value& v);   // 0x004C5480
};

template <class Key, class T, class Compare = less<Key>, class Alloc = allocator<pair<const Key, T> > > class map {
public:
    typedef Key key_type;
    typedef pair<const Key, T> value_type;
    typedef _Rb_tree<Key, value_type, _Select1st<value_type>, Compare, Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;

    iterator lower_bound(const key_type& k) { return _M_t.lower_bound(k); }
    iterator end() { return _M_t.end(); }
    Compare key_comp() const { return _M_t.key_comp(); }
    iterator insert(iterator position, const value_type& v) { return _M_t.insert_unique(position, v); }
    T& operator[](const key_type& k)
    {
        iterator i = lower_bound(k);
        // i->first is greater than or equivalent to k.
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }
};

}   // namespace _STL

template NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry& _STL::map<void*, NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry>::operator[](void* const&);
