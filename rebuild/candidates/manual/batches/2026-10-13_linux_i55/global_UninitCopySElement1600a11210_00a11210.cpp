// _Uninit_copy<SElement16*> -- placement-copy [first, last) to dest (16-byte POD elements), return the advanced dest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A11210, oracle length 56 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  cmp ecx,edx ; mov eax,[dest] ; je E ; push esi ; L: test eax,eax ; je S ; (copy 16 bytes) ; S: add ecx,0x10 ; add eax,0x10 ; cmp ecx,edx ; jne L ; pop esi ; E: ret 8
// Dinkumware `_Uninit_copy` (`__fastcall`: first in ecx, last in edx, dest and the by-value
// allocator on the stack) for 16-byte trivially-copyable elements: each element is copied in
// place under the placement-new null test and the advanced destination is returned.
// One of 12 bodies of this shape.
#include <new>

struct SElement16 {                        // dword members: the copy is memberwise (a byte array copies through pointer registers)
    long m_00;
    long m_04;
    long m_08;
    long m_0c;
    SElement16(const SElement16& o) : m_00(o.m_00), m_04(o.m_04), m_08(o.m_08), m_0c(o.m_0c) { }   // memberwise copy: direct dword moves, not a block copy through pointer registers
};

struct CRetailAllocator { };

SElement16* __fastcall UninitCopy_SElement16_00a11210(SElement16* first, SElement16* last, SElement16* dest, CRetailAllocator allocator)
{
    SElement16* d = dest;
    for (; first != last; ++first, ++d)
        new (d) SElement16(*first);
    return d;
}
