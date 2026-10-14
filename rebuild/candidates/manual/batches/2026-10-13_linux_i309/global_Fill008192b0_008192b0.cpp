// fill<T*, T> -- assign value over [first, last) (12-byte POD elements)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008192B0, oracle length 49 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  cmp ecx,edx ; je R ; mov eax,[value] ; push regs ; (alignment pad) ; L: <copy 12 bytes [eax] -> [ecx]> ; add ecx,0xc ; cmp ecx,edx ; jne L ; pop regs ; R: ret 4
// `std::fill` over a 12-byte POD element: the value reference is loaded once and assigned to every
// element of the range (dword moves). Fastcall: first in ecx, last in edx, the value on the
// stack. The manifest label is propagated; the element type is modelled by size. One of
// 6 bodies of this shape.
struct SElement { unsigned char _body[0xc]; };

void __fastcall Fill_008192b0(SElement* first, SElement* last, const SElement& value)
{
    for (; first != last; ++first)
        *first = value;
}
