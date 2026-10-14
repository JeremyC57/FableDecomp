// CLinkedItem_00c4cef0::Unlink() -- take this item out of its doubly linked chain: the previous item (+4) gets this item's next (+8), the next item gets this item's previous, then both links are cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4CEF0, length 37 bytes.
// Callees: none. Shape: the zero register edx serves both null tests (`cmp eax,edx`) and the two clears; esi carries the cross links. The chunk label
// "FinishWithDynamicTextures" is propagated; the class is named by address.
struct CLinkedItem_00c4cef0 {
    long               m_0;
    CLinkedItem_00c4cef0* m_pPrev;                 // +0x04
    CLinkedItem_00c4cef0* m_pNext;                 // +0x08
    void Unlink();
};

void CLinkedItem_00c4cef0::Unlink()
{
    if (m_pPrev)
        m_pPrev->m_pNext = m_pNext;
    if (m_pNext)
        m_pNext->m_pPrev = m_pPrev;
    m_pPrev = 0;
    m_pNext = 0;
}
