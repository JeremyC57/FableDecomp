// _Fill<SElement112*> -- assign value to every element of [first, last) (112-byte POD elements)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B48EF0, oracle length 40 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  cmp ecx,edx ; je E ; mov eax,[value] ; push esi ; L: (copy 112 bytes from [eax] to [ecx]) ; add ecx,0x70 ; cmp ecx,edx ; jne L ; pop esi ; E: ret 4
// Dinkumware `_Fill` (`__fastcall`: first in ecx, last in edx, the value reference on the
// stack) for 112-byte trivially-copyable elements. One of 10 bodies of this shape.
struct SElement112 {
    unsigned char _body[0x70];
};

void __fastcall Fill_SElement112_00b48ef0(SElement112* first, SElement112* last, const SElement112& value)
{
    for (; first != last; ++first)
        *first = value;
}
