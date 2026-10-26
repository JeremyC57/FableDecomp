// _Uninit_copy<C3DMeshInfo*> -- placement-copy [first, last) to dest (96-byte elements, copy ctor @ 0x00A2A400)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2AE20, oracle length 46 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[esp+10h] ; je E ; L: test edi,edi ; je S ; push esi ; mov ecx,edi ; call C3DMeshInfo::C3DMeshInfo(const C3DMeshInfo&) ; S: add esi,96 ; add edi,96 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_copy`, __fastcall: first in ecx, last in edx,
// dest on the stack): copy-constructs every element in place with placement new (hence the null
// test on the destination) through the out-of-line copy constructor @ 0x00A2A400 and returns
// the advanced destination. `ret 8` pops dest and the unused allocator argument. The element
// type is named from the copy constructor's label when unique. One of 21 bodies.
#include <new>

class C3DMeshInfo { public:
    unsigned char _body[0x60];
    C3DMeshInfo(const C3DMeshInfo& other);           // 0x00A2A400
};

struct CRetailAllocator { };

C3DMeshInfo* __fastcall UninitCopy_C3DMeshInfo_00a2ae20(C3DMeshInfo* first, C3DMeshInfo* last, C3DMeshInfo* dest, CRetailAllocator allocator)
{
    C3DMeshInfo* d = dest;                      // named local: retail loads dest before the loop test (GOTCHAS)
    for (; first != last; ++first, ++d)   // increment order = retail `add esi ; add edi` under /O1-style "s"
        new (d) C3DMeshInfo(*first);
    return d;
}