// _global::__copy_aux<const SPair8*, back_insert_iterator<deque<SPair8> > >(const SPair8*, const SPair8*, back_insert_iterator<deque<SPair8> >, const __false_type&) -- STLport copy dispatch: forward to the random-access __copy
// Leaf: __copy_aux
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B5530, 33 bytes, __fastcall: the hidden result pointer in ecx (returned in
//   eax), first in edx (passed straight through), last / result / the __false_type tag on the stack (`ret 0Ch`).
//   Catalogued FUN_007b5530 (unreferenced). Callee: __copy(first, last, result, random_access_iterator_tag(),
//   (ptrdiff_t*)0) @ 0x007B5400 (manifest Vector_InsertRange_8byte_StdVector), whose loop is the inlined
//   deque::push_back of 8-byte elements (`_M_finish._M_cur` +0x10, `_M_finish._M_last` +0x18, overflow
//   _M_push_back_aux_v 0x007B4F70). Its twin 0x007B5560 is copy() itself.
// Shape: the empty random_access_iterator_tag temporary is homed in the dead `result` argument slot
//   (`lea eax,[esp+10h]` after `result` was read into ecx).
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

back_insert_iterator_deque __fastcall __copy_aux(const SPair8* __first, const SPair8* __last, back_insert_iterator_deque __result,
                                                 const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (long*)0);
}