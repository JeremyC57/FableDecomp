// _Uninit_fill_n<CElement124_006250b0*> -- placement-copy `count` copies of value from first (124-byte elements, copy ctor @ 0x006250B0), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633340, oracle length 42 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement124_006250b0::CElement124_006250b0(const CElement124_006250b0&) ; S: add esi,0x7c ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x006250B0 and returns the advanced destination. `ret 4` pops the value
// reference. The element type is named from the copy constructor's label when
// unique. One of 9 bodies.
#include <new>

struct CElement124_006250b0 {
    unsigned char _body[0x7c];
    CElement124_006250b0(const CElement124_006250b0& other);           // 0x006250B0
};

CElement124_006250b0* __fastcall UninitFillN_CElement124_006250b0_00633340(CElement124_006250b0* first, unsigned long count, const CElement124_006250b0& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement124_006250b0(value);
    return first;
}