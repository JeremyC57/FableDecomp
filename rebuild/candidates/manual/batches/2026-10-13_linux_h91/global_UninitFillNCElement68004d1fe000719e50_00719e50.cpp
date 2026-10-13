// _Uninit_fill_n<CElement68_004d1fe0*> -- placement-copy `count` copies of value from first (68-byte elements, copy ctor @ 0x004D1FE0), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00719E50, oracle length 42 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement68_004d1fe0::CElement68_004d1fe0(const CElement68_004d1fe0&) ; S: add esi,0x44 ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x004D1FE0 and returns the advanced destination. `ret 4` pops the value
// reference. The element type is named from the copy constructor's label when
// unique. One of 12 bodies.
#include <new>

struct CElement68_004d1fe0 {
    unsigned char _body[0x44];
    CElement68_004d1fe0(const CElement68_004d1fe0& other);           // 0x004D1FE0
};

CElement68_004d1fe0* __fastcall UninitFillN_CElement68_004d1fe0_00719e50(CElement68_004d1fe0* first, unsigned long count, const CElement68_004d1fe0& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement68_004d1fe0(value);
    return first;
}
