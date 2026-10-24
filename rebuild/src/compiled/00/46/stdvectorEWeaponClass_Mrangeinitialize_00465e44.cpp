// std::vector<EWeaponClass>::_M_range_initialize(set<EWeaponClass>::const_iterator first, const_iterator last, const forward_iterator_tag&) -- STLport range init from a set: count the nodes, malloc that many words, copy-construct every node value in order
// Leaf: _M_range_initialize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00465E44, length 106 bytes, `ret 0Ch` (thiscall: first, last, tag).
// Callees: _Rb_global<bool>::_M_increment(node) @ 0x004292C0 (`__cdecl` static; catalog "CMemoryAllocatorVariableSize::
// GetNoAllocatedAreas" is propagated), malloc @ 0x00BFEA0E.
// Label: the catalog "CMemoryAllocatorVariableSize_InitFromRange" is wrong. The single caller, CCombatAbilityBlockDefBase::
// Transfer (0x00465C54), zeroes a local vector (the inlined _Vector_base) and then calls this body with the begin()/end()
// node pointers of the set at +0x48 -- the PDB's CCombatAbilityBlockingData::ValidBlockWeaponTypes, a set<EWeaponClass>
// (def base 0x28 + 0x20) -- and its forward_iterator_tag homed in a dead argument slot (`lea ecx,[ebp+0Bh]`).
// Model: STLport 4.x `_M_range_initialize(first, last, const forward_iterator_tag&)`: `distance` (the input-iterator
// `__distance` walking a copy, `++it; ++n;`), allocate(n), end of storage, then `__uninitialized_copy(..., __false_type)`
// with the placement-new null test (an enum has no type-traits specialisation, so it is not POD for STLport). Tree nodes
// are the STLport _Rb_tree_node: color, parent, left, right, then the value at +0x10.
// Shape: frame pointer, `pop ecx` after the __cdecl calls = size TU with /Oy off: #pragma optimize("y", off) + ("s", on).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

enum EWeaponClass { WEAPON_CLASS_NONE };

namespace std {
struct __false_type { };
struct input_iterator_tag { };
struct forward_iterator_tag : input_iterator_tag { };

struct _Rb_tree_node_base {
    bool                 _M_color;     // +0x00
    _Rb_tree_node_base*  _M_parent;    // +0x04
    _Rb_tree_node_base*  _M_left;      // +0x08
    _Rb_tree_node_base*  _M_right;     // +0x0C
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                  // +0x10
};

template <class Dummy> struct _Rb_global {
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* node);   // 0x004292C0
};

template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    const V& operator*() const { return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& other) const { return _M_node != other._M_node; }
};

template <class InputIter>
__forceinline int __distance(const InputIter& first, const InputIter& last, const input_iterator_tag&)
{
    int n = 0;
    InputIter it(first);
    while (it != last) {
        ++it;
        ++n;
    }
    return n;
}

template <class InputIter> __forceinline int distance(InputIter first, InputIter last)
{
    return __distance(first, last, forward_iterator_tag());
}

template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T> struct allocator {
    __forceinline T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T, class A = allocator<T> > class vector {
public:
    typedef __false_type _IsPODType;
    T* _M_start;
    T* _M_finish;
    struct : A { T* _M_data; } _M_end_of_storage;
    template <class ForwardIter> void _M_range_initialize(ForwardIter first, ForwardIter last, const forward_iterator_tag&);
};

template <class T, class A> template <class ForwardIter>
void vector<T, A>::_M_range_initialize(ForwardIter first, ForwardIter last, const forward_iterator_tag&)
{
    unsigned int n = distance(first, last);
    this->_M_start = this->_M_end_of_storage.allocate(n);
    this->_M_end_of_storage._M_data = this->_M_start + n;
    this->_M_finish = __uninitialized_copy(first, last, this->_M_start, _IsPODType());
}

template void vector<EWeaponClass>::_M_range_initialize<_Rb_tree_iterator<EWeaponClass> >(
    _Rb_tree_iterator<EWeaponClass> first, _Rb_tree_iterator<EWeaponClass> last, const forward_iterator_tag&);
}