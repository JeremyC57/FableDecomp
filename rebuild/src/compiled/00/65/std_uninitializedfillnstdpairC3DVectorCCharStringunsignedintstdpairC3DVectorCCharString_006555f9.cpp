#pragma optimize("s",on)
// _Uninit_fill_n<CElement16_00654dd4*> -- placement-copy `count` copies of value from first (16-byte elements, copy ctor @ 0x00654DD4), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006555F9, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement16_00654dd4::CElement16_00654dd4(const CElement16_00654dd4&) ; S: add esi,0x10 ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack, then the by-value allocator): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x00654DD4 and returns the advanced destination. `ret 8` pops the value
// reference and the unused allocator. The element type is named from the copy constructor's label when
// unique. One of 26 bodies.
#include <new>

struct CElement16_00654dd4 {
    unsigned char _body[0x10];
    CElement16_00654dd4(const CElement16_00654dd4& other);           // 0x00654DD4
};
struct CRetailAllocator { };
CElement16_00654dd4* __fastcall UninitFillN_CElement16_00654dd4_006555f9(CElement16_00654dd4* first, unsigned long count, const CElement16_00654dd4& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement16_00654dd4(value);
    return first;
}