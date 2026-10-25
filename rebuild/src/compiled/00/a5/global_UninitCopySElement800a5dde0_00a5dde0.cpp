// __uninitialized_copy<SElement8*> -- placement-copy [first, last) to dest (8-byte POD elements), return the advanced dest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5DDE0, oracle length 44 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  cmp ecx,edx ; mov eax,[dest] ; je E ; push esi ; L: test eax,eax ; je S ; (copy 8 bytes) ; S: add ecx,0x8 ; add eax,0x8 ; cmp ecx,edx ; jne L ; pop esi ; E: ret 8
// STLport 4.6 `__uninitialized_copy(first, last, result, const __false_type&)` (`__fastcall`: first in ecx, last in
// edx, dest and the tag reference on the stack -- the `ret 8`; the tag is never read) for 8-byte trivially-copyable
// elements: each element is copied in place under the placement-new null test (`_Construct`) and the advanced
// destination is returned. Only caller: the STLport vector<8-byte element>::operator= 0x00A5E4D0 (agent420: it pushes
// x's dead argument slot as the __false_type reference). The element type is not named by any table (SElement8 is
// descriptive). One of 14 bodies of this shape.
#include <new>

struct SElement8 {                        // dword members: the copy is memberwise (a byte array copies through pointer registers)
    long m_00;
    long m_04;
};

struct __false_type { };

SElement8* __fastcall UninitCopy_SElement8_00a5dde0(SElement8* first, SElement8* last, SElement8* dest, const __false_type&)
{
    SElement8* d = dest;
    for (; first != last; ++first, ++d)
        new (d) SElement8(*first);
    return d;
}