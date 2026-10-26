// _Uninit_fill_n<CTexture*> -- placement-copy `count` copies of value from first (8-byte elements, copy ctor @ 0x009F9D00), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049AE00, oracle length 42 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CTexture::CTexture(const CTexture&) ; S: add esi,0x8 ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x009F9D00 and returns the advanced destination. `ret 4` pops the value
// reference. The element type is named from the copy constructor's label when
// unique. One of 12 bodies.
#include <new>

class CTexture { public:
    unsigned char _body[0x8];
    CTexture(const CTexture& other);           // 0x009F9D00
};

CTexture* __fastcall UninitFillN_CTexture_0049ae00(CTexture* first, unsigned long count, const CTexture& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CTexture(value);
    return first;
}