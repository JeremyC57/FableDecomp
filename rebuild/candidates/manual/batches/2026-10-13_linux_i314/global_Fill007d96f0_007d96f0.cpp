// fill<T*, T> -- assign value over [first, last) (16-byte POD elements)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D96F0, oracle length 55 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  cmp ecx,edx ; je R ; mov eax,[value] ; push regs ; (alignment pad) ; L: <copy 16 bytes [eax] -> [ecx]> ; add ecx,0x10 ; cmp ecx,edx ; jne L ; pop regs ; R: ret 4
// `std::fill` over a 16-byte POD element: the value reference is loaded once and assigned to every
// element of the range (dword moves). Fastcall: first in ecx, last in edx, the value on the
// stack. The manifest label is propagated; the element type is modelled by size. One of
// 6 bodies of this shape.
struct SElement { unsigned char _body[0x10]; };

void __fastcall Fill_007d96f0(SElement* first, SElement* last, const SElement& value)
{
    for (; first != last; ++first)
        *first = value;
}
