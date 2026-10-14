// _Uninit_copy<T*> -- placement-copy [first, last) to dest (8-byte POD elements, allocator by value, size TU), return the advanced destination
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BA833, oracle length 37 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  cmp ecx,edx ; mov eax,[dest] ; je R ; push esi ; L: test eax,eax ; je N ; <copy 8 bytes [ecx] -> [eax]> ; N: add ecx,0x8 ; add eax,0x8 ; cmp ecx,edx ; jne L ; pop esi ; R: ret 8
// Dinkumware `_Uninit_copy` for a 8-byte element: each copy is a placement new -- VC emits the null
// check on the placement address -- from the source range; the allocator rides along by value
// (a 1-byte class in a 4-byte slot). A POD of dwords.
// Size optimisation leaves the loop unaligned. Fastcall: first in ecx, last in edx. The manifest label is propagated; the element type is modelled by size. One of 5 bodies of this shape.
#pragma optimize("s", on)

#include <new>

struct SElement { long m_0; long m_4; };
struct CRetailAllocator { };

SElement* __fastcall UninitCopy_005ba833(const SElement* first, const SElement* last, SElement* dest, CRetailAllocator allocator)
{
    for (; first != last; ++first, ++dest)
        new (dest) SElement(*first);
    return dest;
}
