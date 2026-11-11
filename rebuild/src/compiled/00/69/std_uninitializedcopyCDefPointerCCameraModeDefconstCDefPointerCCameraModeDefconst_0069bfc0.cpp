// _Uninit_copy<T*> -- placement-copy [first, last) to dest (4-byte counted-pointer elements, allocator by value), return the advanced destination
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069BFC0, oracle length 45 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  cmp ecx,edx ; mov eax,[dest] ; je R ; push esi ; (alignment pad) ; L: test eax,eax ; je N ; <copy 4 bytes [ecx] -> [eax] ; addref> ; N: add ecx,0x4 ; add eax,0x4 ; cmp ecx,edx ; jne L ; pop esi ; R: ret 8
// Dinkumware `_Uninit_copy` for a 4-byte element: each copy is a placement new -- VC emits the null
// check on the placement address -- from the source range; the allocator rides along by value
// (a 1-byte class in a 4-byte slot). The element is a counted pointer whose copy constructor bumps the block's reference count when the block is present.
// The speed TU pads the loop head. Fastcall: first in ecx, last in edx. The manifest label is propagated; the element type is modelled by size. One of 7 bodies of this shape.
#include <new>

struct SCountBlock { long m_a; long m_references; };
struct SElement {                          // counted pointer: the copy bumps the block's count at +4
    SCountBlock* m_pCount;
    SElement(const SElement& other) : m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
};
struct CRetailAllocator { };

SElement* __fastcall UninitCopy_0069bfc0(const SElement* first, const SElement* last, SElement* dest, CRetailAllocator allocator)
{
    for (; first != last; ++first, ++dest)
        new (dest) SElement(*first);
    return dest;
}