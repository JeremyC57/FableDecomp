// _Uninit_fill_n<CCountedPointer*> -- placement-copy `count` counted pointers (object + count block, count incremented when present), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD1690, oracle length 46 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe E ; push esi ; mov esi,edx ; mov edx,[value] ; L: test eax,eax ; je S ; mov ecx,[edx] ; mov [eax],ecx ; mov ecx,[edx+4] ; test ecx,ecx ; mov [eax+4],ecx ; je S ; inc dword ptr [ecx] ; S: add eax,8 ; dec esi ; jne L ; pop esi ; E: ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// value reference and the by-value allocator on the stack) for 8-byte counted-pointer
// elements: the copy constructor is inlined under the placement-new null test (object and
// count-block pointers copied, the block's count incremented when present); the advanced
// destination is returned. The manifest label names a {__int64, CCountedPointer} pair
// instantiation and is propagated; the bytes copy one 8-byte counted pointer. One of 14 bodies.
#include <new>

struct SCountBlock {
    long m_references;                     // +0
};

struct CCountedPointer {
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
};

struct CRetailAllocator { };

CCountedPointer* __fastcall UninitFillN_CCountedPointer_00ad1690(CCountedPointer* first, unsigned long count, const CCountedPointer& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) CCountedPointer(value);
    return first;
}