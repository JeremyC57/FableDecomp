#pragma optimize("s",on)
// _Uninit_fill_n<CAbilityButtonGuiDef*> -- placement-copy `count` copies of value from first (92-byte elements, copy ctor @ 0x0046F1B4), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004768F8, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CAbilityButtonGuiDef::CAbilityButtonGuiDef(const CAbilityButtonGuiDef&) ; S: add esi,0x5c ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x0046F1B4 and returns the advanced destination. `ret 4` pops the value
// reference. The element type is named from the copy constructor's label when
// unique. One of 26 bodies.
#include <new>

struct CAbilityButtonGuiDef {
    unsigned char _body[0x5c];
    CAbilityButtonGuiDef(const CAbilityButtonGuiDef& other);           // 0x0046F1B4
};

CAbilityButtonGuiDef* __fastcall UninitFillN_CAbilityButtonGuiDef_004768f8(CAbilityButtonGuiDef* first, unsigned long count, const CAbilityButtonGuiDef& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CAbilityButtonGuiDef(value);
    return first;
}