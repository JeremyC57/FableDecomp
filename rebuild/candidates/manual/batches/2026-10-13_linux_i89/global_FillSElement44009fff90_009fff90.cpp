// _Fill<SElement44*> -- assign value to every element of [first, last) (44-byte POD elements)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FFF90, oracle length 40 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  cmp ecx,edx ; je E ; mov eax,[value] ; push esi ; L: (copy 44 bytes from [eax] to [ecx]) ; add ecx,0x2c ; cmp ecx,edx ; jne L ; pop esi ; E: ret 4
// Dinkumware `_Fill` (`__fastcall`: first in ecx, last in edx, the value reference on the
// stack) for 44-byte trivially-copyable elements. One of 10 bodies of this shape.
struct SElement44 {
    unsigned char _body[0x2c];
};

void __fastcall Fill_SElement44_009fff90(SElement44* first, SElement44* last, const SElement44& value)
{
    for (; first != last; ++first)
        *first = value;
}
