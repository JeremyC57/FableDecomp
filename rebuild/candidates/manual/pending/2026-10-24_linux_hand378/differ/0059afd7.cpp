// std::__copy<_Deque_iterator<CComponent*>, _Deque_iterator<CComponent*>, int>(first, last, result, const random_access_iterator_tag&, int*) -- counted forward copy between two deque ranges of screen components
// Grade: DIFFER(73v73) -- scheduling only: in `*__result = *__first` retail loads the source element (`mov eax,[eax]`) before the destination cursor (`mov ecx,[ebp+28h]`); this loads the cursor first. Everything else (ecx hidden pointer, the movsd result copy, the slots) matches.
// Leaf: __copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059AFD7, 73 bytes, `ret 34h` (`/Gr` `__fastcall`: the hidden result pointer in
// ecx, the tag reference in edx; the three 16-byte iterators and the null distance pointer on the stack).
// Callees: _Deque_iterator::operator-(const _Deque_iterator&) const @ 0x0059A8F3 (the element count, landed with the
// CFrontEndManager deque code), _Deque_iterator::operator++() @ 0x0059AAF9 (out of line here).
// Only caller 0x0059B2FD (the CFrontEndManager-TU deque<NUISystem::CComponent*> code, m_stackScreens).
// Label: the catalog "ProcessItemStats" is wrong: the body is STLport 4.x `__copy` for random-access iterators
// verbatim: `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first; ++__result; }
// return __result;` -- the iterators stay in their argument slots (each `++` a thiscall on the slot), the element
// is one pointer store, and the result is copied out with four `movsd`. The iterator needs STLport's user
// constructors (cursor/node and default) but an implicit copy: as a POD the hidden result pointer would be pushed
// on the stack instead of travelling in ecx, and a user memberwise copy ctor is either called (size TU) or, forced
// inline, emitted as four `mov` pairs instead of the block copy.
// Shape: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace NUISystem {
class CComponent;
}

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class T> struct _Deque_iterator {
    T*  _M_cur;                                      // +0x00
    T*  _M_first;                                    // +0x04
    T*  _M_last;                                     // +0x08
    T** _M_node;                                     // +0x0c
    _Deque_iterator(T* __x, T** __y) : _M_cur(__x), _M_first(*__y), _M_last(*__y + 32), _M_node(__y) {}
    _Deque_iterator() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) {}
    int operator-(const _Deque_iterator& x) const;   // 0x0059A8F3
    _Deque_iterator& operator++();                   // 0x0059AAF9
    T& operator*() const { return *_M_cur; }
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

typedef _Deque_iterator<NUISystem::CComponent*> CComponentDequeIter;
template CComponentDequeIter __fastcall __copy(CComponentDequeIter, CComponentDequeIter, CComponentDequeIter,
                                               const random_access_iterator_tag&, int*);
}
