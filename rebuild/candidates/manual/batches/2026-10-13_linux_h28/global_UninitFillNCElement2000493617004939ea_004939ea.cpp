// _Uninit_fill_n<CElement20_00493617*> -- placement-copy `count` copies of value from first (20-byte elements, copy ctor @ 0x00493617), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004939EA, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement20_00493617::CElement20_00493617(const CElement20_00493617&) ; S: add esi,0x14 ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack, then the by-value allocator): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x00493617 and returns the advanced destination. `ret 8` pops the value
// reference and the unused allocator. The element type is named from the copy constructor's label when
// unique. One of 26 bodies.
#include <new>

struct CElement20_00493617 {
    unsigned char _body[0x14];
    CElement20_00493617(const CElement20_00493617& other);           // 0x00493617
};
struct CRetailAllocator { };
CElement20_00493617* __fastcall UninitFillN_CElement20_00493617_004939ea(CElement20_00493617* first, unsigned long count, const CElement20_00493617& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement20_00493617(value);
    return first;
}
