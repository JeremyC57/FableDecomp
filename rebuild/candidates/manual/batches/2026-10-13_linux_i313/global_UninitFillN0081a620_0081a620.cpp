// _Uninit_fill_n<T*> -- placement-copy `count` copies of value from first (8-byte POD elements), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081A620, oracle length 40 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe R ; mov ecx,[value] ; push regs ; (alignment pad) ; L: test eax,eax ; je N ; <copy 8 bytes [ecx] -> [eax]> ; N: add eax,0x8 ; dec edx ; jne L ; pop regs ; R: ret 4
// Dinkumware `_Uninit_fill_n` for a 8-byte element: each copy is a placement new -- VC emits the
// null check on the placement address -- of the value reference.
// A POD of dwords copied member by member (three registers when VC splits the base pointers).
// The speed TU pads the loop head. Fastcall: first in ecx, count in edx. The manifest label is propagated; the element type is modelled by size. One of 6 bodies of this shape.
#include <new>

struct SElement { long m_0; long m_4; };

SElement* __fastcall UninitFillN_0081a620(SElement* first, unsigned long count, const SElement& value)
{
    for (; 0 < count; --count, ++first)
        new (first) SElement(value);
    return first;
}
