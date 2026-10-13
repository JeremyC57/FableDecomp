// _Uninit_copy<CRegion*> -- placement-copy [first, last) to dest (88-byte elements, copy ctor @ 0x0051A900)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051B420, oracle length 46 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[esp+10h] ; je E ; L: test edi,edi ; je S ; push esi ; mov ecx,edi ; call CRegion::CRegion(const CRegion&) ; S: add esi,88 ; add edi,88 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_copy`, __fastcall: first in ecx, last in edx,
// dest on the stack): copy-constructs every element in place with placement new (hence the null
// test on the destination) through the out-of-line copy constructor @ 0x0051A900 and returns
// the advanced destination. `ret 8` pops dest and the unused allocator argument. The element
// type is named from the copy constructor's label when unique. One of 21 bodies.
#include <new>

struct CRegion {
    unsigned char _body[0x58];
    CRegion(const CRegion& other);           // 0x0051A900
};

struct CRetailAllocator { };

CRegion* __fastcall UninitCopy_CRegion_0051b420(CRegion* first, CRegion* last, CRegion* dest, CRetailAllocator allocator)
{
    CRegion* d = dest;                      // named local: retail loads dest before the loop test (GOTCHAS)
    for (; first != last; ++first, ++d)   // increment order = retail `add esi ; add edi` under /O1-style "s"
        new (d) CRegion(*first);
    return d;
}
