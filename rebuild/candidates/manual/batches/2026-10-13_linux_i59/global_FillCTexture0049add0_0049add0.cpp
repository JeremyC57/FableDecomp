// fill<CTexture*> -- assign value to every element of [first, last) through CTexture::operator= @ 0x009FA1C0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049ADD0, oracle length 37 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,edx ; mov esi,ecx ; cmp esi,edi ; je E ; push ebx ; mov ebx,[value] ; L: push ebx ; mov ecx,esi ; call CTexture::operator= ; add esi,0x8 ; cmp esi,edi ; jne L ; pop ebx ; E: pop edi ; pop esi ; ret 4
// `fill(first, last, value)` (`__fastcall`: first in ecx, last in edx, the value reference on
// the stack) for 8-byte elements with an out-of-line copy-assignment @ 0x009FA1C0.
// One of 12 bodies of this shape.
struct CTexture {
    unsigned char _body[0x8];
    CTexture& operator=(const CTexture& other);   // 0x009FA1C0
};

void __fastcall Fill_CTexture_0049add0(CTexture* first, CTexture* last, const CTexture& value)
{
    for (; first != last; ++first)
        *first = value;
}
