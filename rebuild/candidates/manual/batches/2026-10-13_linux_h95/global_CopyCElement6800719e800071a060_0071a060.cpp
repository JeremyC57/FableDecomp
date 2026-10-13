// SGI __copy<CElement68_00719e80*> (random-access, 68-byte elements assigned through operator= @ 0x00719E80) -- counted forward copy, returns the advanced result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071A060, oracle length 67 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; sub edx,esi ; (divide by 68) ; test eax,eax ; jle Z ; push ebx ; push edi ; mov edi,[result] ; mov ebx,eax ; L: push esi ; mov ecx,edi ; call CElement68_00719e80::operator= ; add esi,0x44 ; add edi,0x44 ; dec ebx ; jne L ; mov eax,edi ; pop edi ; pop ebx ; pop esi ; ret 0Ch ; Z: mov eax,[result] ; pop esi ; ret 0Ch
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)` for 68-byte
// elements with an out-of-line copy-assignment operator @ 0x00719E80: the signed element count
// is computed once, each element is assigned (`*result = *first`) and the advanced result is
// returned. /Gr TU (__fastcall: first in ecx, last in edx, result + two tags on the stack).
// The manifest label (`_Copy_impl<...>`) is propagated from Dinkumware. One of 12 bodies.
struct CElement68_00719e80 {
    unsigned char _body[0x44];
    CElement68_00719e80& operator=(const CElement68_00719e80& other);   // 0x00719E80
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CElement68_00719e80* __fastcall Copy_CElement68_00719e80_0071a060(CElement68_00719e80* first, CElement68_00719e80* last, CElement68_00719e80* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
