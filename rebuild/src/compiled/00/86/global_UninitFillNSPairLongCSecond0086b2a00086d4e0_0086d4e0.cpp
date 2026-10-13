// _Uninit_fill_n<SPairLongCSecond_0086b2a0*> -- placement-copy `count` copies of a {long, CSecond_0086b2a0} pair (16-byte elements, CSecond_0086b2a0 copy ctor @ 0x0086B2A0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086D4E0, oracle length 50 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push ebx ; mov ebx,[value] ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; mov eax,[ebx] ; lea ecx,[ebx+4] ; push ecx ; lea ecx,[esi+4] ; mov [esi],eax ; call CSecond_0086b2a0::CSecond_0086b2a0 ; S: add esi,0x10 ; dec edi ; jne L ; pop edi ; pop ebx ; E: mov eax,esi ; pop esi ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// value reference and the by-value allocator on the stack) for a pair whose first member is a 4-byte scalar
// and whose second member CSecond_0086b2a0 has the out-of-line copy constructor @ 0x0086B2A0: the pair's
// implicit copy constructor is inlined into the placement new (hence the null test), the
// advanced destination is returned. One of 13 bodies.
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

SPairLongCSecond_0086b2a0* __fastcall UninitFillN_SPairLongCSecond_0086b2a0_0086d4e0(SPairLongCSecond_0086b2a0* first, unsigned long count, const SPairLongCSecond_0086b2a0& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairLongCSecond_0086b2a0(value);
    return first;
}