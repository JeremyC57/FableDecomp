// fill<CElement12_0075b8d0*> -- assign value to every element of [first, last) through CElement12_0075b8d0::operator= @ 0x0075B8D0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCD8F0, oracle length 37 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,edx ; mov esi,ecx ; cmp esi,edi ; je E ; push ebx ; mov ebx,[value] ; L: push ebx ; mov ecx,esi ; call CElement12_0075b8d0::operator= ; add esi,0xc ; cmp esi,edi ; jne L ; pop ebx ; E: pop edi ; pop esi ; ret 4
// `fill(first, last, value)` (`__fastcall`: first in ecx, last in edx, the value reference on
// the stack) for 12-byte elements with an out-of-line copy-assignment @ 0x0075B8D0.
// One of 12 bodies of this shape.
struct CElement12_0075b8d0 {
    unsigned char _body[0xc];
    CElement12_0075b8d0& operator=(const CElement12_0075b8d0& other);   // 0x0075B8D0
};

void __fastcall Fill_CElement12_0075b8d0_00bcd8f0(CElement12_0075b8d0* first, CElement12_0075b8d0* last, const CElement12_0075b8d0& value)
{
    for (; first != last; ++first)
        *first = value;
}
