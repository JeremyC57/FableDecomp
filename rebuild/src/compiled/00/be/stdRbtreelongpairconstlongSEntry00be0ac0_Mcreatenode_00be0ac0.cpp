// std::_Rb_tree<long, pair<const long, SEntry_00be0ac0>, ...>::_M_create_node(const value_type&) -- malloc a 0x24-byte tree node and copy-construct the {key, entry} pair into it
// Leaf: _M_create_node
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE0AC0, 64 bytes, `__fastcall`, `ret 4`. Callee: malloc import @ 0x00BFEA0E (`add esp,4`:
//   not a size TU).
// Identity: the propagated label `CPredicate_And<NScript::CIsThingAlive...>::Clone` is wrong. The only caller is the tree's
//   _M_insert (0x00BE0B4B), which compares the long key at node +0x10 (`cmp eax,[ebx+10h] ; jl`); STLport _M_create_node
//   = allocate(1) through malloc, then _Construct = placement new (the `test ecx,ecx ; je` null test on node +0x10).
// Layout: STLport _Rb_tree_node_base (color, parent, left, right) then the value at +0x10: a long key and a 16-byte
//   entry {long, bool, long, intrusive pointer}. The pair's implicit copy ctor copies the members in order (the bool as a
//   byte), and the pointer's user copy ctor stores the pointer before bumping the pointee's count (`inc dword ptr [edx]`).
//   The entry's member names are descriptive; its real type is not recoverable from this body.
#include <stdlib.h>
#include <new>

struct CRefCountedObject_00be0ac0 {
    long m_refCount;                                     // +0x00
};
struct CRefPointer_00be0ac0 {
    CRefCountedObject_00be0ac0* m_pObject;
    CRefPointer_00be0ac0(const CRefPointer_00be0ac0& other) : m_pObject(other.m_pObject)
    {
        if (m_pObject)
            ++m_pObject->m_refCount;
    }
};
struct SEntry_00be0ac0 {                                 // 0x10 bytes
    long                 m_value;                        // +0x00
    bool                 m_flag;                         // +0x04
    long                 m_data;                         // +0x08
    CRefPointer_00be0ac0 m_pObject;                      // +0x0C
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> struct less {};
template <class T> class allocator {};
template <class V> struct _Select1st {};
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0C
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    _Rb_tree_node_base* _M_header;
    size_t              _M_node_count;
    _Link_type _M_create_node(const V& x)
    {
        _Link_type tmp = (_Link_type)malloc(sizeof(_Rb_tree_node<V>));
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
};
}

typedef std::pair<const long, SEntry_00be0ac0> TValue_00be0ac0;
template std::_Rb_tree<long, TValue_00be0ac0, std::_Select1st<TValue_00be0ac0>, std::less<long>, std::allocator<TValue_00be0ac0> >::_Link_type
    std::_Rb_tree<long, TValue_00be0ac0, std::_Select1st<TValue_00be0ac0>, std::less<long>, std::allocator<TValue_00be0ac0> >::_M_create_node(const TValue_00be0ac0&);