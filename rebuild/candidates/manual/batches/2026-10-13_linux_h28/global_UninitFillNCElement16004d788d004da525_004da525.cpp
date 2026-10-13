// _Uninit_fill_n<CElement16_004d788d*> -- placement-copy `count` copies of value from first (16-byte elements, copy ctor @ 0x004D788D), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA525, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement16_004d788d::CElement16_004d788d(const CElement16_004d788d&) ; S: add esi,0x10 ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack, then the by-value allocator): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x004D788D and returns the advanced destination. `ret 8` pops the value
// reference and the unused allocator. The element type is named from the copy constructor's label when
// unique. One of 26 bodies.
#include <new>

struct CElement16_004d788d {
    unsigned char _body[0x10];
    CElement16_004d788d(const CElement16_004d788d& other);           // 0x004D788D
};
struct CRetailAllocator { };
CElement16_004d788d* __fastcall UninitFillN_CElement16_004d788d_004da525(CElement16_004d788d* first, unsigned long count, const CElement16_004d788d& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement16_004d788d(value);
    return first;
}
