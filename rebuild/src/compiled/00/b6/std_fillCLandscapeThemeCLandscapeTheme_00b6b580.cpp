// _Fill<SElement56*> -- assign value to every element of [first, last) (56-byte POD elements)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6B580, oracle length 40 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  cmp ecx,edx ; je E ; mov eax,[value] ; push esi ; L: (copy 56 bytes from [eax] to [ecx]) ; add ecx,0x38 ; cmp ecx,edx ; jne L ; pop esi ; E: ret 4
// Dinkumware `_Fill` (`__fastcall`: first in ecx, last in edx, the value reference on the
// stack) for 56-byte trivially-copyable elements. One of 10 bodies of this shape.
struct SElement56 {
    unsigned char _body[0x38];
};

void __fastcall Fill_SElement56_00b6b580(SElement56* first, SElement56* last, const SElement56& value)
{
    for (; first != last; ++first)
        *first = value;
}