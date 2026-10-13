// SGI __copy<CElement44_006255b0*> (random-access, 44-byte elements assigned through operator= @ 0x006255B0) -- counted forward copy, returns the advanced result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00625820, oracle length 67 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; sub edx,esi ; (divide by 44) ; test eax,eax ; jle Z ; push ebx ; push edi ; mov edi,[result] ; mov ebx,eax ; L: push esi ; mov ecx,edi ; call CElement44_006255b0::operator= ; add esi,0x2c ; add edi,0x2c ; dec ebx ; jne L ; mov eax,edi ; pop edi ; pop ebx ; pop esi ; ret 0Ch ; Z: mov eax,[result] ; pop esi ; ret 0Ch
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)` for 44-byte
// elements with an out-of-line copy-assignment operator @ 0x006255B0: the signed element count
// is computed once, each element is assigned (`*result = *first`) and the advanced result is
// returned. /Gr TU (__fastcall: first in ecx, last in edx, result + two tags on the stack).
// The manifest label (`_Copy_impl<...>`) is propagated from Dinkumware. One of 12 bodies.
struct CElement44_006255b0 {
    unsigned char _body[0x2c];
    CElement44_006255b0& operator=(const CElement44_006255b0& other);   // 0x006255B0
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CElement44_006255b0* __fastcall Copy_CElement44_006255b0_00625820(CElement44_006255b0* first, CElement44_006255b0* last, CElement44_006255b0* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
