// _Uninit_fill_n<CElement72_00ad0100*> -- placement-copy `count` copies of value from first (72-byte elements, copy ctor @ 0x00AD0100), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD1A00, oracle length 42 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement72_00ad0100::CElement72_00ad0100(const CElement72_00ad0100&) ; S: add esi,0x48 ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x00AD0100 and returns the advanced destination. `ret 4` pops the value
// reference. The element type is named from the copy constructor's label when
// unique. One of 9 bodies.
#include <new>

struct CElement72_00ad0100 {
    unsigned char _body[0x48];
    CElement72_00ad0100(const CElement72_00ad0100& other);           // 0x00AD0100
};

CElement72_00ad0100* __fastcall UninitFillN_CElement72_00ad0100_00ad1a00(CElement72_00ad0100* first, unsigned long count, const CElement72_00ad0100& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement72_00ad0100(value);
    return first;
}
