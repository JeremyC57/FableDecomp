#pragma optimize("s",on)
// _Uninit_copy<CElement44_005c2086*> -- placement-copy [first, last) to dest (44-byte elements, copy ctor @ 0x005C2086)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2929, oracle length 43 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[esp+10h] ; je E ; L: test edi,edi ; je S ; push esi ; mov ecx,edi ; call CElement44_005c2086::CElement44_005c2086(const CElement44_005c2086&) ; S: add esi,44 ; add edi,44 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_copy`, __fastcall: first in ecx, last in edx,
// dest on the stack): copy-constructs every element in place with placement new (hence the null
// test on the destination) through the out-of-line copy constructor @ 0x005C2086 and returns
// the advanced destination. `ret 8` pops dest and the unused allocator argument. The element
// type is named from the copy constructor's label when unique. One of 21 bodies.
#include <new>

struct CElement44_005c2086 {
    unsigned char _body[0x2c];
    CElement44_005c2086(const CElement44_005c2086& other);           // 0x005C2086
};

struct CRetailAllocator { };

CElement44_005c2086* __fastcall UninitCopy_CElement44_005c2086_005c2929(CElement44_005c2086* first, CElement44_005c2086* last, CElement44_005c2086* dest, CRetailAllocator allocator)
{
    CElement44_005c2086* d = dest;                      // named local: retail loads dest before the loop test (GOTCHAS)
    for (; first != last; ++first, ++d)   // increment order = retail `add esi ; add edi` under /O1-style "s"
        new (d) CElement44_005c2086(*first);
    return d;
}