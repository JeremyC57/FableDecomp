// _Uninit_copy<T*> -- placement-copy [first, last) to dest (12-byte memberwise-copied elements, allocator by value), return the advanced destination
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006EFDB0, oracle length 50 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  cmp ecx,edx ; mov eax,[dest] ; je R ; push esi ; (alignment pad) ; L: test eax,eax ; je N ; <copy 12 bytes [ecx] -> [eax]> ; N: add ecx,0xc ; add eax,0xc ; cmp ecx,edx ; jne L ; pop esi ; R: ret 8
// Dinkumware `_Uninit_copy` for a 12-byte element: each copy is a placement new -- VC emits the null
// check on the placement address -- from the source range; the allocator rides along by value
// (a 1-byte class in a 4-byte slot). A class with a memberwise user copy constructor: every dword moves through one register.
// The speed TU pads the loop head. Fastcall: first in ecx, last in edx. The manifest label is propagated; the element type is modelled by size. One of 5 bodies of this shape.
#include <new>

struct SElement {
    long m_0; long m_4; long m_8;
    SElement(const SElement& other) : m_0(other.m_0), m_4(other.m_4), m_8(other.m_8) { }   // memberwise user copy: one register per dword
};
struct CRetailAllocator { };

SElement* __fastcall UninitCopy_006efdb0(const SElement* first, const SElement* last, SElement* dest, CRetailAllocator allocator)
{
    for (; first != last; ++first, ++dest)
        new (dest) SElement(*first);
    return dest;
}
