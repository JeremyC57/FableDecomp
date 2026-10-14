// CMultiStatBar::GetCurrentChange() -- the change of the first queued entry (the list at +0x68: the sentinel's next node's value at +8), or 6 when the bar is in state 1 (+0x28) or the list is empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00646D90, length 23 bytes.
// Leaf: GetCurrentChange
// Shape: the size-optimised module (`push 6 ; pop eax`), no frame, `ret`.
#pragma optimize("s", on)
struct SChangeNode_00646d90 {
    SChangeNode_00646d90* volatile m_pNext;        // +0x00 (re-read after the compare: no CSE in retail)
    SChangeNode_00646d90* m_pPrev;                 // +0x04
    long                  m_change;                // +0x08
};

struct CMultiStatBar {
    unsigned char         _pad00[0x28];
    long                  m_state;                 // +0x28
    unsigned char         _pad2c[0x3c];
    SChangeNode_00646d90* m_pChanges;              // +0x68: the list sentinel
    long GetCurrentChange();
};

long CMultiStatBar::GetCurrentChange()
{
    if (m_state != 1) {
        SChangeNode_00646d90* head = m_pChanges;
        if (head->m_pNext != head)
            return head->m_pNext->m_change;
    }
    return 6;
}
