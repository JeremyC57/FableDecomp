#pragma optimize("s",on)
// _Uninit_fill_n<CElement8_00592dfb*> -- placement-copy `count` copies of value from first (8-byte elements, copy ctor @ 0x00592DFB), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059382B, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement8_00592dfb::CElement8_00592dfb(const CElement8_00592dfb&) ; S: add esi,0x8 ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack, then the by-value allocator): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x00592DFB and returns the advanced destination. `ret 8` pops the value
// reference and the unused allocator. The element type is named from the copy constructor's label when
// unique. One of 26 bodies.
#include <new>

struct CElement8_00592dfb {
    unsigned char _body[0x8];
    CElement8_00592dfb(const CElement8_00592dfb& other);           // 0x00592DFB
};
struct CRetailAllocator { };
CElement8_00592dfb* __fastcall UninitFillN_CElement8_00592dfb_0059382b(CElement8_00592dfb* first, unsigned long count, const CElement8_00592dfb& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement8_00592dfb(value);
    return first;
}