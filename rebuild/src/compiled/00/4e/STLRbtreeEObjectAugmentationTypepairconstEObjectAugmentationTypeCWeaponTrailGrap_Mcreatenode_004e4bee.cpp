// std::_Rb_tree<EObjectAugmentationType,pair<const EObjectAugmentationType,CWeaponTrailGraphicSet>,...>::_M_create_node(const value_type& x) -- STLport map node factory (probably CWeaponDef::WeaponTrails): malloc a 0x1C-byte red-black node and copy-construct the 12-byte {key, trail set} value at +0x10
// Leaf: _M_create_node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E4BEE, 40 bytes, `ret 4` (size-optimised TU: unaligned entry,
//   `pop ecx` after malloc). `this` (the tree) is never read: the node allocator is empty.
// Callers: the tree's `_M_copy` 0x004E4B5E (x2, catalogued as `Tree_CloneRecursive`) and `_M_insert` 0x004E9299 (x2,
//   catalogued as `RBTree::InsertNode`).
// Callee: CRT malloc 0x00BFEA0E.
// Label correction: the catalog's `AllocAndCopyThreeFields` is descriptive. Identity: the node holds a 12-byte value
//   keyed by a signed 4-byte key at +0x10 (`_M_insert` compares with `jl`); the tree's operator= 0x004E4AEB follows a def
//   operator= tail copying CCharStrings up to +0xE0, and Ego_r.pdb's CWeaponDef (the TU's ~CWeaponDef sits at
//   0x004E3D4A) holds `map<EObjectAugmentationType, CWeaponTrailGraphicSet> WeaponTrails` -- the only def-side map with a
//   12-byte value_type of that shape (CWeaponTrailGraphicSet = long Attack, long Knockdown). The alternative
//   pair<const long, pair<long,long>> compiles to the same bytes.
// Shape: STLport `_Link_type _M_create_node(const value_type& x) { _Link_type tmp = _M_header.allocate(1);
//   _Construct(&tmp->_M_value_field, x); return tmp; }` -- `allocate(1)` folds to `malloc(0x1C)`, the placement-new
//   null test is on `&tmp->_M_value_field` (`lea ecx,[eax+10h] ; test ecx,ecx`), the value is STLport pair's
//   memberwise copy (key dword, then the trail set's implicit two-dword copy); allocate / _Construct `__forceinline`
//   under `optimize("s")`.
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(size_t, void* where) { return where; }

namespace std {

enum EObjectAugmentationType { OBJECT_AUGMENTATION_NONE };

struct CWeaponTrailGraphicSet {                 // Ego_r.pdb, 8 bytes
    long Attack;                                // +0x00
    long Knockdown;                             // +0x04
};

template <class T1, class T2> struct pair {     // STLport pair: memberwise copy constructor
    T1 first;
    T2 second;
    __forceinline pair(const pair& p) : first(p.first), second(p.second) { }
};

template <class T> __forceinline void _Construct(T* p, const T& value) { new (p) T(value); }

struct _Rb_tree_node_base {
    bool                _M_color;               // +0x00
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                           // +0x10
};

template <class T> struct allocator {           // STLport allocator<T>: empty
    allocator() { }
    __forceinline T* allocate(size_t n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};

template <class K, class V> struct _Rb_tree {   // STLport _Rb_tree (12 bytes in retail)
    typedef V                 value_type;
    typedef _Rb_tree_node<V>* _Link_type;
    _STLP_alloc_proxy<_Rb_tree_node<V> > _M_header;   // +0x00
    unsigned long                        _M_node_count;   // +0x04
    _Link_type _M_create_node(const value_type& x)
    {
        _Link_type tmp = this->_M_header.allocate(1);
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
};

typedef pair<const EObjectAugmentationType, CWeaponTrailGraphicSet> SWeaponTrailValue;   // 12 bytes
template _Rb_tree_node<SWeaponTrailValue>* _Rb_tree<EObjectAugmentationType, SWeaponTrailValue>::_M_create_node(const SWeaponTrailValue&);

}