// _Fill<SElement8*> -- assign value to every element of [first, last) (8-byte POD elements)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B15A0, oracle length 37 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  cmp ecx,edx ; je E ; mov eax,[value] ; push esi ; L: (copy 8 bytes from [eax] to [ecx]) ; add ecx,0x8 ; cmp ecx,edx ; jne L ; pop esi ; E: ret 4
// Dinkumware `_Fill` (`__fastcall`: first in ecx, last in edx, the value reference on the
// stack) for 8-byte trivially-copyable elements. One of 14 bodies of this shape.
struct SElement8 {
    unsigned char _body[0x8];
};

void __fastcall Fill_SElement8_009b15a0(SElement8* first, SElement8* last, const SElement8& value)
{
    for (; first != last; ++first)
        *first = value;
}