// std::find<long*> -- Dinkumware `_Find`: four-way unrolled scan with a switch on the remainder (4-byte elements)
// Leaf: __find
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A7D0E0, oracle length 112 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,ecx ; mov ecx,edx ; push ebx ; sub ecx,eax ; push esi ; sar ecx,4 ; test ecx,ecx ; push edi ; mov edi,[esp+10h] ; jle T ; mov esi,[edi] ; L: cmp [eax],esi ; je F ; (three more compare/advance steps) ; add eax,4 ; dec ecx ; test ecx,ecx ; jg L ; T: mov ecx,edx ; sub ecx,eax ; sar ecx,2 ; dec ecx ; je C1 ; dec ecx ; je C2 ; dec ecx ; jne D ; (case 3, 2, 1 compares) ; D: mov eax,edx ; F: pop edi ; pop esi ; pop ebx ; ret 8
// The retail Dinkumware `find` for 4-byte scalar elements (__fastcall: first in ecx, last in
// edx, the value by reference on the stack; `ret 8` also pops an unused tag): a loop over groups
// of four comparisons (the value cached in a register) followed by a `switch` on the 0..3
// remaining elements. Written out by hand because the DDK headers are a different revision.
// One of 23 bodies of this shape.
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&)   // STLport 4.6 _algobase.c
{
    long __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}
}
template long* __fastcall std::__find<long*, long>(long*, long*, const long&, const std::random_access_iterator_tag&);