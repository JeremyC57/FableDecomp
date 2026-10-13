// SGI __copy<CElement12_0075b8d0*> (random-access, 12-byte elements assigned through operator= @ 0x0075B8D0) -- counted forward copy, returns the advanced result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCD950, oracle length 66 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; sub edx,esi ; (divide by 12) ; test eax,eax ; jle Z ; push ebx ; push edi ; mov edi,[result] ; mov ebx,eax ; L: push esi ; mov ecx,edi ; call CElement12_0075b8d0::operator= ; add esi,0xc ; add edi,0xc ; dec ebx ; jne L ; mov eax,edi ; pop edi ; pop ebx ; pop esi ; ret 0Ch ; Z: mov eax,[result] ; pop esi ; ret 0Ch
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)` for 12-byte
// elements with an out-of-line copy-assignment operator @ 0x0075B8D0: the signed element count
// is computed once, each element is assigned (`*result = *first`) and the advanced result is
// returned. /Gr TU (__fastcall: first in ecx, last in edx, result + two tags on the stack).
// The manifest label (`_Copy_impl<...>`) is propagated from Dinkumware. One of 9 bodies.
struct CElement12_0075b8d0 {
    unsigned char _body[0xc];
    CElement12_0075b8d0& operator=(const CElement12_0075b8d0& other);   // 0x0075B8D0
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CElement12_0075b8d0* __fastcall Copy_CElement12_0075b8d0_00bcd950(CElement12_0075b8d0* first, CElement12_0075b8d0* last, CElement12_0075b8d0* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}