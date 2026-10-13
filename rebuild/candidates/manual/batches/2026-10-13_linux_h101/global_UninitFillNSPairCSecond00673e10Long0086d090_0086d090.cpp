// _Uninit_fill_n<SPairCSecond_00673e10Long*> -- placement-copy `count` copies of a {CSecond_00673e10, long} pair (16-byte elements, CSecond_00673e10 copy ctor @ 0x00673E10)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086D090, oracle length 48 bytes.
// Grade: candidate -- still DIFFER(48v48) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push ebx ; mov ebx,[value] ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push ebx ; mov ecx,esi ; call CSecond_00673e10::CSecond_00673e10 ; mov eax,[ebx+4] ; mov [esi+4],eax ; S: add esi,0x10 ; dec edi ; jne L ; pop edi ; pop ebx ; E: mov eax,esi ; pop esi ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// value reference and the by-value allocator on the stack) for a pair whose second member is a 4-byte scalar
// and whose first member CSecond_00673e10 has the out-of-line copy constructor @ 0x00673E10: the pair's
// implicit copy constructor is inlined into the placement new (hence the null test), the
// advanced destination is returned. One of 12 bodies.
#include <new>

struct CSecond_00673e10 {
    unsigned char _body[0xc];
    CSecond_00673e10(const CSecond_00673e10& other);           // 0x00673E10
};

struct SPairCSecond_00673e10Long {
    CSecond_00673e10 first;
    long second;
};

struct CRetailAllocator { };

SPairCSecond_00673e10Long* __fastcall UninitFillN_SPairCSecond_00673e10Long_0086d090(SPairCSecond_00673e10Long* first, unsigned long count, const SPairCSecond_00673e10Long& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairCSecond_00673e10Long(value);
    return first;
}
