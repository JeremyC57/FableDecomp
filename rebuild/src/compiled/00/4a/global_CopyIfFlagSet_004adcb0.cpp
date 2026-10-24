// CopyIfFlagSet(list<CThing*>::iterator first, list<CThing*>::iterator last, back_insert_iterator<vector<CThing*> > result) -- copy every listed thing whose byte flag at +0xD4 is set onto the back of a vector of thing pointers; returns the output iterator
// Flags: /Gr
// Leaf: CopyIfFlagSet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ADCB0, 119 bytes (128 with padding), `ret 0Ch` (/Gr __fastcall: the
//   hidden back_insert_iterator result in ecx -> ebx; the two list iterators and the output iterator by value on the
//   stack, edx unused).
// Callees: vector<CThing*>::_M_insert_overflow(pos, x, __true_type, 1, true) 0x0048D0D0 (the push_back growth path; a
//   same-TU stand-in body precedes the function, see Shape).
// Caller: CWorld::LoadHSP 0x004A0940 (`f(list.begin(), list.end(), back_inserter(things))` on the list at +0x8C of the
//   object behind the world's slot-4 accessor, then a find_if by script name 0x0048CC60 with the member predicate
//   0x0048BC70).
// Layout: STLport list nodes {_M_next +0, _M_prev +4, _M_data +8}; STLport vector {_M_start, _M_finish,
//   _M_end_of_storage}; the tested byte is a bool at +0xD4 of the listed object.
// Shape: STLport-style input-iterator copy loop (`for ( ; first != last; ++first)`), the element read into a named
//   local before the flag test (homed in the dead output-iterator argument slot because push_back takes it by const
//   reference), then the inline back_insert_iterator `*result++ = thing` -> STLport `push_back` (`if (_M_finish !=
//   _M_end_of_storage._M_data) { _Construct(_M_finish, x); ++_M_finish; } else _M_insert_overflow(_M_finish, x,
//   __true_type(), 1UL, true);`). The empty `__true_type()` temporary shares x's live slot only when the growth member
//   is visible and never touches the tag (GOTCHAS retry 10f): a same-TU `auto_inline(off)` body of the POD
//   `_M_insert_overflow` stands in for the retail COMDAT.
// Identity: no PDB name set carries a three-argument list -> back_insert_iterator<vector<CThing*>> filter, so the
//   name is descriptive (review at landing). Catalogued as `Vector_InsertFiltered` (right idea).
#include <stdlib.h>                                   // malloc @ 0x00BFEA0E, free @ 0x00BFEA14
#include <string.h>                                   // memmove @ 0x00BFEAE6
#include <new>

struct __true_type { __true_type() { } };              // user constructor: no zero-initialised tag temporary

class CThing {
public:
    unsigned char _pad00[0xD4];
    bool          Flag;                               // +0xD4 (the tested byte)
};

struct _List_node_base {
    _List_node_base* _M_next;                         // +0x00
    _List_node_base* _M_prev;                         // +0x04
};
struct _ThingListNode : _List_node_base {
    CThing* _M_data;                                  // +0x08
};
struct _ThingListIterator {                           // STLport _List_iterator<CThing*, _Nonconst_traits<CThing*> >
    _List_node_base* _M_node;
    CThing*& operator*() const { return ((_ThingListNode*)_M_node)->_M_data; }
    _ThingListIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _ThingListIterator& __x) const { return _M_node != __x._M_node; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};
template <class _Tp> struct allocator { allocator() { } };

struct CThingPtrVector {                              // STLport vector<CThing*>
    CThing** _M_start;                                // +0x00
    CThing** _M_finish;                               // +0x04
    _STLP_alloc_proxy<CThing**, CThing*, allocator<CThing*> > _M_end_of_storage;   // +0x08
    void _M_insert_overflow(CThing** __position, CThing* const& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x0048D0D0
    void push_back(CThing* const& __x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            new (_M_finish) CThing*(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
};

#pragma auto_inline(off)
// Same-TU stand-in for the STLport POD growth COMDAT 0x0048D0D0: reads x through its reference, never the tag.
void CThingPtrVector::_M_insert_overflow(CThing** __position, CThing* const& __x, const __true_type&, unsigned int __fill_len, bool __atend)
{
    unsigned int __old_size = unsigned int(_M_finish - _M_start);
    unsigned int __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    CThing** __new_start = __len != 0 ? (CThing**)malloc(__len * sizeof(CThing*)) : 0;
    CThing** __new_finish = __new_start;
    unsigned int __head = unsigned int(__position - _M_start);
    if (__head) { memmove(__new_finish, _M_start, __head * sizeof(CThing*)); __new_finish += __head; }
    for ( ; __fill_len > 0; --__fill_len, ++__new_finish)
        *__new_finish = __x;
    if (!__atend) {
        unsigned int __tail = unsigned int(_M_finish - __position);
        if (__tail) { memmove(__new_finish, __position, __tail * sizeof(CThing*)); __new_finish += __tail; }
    }
    if (_M_start) free(_M_start);
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage._M_data = __new_start + __len;
}
#pragma auto_inline(on)

struct _ThingBackInsertIterator {                     // STLport back_insert_iterator<vector<CThing*> >
    CThingPtrVector* container;
    explicit _ThingBackInsertIterator(CThingPtrVector& __x) : container(&__x) { }
    _ThingBackInsertIterator& operator=(CThing* const& __value) { container->push_back(__value); return *this; }
    _ThingBackInsertIterator& operator*() { return *this; }
    _ThingBackInsertIterator& operator++() { return *this; }
    _ThingBackInsertIterator& operator++(int) { return *this; }
};

template <class _InputIter, class _OutputIter>
_OutputIter CopyIfFlagSet(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    for ( ; __first != __last; ++__first) {
        CThing* __thing = *__first;
        if (__thing->Flag)
            *__result++ = __thing;
    }
    return __result;
}

template _ThingBackInsertIterator CopyIfFlagSet<_ThingListIterator, _ThingBackInsertIterator>(_ThingListIterator, _ThingListIterator, _ThingBackInsertIterator);