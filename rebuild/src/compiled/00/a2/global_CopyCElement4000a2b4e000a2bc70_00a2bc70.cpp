// SGI __copy<CElement40_00a2b4e0*> (random-access, 40-byte elements assigned through operator= @ 0x00A2B4E0) -- counted forward copy, returns the advanced result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2BC70, oracle length 67 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; sub edx,esi ; (divide by 40) ; test eax,eax ; jle Z ; push ebx ; push edi ; mov edi,[result] ; mov ebx,eax ; L: push esi ; mov ecx,edi ; call CElement40_00a2b4e0::operator= ; add esi,0x28 ; add edi,0x28 ; dec ebx ; jne L ; mov eax,edi ; pop edi ; pop ebx ; pop esi ; ret 0Ch ; Z: mov eax,[result] ; pop esi ; ret 0Ch
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)` for 40-byte
// elements with an out-of-line copy-assignment operator @ 0x00A2B4E0: the signed element count
// is computed once, each element is assigned (`*result = *first`) and the advanced result is
// returned. /Gr TU (__fastcall: first in ecx, last in edx, result + two tags on the stack).
// The manifest label (`_Copy_impl<...>`) is propagated from Dinkumware. One of 12 bodies.
struct CElement40_00a2b4e0 {
    unsigned char _body[0x28];
    CElement40_00a2b4e0& operator=(const CElement40_00a2b4e0& other);   // 0x00A2B4E0
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CElement40_00a2b4e0* __fastcall Copy_CElement40_00a2b4e0_00a2bc70(CElement40_00a2b4e0* first, CElement40_00a2b4e0* last, CElement40_00a2b4e0* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}