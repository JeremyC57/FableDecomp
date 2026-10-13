// _Uninit_fill_n<CPairCountedRef8*> -- placement-copy `count` 8-byte {key, counted ref} pairs (count bump inlined)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00763EB0, oracle length 46 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe E ; push esi ; mov esi,edx ; mov edx,[esp+8] ; L: test eax,eax ; je N ; mov ecx,[edx] ; mov [eax],ecx ; mov ecx,[edx+4] ; test ecx,ecx ; mov [eax+4],ecx ; je N ; inc dword ptr [ecx] ; N: add eax,8 ; dec esi ; jne L ; pop esi ; E: ret 8
// Dinkumware `_Uninit_fill_n` (__fastcall: dest in ecx, count in edx, the value and the
// allocator on the stack): every slot is copy-constructed in place with placement new (null test
// on the slot) from the value, whose second member is an intrusive counted reference (copying
// it bumps the count block's first dword); the advanced destination is returned. The element
// type is named by its shape. One of
// 20 bodies.
#include <new>

struct SCountBlock { long m_references; };

struct CPairCountedRef8 {
    unsigned long m_key;
    SCountBlock* m_pCount;
    CPairCountedRef8(const CPairCountedRef8& other) : m_key(other.m_key), m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
};

struct CRetailAllocator { };

CPairCountedRef8* __fastcall UninitFillN_CPairCountedRef8_00763eb0(CPairCountedRef8* dest, unsigned long count, const CPairCountedRef8& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++dest)
        new (dest) CPairCountedRef8(value);
    return dest;                           // the advanced pointer is returned (retail keeps dest in eax from the start)
}
