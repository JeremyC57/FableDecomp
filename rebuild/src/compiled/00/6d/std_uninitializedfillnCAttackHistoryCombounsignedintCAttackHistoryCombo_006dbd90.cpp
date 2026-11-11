// _Uninit_fill_n<SPairCSecond_004f9408Long*> -- placement-copy `count` copies of a {CSecond_004f9408, long} pair (16-byte elements, CSecond_004f9408 copy ctor @ 0x004F9408)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DBD90, oracle length 48 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push ebx ; mov ebx,[value] ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push ebx ; mov ecx,esi ; call CSecond_004f9408::CSecond_004f9408 ; mov eax,[ebx+4] ; mov [esi+4],eax ; S: add esi,0x10 ; dec edi ; jne L ; pop edi ; pop ebx ; E: mov eax,esi ; pop esi ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// value reference and the by-value allocator on the stack) for a pair whose second member is a 4-byte scalar
// and whose first member CSecond_004f9408 has the out-of-line copy constructor @ 0x004F9408: the pair's
// implicit copy constructor is inlined into the placement new (hence the null test), the
// advanced destination is returned. One of 12 bodies.
#include <new>

struct CSecond_004f9408 {
    unsigned char _body[0xc];
    CSecond_004f9408(const CSecond_004f9408& other);           // 0x004F9408
};

struct SPairCSecond_004f9408Long {
    CSecond_004f9408 first;
    long second;
};

struct CRetailAllocator { };

SPairCSecond_004f9408Long* __fastcall UninitFillN_SPairCSecond_004f9408Long_006dbd90(SPairCSecond_004f9408Long* first, unsigned long count, const SPairCSecond_004f9408Long& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairCSecond_004f9408Long(value);
    return first;
}