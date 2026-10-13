#pragma optimize("s",on)
// _Uninit_fill_n<CElement72_004784b3*> -- placement-copy `count` copies of value from first (72-byte elements, copy ctor @ 0x004784B3), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047A2CF, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement72_004784b3::CElement72_004784b3(const CElement72_004784b3&) ; S: add esi,0x48 ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x004784B3 and returns the advanced destination. `ret 4` pops the value
// reference. The element type is named from the copy constructor's label when
// unique. One of 26 bodies.
#include <new>

struct CElement72_004784b3 {
    unsigned char _body[0x48];
    CElement72_004784b3(const CElement72_004784b3& other);           // 0x004784B3
};

CElement72_004784b3* __fastcall UninitFillN_CElement72_004784b3_0047a2cf(CElement72_004784b3* first, unsigned long count, const CElement72_004784b3& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement72_004784b3(value);
    return first;
}