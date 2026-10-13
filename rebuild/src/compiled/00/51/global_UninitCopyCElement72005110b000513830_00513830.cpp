// _Uninit_copy<CElement72_005110b0*> -- placement-copy [first, last) to dest (72-byte elements, copy ctor @ 0x005110B0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00513830, oracle length 46 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[esp+10h] ; je E ; L: test edi,edi ; je S ; push esi ; mov ecx,edi ; call CElement72_005110b0::CElement72_005110b0(const CElement72_005110b0&) ; S: add esi,72 ; add edi,72 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_copy`, __fastcall: first in ecx, last in edx,
// dest on the stack): copy-constructs every element in place with placement new (hence the null
// test on the destination) through the out-of-line copy constructor @ 0x005110B0 and returns
// the advanced destination. `ret 8` pops dest and the unused allocator argument. The element
// type is named from the copy constructor's label when unique. One of 21 bodies.
#include <new>

struct CElement72_005110b0 {
    unsigned char _body[0x48];
    CElement72_005110b0(const CElement72_005110b0& other);           // 0x005110B0
};

struct CRetailAllocator { };

CElement72_005110b0* __fastcall UninitCopy_CElement72_005110b0_00513830(CElement72_005110b0* first, CElement72_005110b0* last, CElement72_005110b0* dest, CRetailAllocator allocator)
{
    CElement72_005110b0* d = dest;                      // named local: retail loads dest before the loop test (GOTCHAS)
    for (; first != last; ++first, ++d)   // increment order = retail `add esi ; add edi` under /O1-style "s"
        new (d) CElement72_005110b0(*first);
    return d;
}