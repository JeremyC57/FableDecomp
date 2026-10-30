// _Uninit_fill_n<CConversation*> -- placement-copy `count` copies of value from first (92-byte elements, copy ctor @ 0x00E54CA0), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F339E0, oracle length 42 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CConversation::CConversation(const CConversation&) ; S: add esi,0x5c ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x00E54CA0 and returns the advanced destination. `ret 4` pops the value
// reference. The element type is named from the copy constructor's label when
// unique. One of 12 bodies.
#include <new>

class CConversation { public:
    unsigned char _body[0x5c];
    CConversation(const CConversation& other);           // 0x00E54CA0
};

CConversation* __fastcall UninitFillN_CElement92_00e54ca0_00f339e0(CConversation* first, unsigned long count, const CConversation& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CConversation(value);
    return first;
}