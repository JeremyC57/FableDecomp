// _Uninit_fill_n<CElement184_00ad77f0*> -- placement-copy `count` copies of value from first (184-byte elements, copy ctor @ 0x00AD77F0), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD7B90, oracle length 45 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement184_00ad77f0::CElement184_00ad77f0(const CElement184_00ad77f0&) ; S: add esi,0xb8 ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x00AD77F0 and returns the advanced destination. `ret 4` pops the value
// reference. The element type is named from the copy constructor's label when
// unique. One of 9 bodies.
#include <new>

struct CElement184_00ad77f0 {
    unsigned char _body[0xb8];
    CElement184_00ad77f0(const CElement184_00ad77f0& other);           // 0x00AD77F0
};

CElement184_00ad77f0* __fastcall UninitFillN_CElement184_00ad77f0_00ad7b90(CElement184_00ad77f0* first, unsigned long count, const CElement184_00ad77f0& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement184_00ad77f0(value);
    return first;
}