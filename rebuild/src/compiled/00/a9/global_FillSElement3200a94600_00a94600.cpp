// _Fill<SElement32*> -- assign value to every element of [first, last) (32-byte POD elements)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A94600, oracle length 40 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  cmp ecx,edx ; je E ; mov eax,[value] ; push esi ; L: (copy 32 bytes from [eax] to [ecx]) ; add ecx,0x20 ; cmp ecx,edx ; jne L ; pop esi ; E: ret 4
// Dinkumware `_Fill` (`__fastcall`: first in ecx, last in edx, the value reference on the
// stack) for 32-byte trivially-copyable elements. One of 10 bodies of this shape.
struct SElement32 {
    unsigned char _body[0x20];
};

void __fastcall Fill_SElement32_00a94600(SElement32* first, SElement32* last, const SElement32& value)
{
    for (; first != last; ++first)
        *first = value;
}