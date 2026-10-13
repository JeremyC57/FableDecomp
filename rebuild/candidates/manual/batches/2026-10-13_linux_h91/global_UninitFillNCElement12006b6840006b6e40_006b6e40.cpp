// _Uninit_fill_n<CElement12_006b6840*> -- placement-copy `count` copies of value from first (12-byte elements, copy ctor @ 0x006B6840), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6E40, oracle length 42 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement12_006b6840::CElement12_006b6840(const CElement12_006b6840&) ; S: add esi,0xc ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack, then the by-value allocator): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x006B6840 and returns the advanced destination. `ret 8` pops the value
// reference and the unused allocator. The element type is named from the copy constructor's label when
// unique. One of 12 bodies.
#include <new>

struct CElement12_006b6840 {
    unsigned char _body[0xc];
    CElement12_006b6840(const CElement12_006b6840& other);           // 0x006B6840
};
struct CRetailAllocator { };
CElement12_006b6840* __fastcall UninitFillN_CElement12_006b6840_006b6e40(CElement12_006b6840* first, unsigned long count, const CElement12_006b6840& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement12_006b6840(value);
    return first;
}
