// CQueueOwner_00c0ec60::ClearAll() -- remove the queue's entries from the back until the count at +0x34 is zero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0EC60, length 40 bytes.
// Callees: the remove @ 0x00C0EC10 (on `this`, one argument: the entry; manifest "CTCMapwho_ClearMessageQueue" is propagated). Shape: the entry table at +0x2c
// is indexed with `count - 1` (`mov edx,[ecx+eax*4-4]`), the count is re-read after every call (the remove decrements it); the loop head is aligned with
// a `lea ebx,[ebx]` nop. The chunk label "CDrawGuildSeal::ClearMessages" is propagated; the class is named by address.
struct CQueueOwner_00c0ec60 {
    unsigned char _pad[0x2c];
    void**        m_pEntries;                      // +0x2c
    unsigned char _pad30[4];
    long          m_entryCount;                    // +0x34
    void Remove(void* entry);                      // 0x00C0EC10
    void ClearAll();
};

void CQueueOwner_00c0ec60::ClearAll()
{
    while (m_entryCount != 0)
        Remove(m_pEntries[m_entryCount - 1]);
}