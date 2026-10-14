// CEntryListOwner_004ad8d0::`scalar deleting destructor'(unsigned int) -- non-virtual: the entry list at +0x2c cleared (erase from the head's first link, re-link the head, zero the count) and its head node freed, then the flag-tested operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AD8D0, length 89 bytes.
// Callees: the list's erase @ 0x004A9CB0 (the manifest label "LTextTreeWalkThrough::BuildTreeArray" is propagated), free @ 0x00BFEA14, operator delete @ 0x00BFE9BC.
// Shape: VC7.1 emits a non-virtual scalar deleting destructor for a non-polymorphic class with a non-trivial destructor when a translation unit deletes it (no
// vtable store); the list address is formed with `lea esi,[edi+2Ch]` before the count test. The chunk label "CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures"
// is propagated; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct SEntryNode_004ad8d0 {                          // the list node: +4 the first entry link, +8 next, +0xc prev
    long             m_0;
    SEntryNode_004ad8d0* m_pFirst;                    // +0x04
    SEntryNode_004ad8d0* m_pNext;                     // +0x08
    SEntryNode_004ad8d0* m_pPrev;                     // +0x0c
};

struct CRetailEntryList_004ad8d0 {                    // a retail list: head node and count; clear() erases from the head's first link and re-links the head
    SEntryNode_004ad8d0* m_pHead;                     // +0x00
    long             m_count;                      // +0x04
    void EraseFrom(SEntryNode_004ad8d0* node);        // 0x004A9CB0
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
    ~CRetailEntryList_004ad8d0()
    {
        clear();
        if (m_pHead)
            free(m_pHead);
    }
};

struct CEntryListOwner_004ad8d0 {
    unsigned char             _pad[0x2c];
    CRetailEntryList_004ad8d0 m_entries;           // +0x2c
    ~CEntryListOwner_004ad8d0() { }
};

// The delete-expression that makes the compiler emit the scalar deleting destructor.
void Destroy_CEntryListOwner_004ad8d0(CEntryListOwner_004ad8d0* object)
{
    delete object;
}