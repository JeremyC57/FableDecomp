// fill<CMapInfo*> -- assign value to every element of [first, last) through CMapInfo::operator= @ 0x00513BF0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00515090, oracle length 37 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,edx ; mov esi,ecx ; cmp esi,edi ; je E ; push ebx ; mov ebx,[value] ; L: push ebx ; mov ecx,esi ; call CMapInfo::operator= ; add esi,0x48 ; cmp esi,edi ; jne L ; pop ebx ; E: pop edi ; pop esi ; ret 4
// `fill(first, last, value)` (`__fastcall`: first in ecx, last in edx, the value reference on
// the stack) for 72-byte elements with an out-of-line copy-assignment @ 0x00513BF0.
// One of 12 bodies of this shape.
struct CMapInfo {
    unsigned char _body[0x48];
    CMapInfo& operator=(const CMapInfo& other);   // 0x00513BF0
};

void __fastcall Fill_CMapInfo_00515090(CMapInfo* first, CMapInfo* last, const CMapInfo& value)
{
    for (; first != last; ++first)
        *first = value;
}