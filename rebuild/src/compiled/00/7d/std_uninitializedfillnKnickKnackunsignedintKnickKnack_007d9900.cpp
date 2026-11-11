// _Uninit_fill_n<T*> -- placement-copy `count` copies of value from first (16-byte memberwise-copied elements, allocator by value), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D9900, oracle length 52 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe R ; mov ecx,[value] ; push regs ; (alignment pad) ; L: test eax,eax ; je N ; <copy 16 bytes [ecx] -> [eax]> ; N: add eax,0x10 ; dec edx ; jne L ; pop regs ; R: ret 8
// Dinkumware `_Uninit_fill_n` for a 16-byte element: each copy is a placement new -- VC emits the
// null check on the placement address -- of the value reference (the allocator rides along by value: a 1-byte class in a 4-byte slot).
// A class with a memberwise user copy constructor: every dword moves through one register.
// The speed TU pads the loop head. Fastcall: first in ecx, count in edx. The manifest label is propagated; the element type is modelled by size. One of 8 bodies of this shape.
#include <new>

struct SElement {
    long m_0; long m_4; long m_8; long m_c;
    SElement(const SElement& other) : m_0(other.m_0), m_4(other.m_4), m_8(other.m_8), m_c(other.m_c) { }   // memberwise user copy: one register per dword
};
struct CRetailAllocator { };

SElement* __fastcall UninitFillN_007d9900(SElement* first, unsigned long count, const SElement& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SElement(value);
    return first;
}