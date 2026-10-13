// _Uninit_copy<CElement12_00513700*> -- placement-copy [first, last) to dest (12-byte elements, copy ctor @ 0x00513700)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00518F30, oracle length 46 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[esp+10h] ; je E ; L: test edi,edi ; je S ; push esi ; mov ecx,edi ; call CElement12_00513700::CElement12_00513700(const CElement12_00513700&) ; S: add esi,12 ; add edi,12 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_copy`, __fastcall: first in ecx, last in edx,
// dest on the stack): copy-constructs every element in place with placement new (hence the null
// test on the destination) through the out-of-line copy constructor @ 0x00513700 and returns
// the advanced destination. `ret 8` pops dest and the unused allocator argument. The element
// type is named from the copy constructor's label when unique. One of 12 bodies.
#include <new>

struct CElement12_00513700 {
    unsigned char _body[0xc];
    CElement12_00513700(const CElement12_00513700& other);           // 0x00513700
};

struct CRetailAllocator { };

CElement12_00513700* __fastcall UninitCopy_CElement12_00513700_00518f30(CElement12_00513700* first, CElement12_00513700* last, CElement12_00513700* dest, CRetailAllocator allocator)
{
    CElement12_00513700* d = dest;                      // named local: retail loads dest before the loop test (GOTCHAS)
    for (; first != last; ++first, ++d)   // increment order = retail `add esi ; add edi` under /O1-style "s"
        new (d) CElement12_00513700(*first);
    return d;
}
