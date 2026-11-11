// _Uninit_fill_n<T*> -- placement-copy `count` copies of value from first (40-byte block elements), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00613210, oracle length 43 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe R ; mov ecx,[value] ; push regs ; (alignment pad) ; L: test eax,eax ; je N ; <copy 40 bytes [ecx] -> [eax]> ; N: add eax,0x28 ; dec edx ; jne L ; pop regs ; R: ret 4
// Dinkumware `_Uninit_fill_n` for a 40-byte element: each copy is a placement new -- VC emits the
// null check on the placement address -- of the value reference.
// A plain block copied with `rep movsd`.
// The speed TU pads the loop head. Fastcall: first in ecx, count in edx. The manifest label is propagated; the element type is modelled by size. One of 7 bodies of this shape.
#include <new>

struct SElement { unsigned char _body[0x28]; };

SElement* __fastcall UninitFillN_00613210(SElement* first, unsigned long count, const SElement& value)
{
    for (; 0 < count; --count, ++first)
        new (first) SElement(value);
    return first;
}