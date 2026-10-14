// CEntryListOwner_00b98230::`scalar deleting destructor'(unsigned int) -- non-virtual: the entry list at +4 cleared (erase from the head's first link, re-link the head, zero the count) and its head node freed, then the flag-tested operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B98230, length 89 bytes.
// Callees: the list's erase @ 0x00B97580 (the manifest label "DestroyLinkedListRecursive_Variant" is propagated), free @ 0x00BFEA14, operator delete @ 0x00BFE9BC.
// Shape: as the landed @ 0x004AD8D0 with the list at +4 (`lea esi,[edi+4]`). The chunk label is propagated; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct SEntryNode_00b98230 {                          // the list node: +4 the first entry link, +8 next, +0xc prev
    long             m_0;
    SEntryNode_00b98230* m_pFirst;                    // +0x04
    SEntryNode_00b98230* m_pNext;                     // +0x08
    SEntryNode_00b98230* m_pPrev;                     // +0x0c
};

struct CRetailEntryList_00b98230 {                    // a retail list: head node and count; clear() erases from the head's first link and re-links the head
    SEntryNode_00b98230* m_pHead;                     // +0x00
    long             m_count;                      // +0x04
    void EraseFrom(SEntryNode_00b98230* node);        // 0x00B97580
    void clear()
    {
        if (m_count) {
            EraseFrom(m_pHead->m_pFirst);
            m_pHead->m_pNext = m_pHead;
            m_pHead->m_pFirst = 0;
            m_pHead->m_pPrev = m_pHead;
            m_count = 0;
        }
    }
    ~CRetailEntryList_00b98230()
    {
        clear();
        if (m_pHead)
            free(m_pHead);
    }
};

struct CEntryListOwner_00b98230 {
    long                      m_0;
    CRetailEntryList_00b98230 m_entries;           // +0x04
    ~CEntryListOwner_00b98230() { }
};

// The delete-expression that makes the compiler emit the scalar deleting destructor.
void Destroy_CEntryListOwner_00b98230(CEntryListOwner_00b98230* object)
{
    delete object;
}
