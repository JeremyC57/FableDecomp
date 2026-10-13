// CRemovalTable_0079f260::Remove(key) -- cdecl helper @ 0x0042A34E over the table at [this]+0x4/0x8/0xc, free the returned block, drop the count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079F260, oracle length 50 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi] ; lea ecx,[eax+0xc] ; push ecx ; lea edx,[eax+0x8] ; push edx ; add eax,0x4 ; push eax ; mov eax,[esp+14h] ; push eax ; call Helper ; add esp,10h ; test eax,eax ; je K ; push eax ; call free ; add esp,4 ; K: dec dword ptr [esi+4] ; pop esi ; ret 4
// Container removal: the __cdecl helper @ 0x0042A34E gets the key and the addresses of three
// consecutive fields of the table object at [this] (the manifest's `CMemoryAllocatorVariableSize`
// label for it is propagated); whatever block it hands back is released with free @ 0x00BFEA14,
// then the element count at +4 is decremented. One of 20 bodies of this shape.
extern "C" void __cdecl free(void* block);

struct STable {
    unsigned char _pad[0x4];
    void* m_first;                         // +0x4
    void* m_second;                        // +0x8
    void* m_third;                         // +0xc
};

void* __cdecl Helper_0042a34e(void* key, void** first, void** second, void** third);   // 0x0042A34E

struct CRemovalTable_0079f260 {
    STable* m_pTable;                      // +0
    long m_count;                          // +4
    void Remove_0079f260(void* key);
};

void CRemovalTable_0079f260::Remove_0079f260(void* key)
{
    void* freed = Helper_0042a34e(key, &m_pTable->m_first, &m_pTable->m_second, &m_pTable->m_third);
    if (freed)
        free(freed);
    --m_count;
}