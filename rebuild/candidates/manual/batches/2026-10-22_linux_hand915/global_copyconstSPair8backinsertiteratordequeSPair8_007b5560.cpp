// _global::copy<const SPair8*, back_insert_iterator<deque<SPair8> > >(const SPair8*, const SPair8*, back_insert_iterator<deque<SPair8> >) -- STLport copy into a deque back-inserter: dispatch to the random-access __copy
// Leaf: ?copy@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B5560, 33 bytes, __fastcall: the hidden result pointer in ecx (returned in
//   eax), first in edx (passed straight through), last / result on the stack (`ret 8`). Catalogued FUN_007b5560
//   (unreferenced twin of the __copy_aux 0x007B5530, here with the __copy_aux dispatch inlined). Callee:
//   __copy(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0) @ 0x007B5400 (the inlined
//   deque::push_back of 8-byte elements).
// Shape: the empty tag temporaries have user constructors (no zero byte store) and the random_access_iterator_tag
//   is homed in the dead `result` argument slot.
struct SPair8 { long first; long second; };      // the 8-byte element
struct deque_SPair8;                             // std::deque<SPair8>

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

struct back_insert_iterator_deque {              // std::back_insert_iterator<deque<SPair8> >
    deque_SPair8* container;
    explicit back_insert_iterator_deque(deque_SPair8& x) : container(&x) { }
};

back_insert_iterator_deque __fastcall __copy(const SPair8* __first, const SPair8* __last, back_insert_iterator_deque __result,
                                             const random_access_iterator_tag&, long*);   // 0x007B5400

__forceinline back_insert_iterator_deque __fastcall __copy_aux(const SPair8* __first, const SPair8* __last, back_insert_iterator_deque __result,
                                                        const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (long*)0);
}

back_insert_iterator_deque __fastcall copy(const SPair8* __first, const SPair8* __last, back_insert_iterator_deque __result)
{
    return __copy_aux(__first, __last, __result, __false_type());
}
