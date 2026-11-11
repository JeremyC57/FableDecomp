// _Uninit_copy<CElement496_006b6010*> -- placement-copy [first, last) to dest (496-byte elements, copy ctor @ 0x006B6010)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6500, oracle length 52 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[esp+10h] ; je E ; L: test edi,edi ; je S ; push esi ; mov ecx,edi ; call CElement496_006b6010::CElement496_006b6010(const CElement496_006b6010&) ; S: add esi,496 ; add edi,496 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_copy`, __fastcall: first in ecx, last in edx,
// dest on the stack): copy-constructs every element in place with placement new (hence the null
// test on the destination) through the out-of-line copy constructor @ 0x006B6010 and returns
// the advanced destination. `ret 8` pops dest and the unused allocator argument. The element
// type is named from the copy constructor's label when unique. One of 21 bodies.
#include <new>

struct CElement496_006b6010 {
    unsigned char _body[0x1f0];
    CElement496_006b6010(const CElement496_006b6010& other);           // 0x006B6010
};

struct CRetailAllocator { };

CElement496_006b6010* __fastcall UninitCopy_CElement496_006b6010_006b6500(CElement496_006b6010* first, CElement496_006b6010* last, CElement496_006b6010* dest, CRetailAllocator allocator)
{
    CElement496_006b6010* d = dest;                      // named local: retail loads dest before the loop test (GOTCHAS)
    for (; first != last; ++first, ++d)   // increment order = retail `add esi ; add edi` under /O1-style "s"
        new (d) CElement496_006b6010(*first);
    return d;
}