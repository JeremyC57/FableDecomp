#pragma optimize("s",on)
// _Uninit_fill_n<CFadeInFadeOutBase*> -- placement-copy `count` copies of value from first (28-byte elements, copy ctor @ 0x00592D97), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00592FD5, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CFadeInFadeOutBase::CFadeInFadeOutBase(const CFadeInFadeOutBase&) ; S: add esi,0x1c ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack, then the by-value allocator): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x00592D97 and returns the advanced destination. `ret 8` pops the value
// reference and the unused allocator. The element type is named from the copy constructor's label when
// unique. One of 26 bodies.
#include <new>

class CFadeInFadeOutBase { public:
    unsigned char _body[0x1c];
    CFadeInFadeOutBase(const CFadeInFadeOutBase& other);           // 0x00592D97
};
struct CRetailAllocator { };
CFadeInFadeOutBase* __fastcall UninitFillN_CFadeInFadeOutBase_00592fd5(CFadeInFadeOutBase* first, unsigned long count, const CFadeInFadeOutBase& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) CFadeInFadeOutBase(value);
    return first;
}