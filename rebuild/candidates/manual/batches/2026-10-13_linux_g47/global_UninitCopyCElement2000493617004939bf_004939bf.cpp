// _Uninit_copy<CElement20_00493617*> -- placement-copy [first, last) to dest (20-byte elements, copy ctor @ 0x00493617)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004939BF, oracle length 43 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[esp+10h] ; je E ; L: test edi,edi ; je S ; push esi ; mov ecx,edi ; call CElement20_00493617::CElement20_00493617(const CElement20_00493617&) ; S: add esi,20 ; add edi,20 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_copy`, __fastcall: first in ecx, last in edx,
// dest on the stack): copy-constructs every element in place with placement new (hence the null
// test on the destination) through the out-of-line copy constructor @ 0x00493617 and returns
// the advanced destination. `ret 8` pops dest and the unused allocator argument. The element
// type is named from the copy constructor's label when unique. One of 21 bodies.
#include <new>

struct CElement20_00493617 {
    unsigned char _body[0x14];
    CElement20_00493617(const CElement20_00493617& other);           // 0x00493617
};

struct CRetailAllocator { };

CElement20_00493617* __fastcall UninitCopy_CElement20_00493617_004939bf(CElement20_00493617* first, CElement20_00493617* last, CElement20_00493617* dest, CRetailAllocator allocator)
{
    CElement20_00493617* d = dest;                      // named local: retail loads dest before the loop test (GOTCHAS)
    for (; first != last; ++first, ++d)   // increment order = retail `add esi ; add edi` under /O1-style "s"
        new (d) CElement20_00493617(*first);
    return d;
}
