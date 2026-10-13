// _Uninit_fill_n<CElement12_004abe90*> -- placement-copy `count` copies of value from first (12-byte elements, copy ctor @ 0x004ABE90), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D34D10, oracle length 42 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement12_004abe90::CElement12_004abe90(const CElement12_004abe90&) ; S: add esi,0xc ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x004ABE90 and returns the advanced destination. `ret 4` pops the value
// reference. The element type is named from the copy constructor's label when
// unique. One of 12 bodies.
#include <new>

struct CElement12_004abe90 {
    unsigned char _body[0xc];
    CElement12_004abe90(const CElement12_004abe90& other);           // 0x004ABE90
};

CElement12_004abe90* __fastcall UninitFillN_CElement12_004abe90_00d34d10(CElement12_004abe90* first, unsigned long count, const CElement12_004abe90& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement12_004abe90(value);
    return first;
}
