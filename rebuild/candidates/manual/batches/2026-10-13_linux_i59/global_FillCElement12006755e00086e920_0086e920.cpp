// fill<CElement12_006755e0*> -- assign value to every element of [first, last) through CElement12_006755e0::operator= @ 0x006755E0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086E920, oracle length 37 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,edx ; mov esi,ecx ; cmp esi,edi ; je E ; push ebx ; mov ebx,[value] ; L: push ebx ; mov ecx,esi ; call CElement12_006755e0::operator= ; add esi,0xc ; cmp esi,edi ; jne L ; pop ebx ; E: pop edi ; pop esi ; ret 4
// `fill(first, last, value)` (`__fastcall`: first in ecx, last in edx, the value reference on
// the stack) for 12-byte elements with an out-of-line copy-assignment @ 0x006755E0.
// One of 12 bodies of this shape.
struct CElement12_006755e0 {
    unsigned char _body[0xc];
    CElement12_006755e0& operator=(const CElement12_006755e0& other);   // 0x006755E0
};

void __fastcall Fill_CElement12_006755e0_0086e920(CElement12_006755e0* first, CElement12_006755e0* last, const CElement12_006755e0& value)
{
    for (; first != last; ++first)
        *first = value;
}
