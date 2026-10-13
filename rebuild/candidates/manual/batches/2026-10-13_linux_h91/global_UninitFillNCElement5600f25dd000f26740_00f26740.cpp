// _Uninit_fill_n<CElement56_00f25dd0*> -- placement-copy `count` copies of value from first (56-byte elements, copy ctor @ 0x00F25DD0), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F26740, oracle length 42 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement56_00f25dd0::CElement56_00f25dd0(const CElement56_00f25dd0&) ; S: add esi,0x38 ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x00F25DD0 and returns the advanced destination. `ret 4` pops the value
// reference. The element type is named from the copy constructor's label when
// unique. One of 12 bodies.
#include <new>

struct CElement56_00f25dd0 {
    unsigned char _body[0x38];
    CElement56_00f25dd0(const CElement56_00f25dd0& other);           // 0x00F25DD0
};

CElement56_00f25dd0* __fastcall UninitFillN_CElement56_00f25dd0_00f26740(CElement56_00f25dd0* first, unsigned long count, const CElement56_00f25dd0& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement56_00f25dd0(value);
    return first;
}
