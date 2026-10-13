// _Uninit_fill_n<SPairLongCSecond_005bbdf1*> -- placement-copy `count` copies of a {long, CSecond_005bbdf1} pair (16-byte elements, CSecond_005bbdf1 copy ctor @ 0x005BBDF1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00728B50, oracle length 50 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push ebx ; mov ebx,[value] ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; mov eax,[ebx] ; lea ecx,[ebx+4] ; push ecx ; lea ecx,[esi+4] ; mov [esi],eax ; call CSecond_005bbdf1::CSecond_005bbdf1 ; S: add esi,0x10 ; dec edi ; jne L ; pop edi ; pop ebx ; E: mov eax,esi ; pop esi ; ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// value reference and the by-value allocator on the stack) for a pair whose first member is a 4-byte scalar
// and whose second member CSecond_005bbdf1 has the out-of-line copy constructor @ 0x005BBDF1: the pair's
// implicit copy constructor is inlined into the placement new (hence the null test), the
// advanced destination is returned. One of 13 bodies.
#include <new>

struct CSecond_005bbdf1 {
    unsigned char _body[0xc];
    CSecond_005bbdf1(const CSecond_005bbdf1& other);           // 0x005BBDF1
};

struct SPairLongCSecond_005bbdf1 {
    long first;
    CSecond_005bbdf1 second;
};

struct CRetailAllocator { };

SPairLongCSecond_005bbdf1* __fastcall UninitFillN_SPairLongCSecond_005bbdf1_00728b50(SPairLongCSecond_005bbdf1* first, unsigned long count, const SPairLongCSecond_005bbdf1& value)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairLongCSecond_005bbdf1(value);
    return first;
}