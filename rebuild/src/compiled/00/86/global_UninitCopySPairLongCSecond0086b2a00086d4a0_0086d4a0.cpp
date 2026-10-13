// _Uninit_copy<SPairLongCSecond_0086b2a0*> -- placement-copy [first, last) to dest (16-byte {long, CSecond_0086b2a0} pairs, CSecond_0086b2a0 copy ctor @ 0x0086B2A0), return the advanced dest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086D4A0, oracle length 54 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[dest] ; je E ; L: test edi,edi ; je S ; mov eax,[esi] ; lea ecx,[esi+4] ; push ecx ; lea ecx,[edi+4] ; mov [edi],eax ; call CSecond_0086b2a0::CSecond_0086b2a0 ; S: add esi,0x10 ; add edi,0x10 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Dinkumware `_Uninit_copy` (`__fastcall`: first in ecx, last in edx, dest and the by-value
// allocator on the stack) for a pair whose first member is a 4-byte scalar and whose second
// member CSecond_0086b2a0 has the out-of-line copy constructor @ 0x0086B2A0: the pair's implicit copy
// constructor is inlined into the placement new (hence the null test) and the advanced
// destination is returned. One of 9 bodies of this shape.
#include <new>

struct CSecond_0086b2a0 {
    unsigned char _body[0xc];
    CSecond_0086b2a0(const CSecond_0086b2a0& other);           // 0x0086B2A0
};

struct SPairLongCSecond_0086b2a0 {
    long first;
    CSecond_0086b2a0 second;
};

struct CRetailAllocator { };

SPairLongCSecond_0086b2a0* __fastcall UninitCopy_SPairLongCSecond_0086b2a0_0086d4a0(SPairLongCSecond_0086b2a0* first, SPairLongCSecond_0086b2a0* last, SPairLongCSecond_0086b2a0* dest, CRetailAllocator allocator)
{
    SPairLongCSecond_0086b2a0* d = dest;
    for (; first != last; ++first, ++d)
        new (d) SPairLongCSecond_0086b2a0(*first);
    return d;
}