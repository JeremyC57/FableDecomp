// _Uninit_fill_n<CElement52_00f27060*> -- placement-copy `count` copies of value from first (52-byte elements, copy ctor @ 0x00F27060), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F279E0, oracle length 42 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement52_00f27060::CElement52_00f27060(const CElement52_00f27060&) ; S: add esi,0x34 ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x00F27060 and returns the advanced destination. `ret 4` pops the value
// reference. The element type is named from the copy constructor's label when
// unique. One of 12 bodies.
#include <new>

struct CElement52_00f27060 {
    unsigned char _body[0x34];
    CElement52_00f27060(const CElement52_00f27060& other);           // 0x00F27060
};

CElement52_00f27060* __fastcall UninitFillN_CElement52_00f27060_00f279e0(CElement52_00f27060* first, unsigned long count, const CElement52_00f27060& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement52_00f27060(value);
    return first;
}