// CMultiStatBar::GetCurrentChange() -- the change of the first queued entry (the list at +0x68: the sentinel's next node's value at +8), or 6 when the bar is in state 1 (+0x28) or the list is empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00646D90, length 23 bytes.
// Leaf: GetCurrentChange
// Shape: the size-optimised module (`push 6 ; pop eax`), no frame, `ret`.
#pragma optimize("s", on)
struct SChangeNode_00646d90 {
    SChangeNode_00646d90* m_pNext;                 // +0x00
    SChangeNode_00646d90* m_pPrev;                 // +0x04
    long                  m_change;                // +0x08
};

struct CChangeIterator {
    SChangeNode_00646d90* m_pNode;
    CChangeIterator(SChangeNode_00646d90* node) : m_pNode(node) { }
    bool operator!=(const CChangeIterator& other) const { return m_pNode != other.m_pNode; }
    long& operator*() const { return m_pNode->m_change; }
};
struct CChangeList {
    SChangeNode_00646d90* m_pHead;
    CChangeIterator begin() const { return CChangeIterator(m_pHead->m_pNext); }
    CChangeIterator end() const { return CChangeIterator(m_pHead); }
    bool empty() const { return m_pHead->m_pNext == m_pHead; }
    long& front() const { return m_pHead->m_pNext->m_change; }
};
struct CMultiStatBar {
    unsigned char         _pad00[0x28];
    long                  m_state;                 // +0x28
    unsigned char         _pad2c[0x3c];
    CChangeList           m_changes;               // +0x68
    long GetCurrentChange();
};

long CMultiStatBar::GetCurrentChange()
{
    if (m_state != 1 && !m_changes.empty())
        return m_changes.front();
    return 6;
}
