// fill<CElement68_00719e80*> -- assign value to every element of [first, last) through CElement68_00719e80::operator= @ 0x00719E80
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071A030, oracle length 37 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,edx ; mov esi,ecx ; cmp esi,edi ; je E ; push ebx ; mov ebx,[value] ; L: push ebx ; mov ecx,esi ; call CElement68_00719e80::operator= ; add esi,0x44 ; cmp esi,edi ; jne L ; pop ebx ; E: pop edi ; pop esi ; ret 4
// `fill(first, last, value)` (`__fastcall`: first in ecx, last in edx, the value reference on
// the stack) for 68-byte elements with an out-of-line copy-assignment @ 0x00719E80.
// One of 12 bodies of this shape.
struct CElement68_00719e80 {
    unsigned char _body[0x44];
    CElement68_00719e80& operator=(const CElement68_00719e80& other);   // 0x00719E80
};

void __fastcall Fill_CElement68_00719e80_0071a030(CElement68_00719e80* first, CElement68_00719e80* last, const CElement68_00719e80& value)
{
    for (; first != last; ++first)
        *first = value;
}
