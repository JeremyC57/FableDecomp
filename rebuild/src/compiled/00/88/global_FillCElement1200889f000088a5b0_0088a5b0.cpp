// fill<CElement12_00889f00*> -- assign value to every element of [first, last) through CElement12_00889f00::operator= @ 0x00889F00
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088A5B0, oracle length 37 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,edx ; mov esi,ecx ; cmp esi,edi ; je E ; push ebx ; mov ebx,[value] ; L: push ebx ; mov ecx,esi ; call CElement12_00889f00::operator= ; add esi,0xc ; cmp esi,edi ; jne L ; pop ebx ; E: pop edi ; pop esi ; ret 4
// `fill(first, last, value)` (`__fastcall`: first in ecx, last in edx, the value reference on
// the stack) for 12-byte elements with an out-of-line copy-assignment @ 0x00889F00.
// One of 12 bodies of this shape.
struct CElement12_00889f00 {
    unsigned char _body[0xc];
    CElement12_00889f00& operator=(const CElement12_00889f00& other);   // 0x00889F00
};

void __fastcall Fill_CElement12_00889f00_0088a5b0(CElement12_00889f00* first, CElement12_00889f00* last, const CElement12_00889f00& value)
{
    for (; first != last; ++first)
        *first = value;
}