// _Uninit_fill_n<SPairCSecond_0099ec30Long*> -- placement-copy `count` copies of a {CSecond_0099ec30, long} pair (8-byte elements, CSecond_0099ec30 copy ctor @ 0x0099EC30)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086AB50, oracle length 48 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push ebx ; mov ebx,[value] ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push ebx ; mov ecx,esi ; call CSecond_0099ec30::CSecond_0099ec30 ; mov eax,[ebx+4] ; mov [esi+4],eax ; S: add esi,0x8 ; dec edi ; jne L ; pop edi ; pop ebx ; E: mov eax,esi ; pop esi ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// value reference and the by-value allocator on the stack) for a pair whose second member is a 4-byte scalar
// and whose first member CSecond_0099ec30 has the out-of-line copy constructor @ 0x0099EC30: the pair's
// implicit copy constructor is inlined into the placement new (hence the null test), the
// advanced destination is returned. One of 12 bodies.
#include <new>

struct CSecond_0099ec30 {
    unsigned char _body[0x4];
    CSecond_0099ec30(const CSecond_0099ec30& other);           // 0x0099EC30
};

struct SPairCSecond_0099ec30Long {
    CSecond_0099ec30 first;
    long second;
};

struct CRetailAllocator { };

SPairCSecond_0099ec30Long* __fastcall UninitFillN_SPairCSecond_0099ec30Long_0086ab50(SPairCSecond_0099ec30Long* first, unsigned long count, const SPairCSecond_0099ec30Long& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairCSecond_0099ec30Long(value);
    return first;
}
