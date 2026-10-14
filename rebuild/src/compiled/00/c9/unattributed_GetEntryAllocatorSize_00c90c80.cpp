// CPoolEntryTable_00c90c80::GetEntryAllocatorSize(long) -- the fixed size class of the entry's allocator, 0 for an empty entry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C90C80, oracle length 36 bytes.
// Callees: a __fastcall table lookup @ 0x00C9EA70 (`mov eax,[ecx*4+table] ; ret`, labelled CMemoryAllocator::IsAllocatorFixedSize), sibling jump.
//
// Retail bytes:  mov eax,[ecx+4] ; mov ecx,[ecx+68h] ; mov ecx,[ecx+edx*4+4] ; mov eax,[eax+1Ch] ; mov eax,[eax+0E78h] ; lea edx,[ecx-1] ; test edx,edx ;
//                jge K ; xor eax,eax ; ret ; K: sub ecx,eax ; jmp Lookup
// __fastcall member (`this` in ecx, the entry index in edx, nothing on the stack): the id table
// at +0x68 is a dword count followed by one allocator id per entry (`[ids + index*4 + 4]`); the base id is fetched from this->+4->+0x1c->+0xe78 before the
// test, and `id - 1 < 0` (signed) means no allocator.  Otherwise the id relative to the base is
// looked up through the sibling call.  The manifest's "CBankFile::GetEntryDataSize" label is a
// propagated mis-attribution; the classes are named by address.
unsigned long __fastcall LookupAllocatorSize(unsigned long relativeId);   // 0x00C9EA70

struct SAllocatorIdTable_00c90c80 {
    unsigned long m_count;           // +0x00
    unsigned long m_allocatorIds[1]; // +0x04 .. one id per entry
};

struct CAllocatorContext_00c90c80 {
    unsigned char _pad[0xe78];
    unsigned long m_baseAllocatorId;   // +0xe78
};

struct CPoolOwner_00c90c80 {
    unsigned char _pad[0x1c];
    CAllocatorContext_00c90c80* m_pContext;   // +0x1c
};

struct CPoolEntryTable_00c90c80 {
    unsigned char _pad[4];
    CPoolOwner_00c90c80* m_pOwner;    // +0x04
    unsigned char _pad2[0x68 - 8];
    SAllocatorIdTable_00c90c80* m_pIds;  // +0x68
    unsigned long __fastcall GetEntryAllocatorSize(long index) const;   // 0x00C90C80
};

unsigned long __fastcall CPoolEntryTable_00c90c80::GetEntryAllocatorSize(long index) const
{
    unsigned long base = m_pOwner->m_pContext->m_baseAllocatorId;
    unsigned long id = m_pIds->m_allocatorIds[index];
    long slot = id - 1;
    if (slot < 0)
        return 0;
    return LookupAllocatorSize(id - base);
}