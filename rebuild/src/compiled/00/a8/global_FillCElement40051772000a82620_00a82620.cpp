// fill<CElement4_00517720*> -- assign value to every element of [first, last) through CElement4_00517720::operator= @ 0x00517720
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A82620, oracle length 37 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,edx ; mov esi,ecx ; cmp esi,edi ; je E ; push ebx ; mov ebx,[value] ; L: push ebx ; mov ecx,esi ; call CElement4_00517720::operator= ; add esi,0x4 ; cmp esi,edi ; jne L ; pop ebx ; E: pop edi ; pop esi ; ret 4
// `fill(first, last, value)` (`__fastcall`: first in ecx, last in edx, the value reference on
// the stack) for 4-byte elements with an out-of-line copy-assignment @ 0x00517720.
// One of 12 bodies of this shape.
struct CElement4_00517720 {
    unsigned char _body[0x4];
    CElement4_00517720& operator=(const CElement4_00517720& other);   // 0x00517720
};

void __fastcall Fill_CElement4_00517720_00a82620(CElement4_00517720* first, CElement4_00517720* last, const CElement4_00517720& value)
{
    for (; first != last; ++first)
        *first = value;
}