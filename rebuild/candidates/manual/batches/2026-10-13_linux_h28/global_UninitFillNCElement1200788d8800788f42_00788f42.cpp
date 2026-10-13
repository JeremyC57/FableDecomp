// _Uninit_fill_n<CElement12_00788d88*> -- placement-copy `count` copies of value from first (12-byte elements, copy ctor @ 0x00788D88), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00788F42, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement12_00788d88::CElement12_00788d88(const CElement12_00788d88&) ; S: add esi,0xc ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack, then the by-value allocator): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x00788D88 and returns the advanced destination. `ret 8` pops the value
// reference and the unused allocator. The element type is named from the copy constructor's label when
// unique. One of 26 bodies.
#include <new>

struct CElement12_00788d88 {
    unsigned char _body[0xc];
    CElement12_00788d88(const CElement12_00788d88& other);           // 0x00788D88
};
struct CRetailAllocator { };
CElement12_00788d88* __fastcall UninitFillN_CElement12_00788d88_00788f42(CElement12_00788d88* first, unsigned long count, const CElement12_00788d88& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement12_00788d88(value);
    return first;
}
