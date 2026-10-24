// std::vector<list<CWeaponTrailEntry> >::erase(iterator first, iterator last) -- STLport range erase over a vector of weapon-trail point lists: assign the tail down over the erased range, destroy the now-unused trailing lists and shrink the finish pointer
// Leaf: ?erase@?$vector@V?$list@VCWeaponTrailEntry@CEnginePrimitiveWeaponTrail@@@std@@V?$allocator@V?$list@VCWeaponTrailEntry@CEnginePrimitiveWeaponTrail@@@std@@@2@@std@@QAEPAV?$list@VCWeaponTrailEntry@CEnginePrimitiveWeaponTrail@@@2@PAV32@0@Z
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- same-TU noinline __destroy_aux template body (tag unnamed, never touched) homes the empty __false_type tag over the live first slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00770790, 78 bytes, thiscall `ret 8` (returns `first` in eax).
// Callees: list<CWeaponTrailEntry>::operator=(const list&) @ 0x0076FF90 (thiscall, out of line), __destroy_aux<list*>
//   @ 0x00770350 (this chunk; __fastcall first/last + the tag reference).
// Element: the 4-byte STLport list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> of CTCWeaponTrail::Trail.
// Label correction: catalogued as `LinkedList_RemoveAll_WithShift`; the body is STLport `vector::erase(first, last)`.
// Shape: STLport 4.x `pointer i = __copy_ptrs(last, _M_finish, first, _TrivialAss()); _Destroy(i, _M_finish);
//   _M_finish = i; return first;` -- the non-trivial assignment path is SGI's random-access `__copy` (`for (n = last -
//   first; n > 0; --n) *result++ = *first++`, the source addressed as result + (last - first)), and `_Destroy` calls the
//   out-of-line `__destroy_aux` with an empty tag homed on the `first` argument slot.
namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> class list {
public:
    _List_node_base* _M_node;
    list<_Tp>& operator=(const list<_Tp>& __x);      // 0x0076FF90
    void clear();
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

// Same-TU stand-in for __destroy_aux @ 0x00770350 (template body, tag unnamed and never touched): VC7.1 then homes the
// empty tag over the live `first` slot as retail does (GOTCHAS: empty-tag rule).
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        __first->clear();
}

template <class _Tp> struct __type_traits {
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
};

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    _Trivial_destructor __tag;
    __destroy_aux(__first, __last, __tag);
}

template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, (_ForwardIterator)0);
}

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};

}

class CEnginePrimitiveWeaponTrail {
public:
    class CWeaponTrailEntry;                             // 28 bytes: two C3DVectors and a float
};

template class std::vector<std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> >;