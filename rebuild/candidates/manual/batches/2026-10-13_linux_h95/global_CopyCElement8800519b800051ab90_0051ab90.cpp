// SGI __copy<CElement88_00519b80*> (random-access, 88-byte elements assigned through operator= @ 0x00519B80) -- counted forward copy, returns the advanced result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051AB90, oracle length 67 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; sub edx,esi ; (divide by 88) ; test eax,eax ; jle Z ; push ebx ; push edi ; mov edi,[result] ; mov ebx,eax ; L: push esi ; mov ecx,edi ; call CElement88_00519b80::operator= ; add esi,0x58 ; add edi,0x58 ; dec ebx ; jne L ; mov eax,edi ; pop edi ; pop ebx ; pop esi ; ret 0Ch ; Z: mov eax,[result] ; pop esi ; ret 0Ch
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)` for 88-byte
// elements with an out-of-line copy-assignment operator @ 0x00519B80: the signed element count
// is computed once, each element is assigned (`*result = *first`) and the advanced result is
// returned. /Gr TU (__fastcall: first in ecx, last in edx, result + two tags on the stack).
// The manifest label (`_Copy_impl<...>`) is propagated from Dinkumware. One of 12 bodies.
struct CElement88_00519b80 {
    unsigned char _body[0x58];
    CElement88_00519b80& operator=(const CElement88_00519b80& other);   // 0x00519B80
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CElement88_00519b80* __fastcall Copy_CElement88_00519b80_0051ab90(CElement88_00519b80* first, CElement88_00519b80* last, CElement88_00519b80* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
