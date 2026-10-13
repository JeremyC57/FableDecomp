// fill<CElement12_00a4a520*> -- assign value to every element of [first, last) through CElement12_00a4a520::operator= @ 0x00A4A520
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4AD80, oracle length 37 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,edx ; mov esi,ecx ; cmp esi,edi ; je E ; push ebx ; mov ebx,[value] ; L: push ebx ; mov ecx,esi ; call CElement12_00a4a520::operator= ; add esi,0xc ; cmp esi,edi ; jne L ; pop ebx ; E: pop edi ; pop esi ; ret 4
// `fill(first, last, value)` (`__fastcall`: first in ecx, last in edx, the value reference on
// the stack) for 12-byte elements with an out-of-line copy-assignment @ 0x00A4A520.
// One of 12 bodies of this shape.
struct CElement12_00a4a520 {
    unsigned char _body[0xc];
    CElement12_00a4a520& operator=(const CElement12_00a4a520& other);   // 0x00A4A520
};

void __fastcall Fill_CElement12_00a4a520_00a4ad80(CElement12_00a4a520* first, CElement12_00a4a520* last, const CElement12_00a4a520& value)
{
    for (; first != last; ++first)
        *first = value;
}
