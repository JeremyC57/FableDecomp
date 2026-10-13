// _Uninit_copy<CElement124_006250b0*> -- placement-copy [first, last) to dest (124-byte elements, copy ctor @ 0x006250B0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00631560, oracle length 46 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[esp+10h] ; je E ; L: test edi,edi ; je S ; push esi ; mov ecx,edi ; call CElement124_006250b0::CElement124_006250b0(const CElement124_006250b0&) ; S: add esi,124 ; add edi,124 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_copy`, __fastcall: first in ecx, last in edx,
// dest on the stack): copy-constructs every element in place with placement new (hence the null
// test on the destination) through the out-of-line copy constructor @ 0x006250B0 and returns
// the advanced destination. `ret 8` pops dest and the unused allocator argument. The element
// type is named from the copy constructor's label when unique. One of 12 bodies.
#include <new>

struct CElement124_006250b0 {
    unsigned char _body[0x7c];
    CElement124_006250b0(const CElement124_006250b0& other);           // 0x006250B0
};

struct CRetailAllocator { };

CElement124_006250b0* __fastcall UninitCopy_CElement124_006250b0_00631560(CElement124_006250b0* first, CElement124_006250b0* last, CElement124_006250b0* dest, CRetailAllocator allocator)
{
    CElement124_006250b0* d = dest;                      // named local: retail loads dest before the loop test (GOTCHAS)
    for (; first != last; ++first, ++d)   // increment order = retail `add esi ; add edi` under /O1-style "s"
        new (d) CElement124_006250b0(*first);
    return d;
}
