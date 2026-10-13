// _Uninit_fill_n<SPairLongCSecond_00ae5d80*> -- placement-copy `count` copies of a {long, CSecond_00ae5d80} pair (16-byte elements, CSecond_00ae5d80 copy ctor @ 0x00AE5D80)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE74C0, oracle length 50 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push ebx ; mov ebx,[value] ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; mov eax,[ebx] ; lea ecx,[ebx+4] ; push ecx ; lea ecx,[esi+4] ; mov [esi],eax ; call CSecond_00ae5d80::CSecond_00ae5d80 ; S: add esi,0x10 ; dec edi ; jne L ; pop edi ; pop ebx ; E: mov eax,esi ; pop esi ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// value reference and the by-value allocator on the stack) for a pair whose first member is a 4-byte scalar
// and whose second member CSecond_00ae5d80 has the out-of-line copy constructor @ 0x00AE5D80: the pair's
// implicit copy constructor is inlined into the placement new (hence the null test), the
// advanced destination is returned. One of 13 bodies.
#include <new>

struct CSecond_00ae5d80 {
    unsigned char _body[0xc];
    CSecond_00ae5d80(const CSecond_00ae5d80& other);           // 0x00AE5D80
};

struct SPairLongCSecond_00ae5d80 {
    long first;
    CSecond_00ae5d80 second;
};

struct CRetailAllocator { };

SPairLongCSecond_00ae5d80* __fastcall UninitFillN_SPairLongCSecond_00ae5d80_00ae74c0(SPairLongCSecond_00ae5d80* first, unsigned long count, const SPairLongCSecond_00ae5d80& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairLongCSecond_00ae5d80(value);
    return first;
}
