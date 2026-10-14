// CEntryListBuffer_00a96da0::~CEntryListBuffer_00a96da0() -- the entry list at +0xc cleared (erase from the head's first link, re-link the head, zero the count) and its head node freed, then the buffer at +0 freed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A96DA0, length 84 bytes.
// Callees: the list's erase @ 0x004507A1 (the manifest label "LTextTreeWalkThrough::BuildTreeArray" is propagated), free @ 0x00BFEA14 twice. Shape: the list's
// inline destructor (from a speed header: `add esp,4` after its free) inside a size-optimised class destructor (`pop ecx` after the last free): inlined code
// keeps the optimisation setting of the function it came from. The chunk label is propagated; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct SEntryNode_00a96da0 {                          // the list node: +4 the first entry link, +8 next, +0xc prev
    long             m_0;
    SEntryNode_00a96da0* m_pFirst;                    // +0x04
    SEntryNode_00a96da0* m_pNext;                     // +0x08
    SEntryNode_00a96da0* m_pPrev;                     // +0x0c
};

struct CRetailEntryList_00a96da0 {                    // a retail list: head node and count; clear() erases from the head's first link and re-links the head
    SEntryNode_00a96da0* m_pHead;                     // +0x00
    long             m_count;                      // +0x04
    void EraseFrom(SEntryNode_00a96da0* node);        // 0x004507A1
    __forceinline void clear()
    {
        if (m_count) {
            EraseFrom(m_pHead->m_pFirst);
            m_pHead->m_pNext = m_pHead;
            m_pHead->m_pFirst = 0;
            m_pHead->m_pPrev = m_pHead;
            m_count = 0;
        }
    }
    __forceinline ~CRetailEntryList_00a96da0()
    {
        clear();
        if (m_pHead)
            free(m_pHead);
    }
};

struct CBuffer_00a96da0 { void* m_pBlock; ~CBuffer_00a96da0() { if (m_pBlock) free(m_pBlock); } };   // a member object: destroyed after the list (reverse declaration order)

struct CEntryListBuffer_00a96da0 {
    CBuffer_00a96da0          m_buffer;            // +0x00
    unsigned char             _pad04[8];
    CRetailEntryList_00a96da0 m_entries;           // +0x0c
    ~CEntryListBuffer_00a96da0();
};

CEntryListBuffer_00a96da0::~CEntryListBuffer_00a96da0()
{
}
