#pragma optimize("s",on)
// _Uninit_fill_n<CElement12_0053cb9b*> -- placement-copy `count` copies of value from first (12-byte elements, copy ctor @ 0x0053CB9B), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053CCD0, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement12_0053cb9b::CElement12_0053cb9b(const CElement12_0053cb9b&) ; S: add esi,0xc ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack, then the by-value allocator): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x0053CB9B and returns the advanced destination. `ret 8` pops the value
// reference and the unused allocator. The element type is named from the copy constructor's label when
// unique. One of 26 bodies.
#include <new>

struct CElement12_0053cb9b {
    unsigned char _body[0xc];
    CElement12_0053cb9b(const CElement12_0053cb9b& other);           // 0x0053CB9B
};
struct CRetailAllocator { };
CElement12_0053cb9b* __fastcall UninitFillN_CElement12_0053cb9b_0053ccd0(CElement12_0053cb9b* first, unsigned long count, const CElement12_0053cb9b& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement12_0053cb9b(value);
    return first;
}