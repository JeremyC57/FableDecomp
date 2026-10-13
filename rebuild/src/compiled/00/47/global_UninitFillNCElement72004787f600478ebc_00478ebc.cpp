#pragma optimize("s",on)
// _Uninit_fill_n<CElement72_004787f6*> -- placement-copy `count` copies of value from first (72-byte elements, copy ctor @ 0x004787F6), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00478EBC, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CElement72_004787f6::CElement72_004787f6(const CElement72_004787f6&) ; S: add esi,0x48 ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x004787F6 and returns the advanced destination. `ret 4` pops the value
// reference. The element type is named from the copy constructor's label when
// unique. One of 26 bodies.
#include <new>

struct CElement72_004787f6 {
    unsigned char _body[0x48];
    CElement72_004787f6(const CElement72_004787f6& other);           // 0x004787F6
};

CElement72_004787f6* __fastcall UninitFillN_CElement72_004787f6_00478ebc(CElement72_004787f6* first, unsigned long count, const CElement72_004787f6& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CElement72_004787f6(value);
    return first;
}