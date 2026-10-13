// _Uninit_fill_n<CElement16_0057f092*> -- placement-copy `count` copies of value from first (16-byte elements, copy ctor @ 0x0057F092), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057F683, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement16_0057f092::CElement16_0057f092(const CElement16_0057f092&) ; S: add esi,0x10 ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack, then the by-value allocator): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x0057F092 and returns the advanced destination. `ret 8` pops the value
// reference and the unused allocator. The element type is named from the copy constructor's label when
// unique. One of 26 bodies.
#include <new>

struct CElement16_0057f092 {
    unsigned char _body[0x10];
    CElement16_0057f092(const CElement16_0057f092& other);           // 0x0057F092
};
struct CRetailAllocator { };
CElement16_0057f092* __fastcall UninitFillN_CElement16_0057f092_0057f683(CElement16_0057f092* first, unsigned long count, const CElement16_0057f092& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement16_0057f092(value);
    return first;
}
