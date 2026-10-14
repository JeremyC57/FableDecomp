// _Uninit_copy<T*> -- placement-copy [first, last) to dest (4-byte POD elements, allocator by value), return the advanced destination
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085DBD0, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  cmp ecx,edx ; mov eax,[dest] ; je R ; push esi ; (alignment pad) ; L: test eax,eax ; je N ; <copy 4 bytes [ecx] -> [eax]> ; N: add ecx,0x4 ; add eax,0x4 ; cmp ecx,edx ; jne L ; pop esi ; R: ret 8
// Dinkumware `_Uninit_copy` for a 4-byte element: each copy is a placement new -- VC emits the null
// check on the placement address -- from the source range; the allocator rides along by value
// (a 1-byte class in a 4-byte slot). A POD of dwords.
// The speed TU pads the loop head. Fastcall: first in ecx, last in edx. The manifest label is propagated; the element type is modelled by size. One of 6 bodies of this shape.
#include <new>

struct SElement { long m_0; };
struct CRetailAllocator { };

SElement* __fastcall UninitCopy_0085dbd0(const SElement* first, const SElement* last, SElement* dest, CRetailAllocator allocator)
{
    for (; first != last; ++first, ++dest)
        new (dest) SElement(*first);
    return dest;
}