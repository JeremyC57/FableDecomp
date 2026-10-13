// _Uninit_copy<SElement8*> -- placement-copy [first, last) to dest (8-byte POD elements), return the advanced dest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B41C0, oracle length 44 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  cmp ecx,edx ; mov eax,[dest] ; je E ; push esi ; L: test eax,eax ; je S ; (copy 8 bytes) ; S: add ecx,0x8 ; add eax,0x8 ; cmp ecx,edx ; jne L ; pop esi ; E: ret 8
// Dinkumware `_Uninit_copy` (`__fastcall`: first in ecx, last in edx, dest and the by-value
// allocator on the stack) for 8-byte trivially-copyable elements: each element is copied in
// place under the placement-new null test and the advanced destination is returned.
// One of 14 bodies of this shape.
#include <new>

struct SElement8 {                        // dword members: the copy is memberwise (a byte array copies through pointer registers)
    long m_00;
    long m_04;
};

struct CRetailAllocator { };

SElement8* __fastcall UninitCopy_SElement8_007b41c0(SElement8* first, SElement8* last, SElement8* dest, CRetailAllocator allocator)
{
    SElement8* d = dest;
    for (; first != last; ++first, ++d)
        new (d) SElement8(*first);
    return d;
}
