// _Uninit_copy<CElement36_008e2fc0*> -- placement-copy [first, last) to dest (36-byte elements, copy ctor @ 0x008E2FC0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E33D0, oracle length 46 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[esp+10h] ; je E ; L: test edi,edi ; je S ; push esi ; mov ecx,edi ; call CElement36_008e2fc0::CElement36_008e2fc0(const CElement36_008e2fc0&) ; S: add esi,36 ; add edi,36 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_copy`, __fastcall: first in ecx, last in edx,
// dest on the stack): copy-constructs every element in place with placement new (hence the null
// test on the destination) through the out-of-line copy constructor @ 0x008E2FC0 and returns
// the advanced destination. `ret 8` pops dest and the unused allocator argument. The element
// type is named from the copy constructor's label when unique. One of 21 bodies.
#include <new>

struct CElement36_008e2fc0 {
    unsigned char _body[0x24];
    CElement36_008e2fc0(const CElement36_008e2fc0& other);           // 0x008E2FC0
};

struct CRetailAllocator { };

CElement36_008e2fc0* __fastcall UninitCopy_CElement36_008e2fc0_008e33d0(CElement36_008e2fc0* first, CElement36_008e2fc0* last, CElement36_008e2fc0* dest, CRetailAllocator allocator)
{
    CElement36_008e2fc0* d = dest;                      // named local: retail loads dest before the loop test (GOTCHAS)
    for (; first != last; ++first, ++d)   // increment order = retail `add esi ; add edi` under /O1-style "s"
        new (d) CElement36_008e2fc0(*first);
    return d;
}