// CMeshBankManager::FlushUnusedBanks() -- walk the bank-info list at +8 (a sentinel node; the info pointer at +0x10 of each node) and erase the first node whose info is in state 1 (`list::erase(iterator)` @ 0x00A0A650: the iterator built by value in the argument slot, the returned iterator discarded into a frame slot), then stop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A09E70, length 55 bytes.
// Leaf: FlushUnusedBanks
// Callees: 0x00A0A650 (the list's erase, thiscall on the list at +8 with the hidden result pointer and the iterator by value). Shape: a 4-byte frame slot for the result, the list in ecx, the sentinel in esi, the loop head aligned, `ret`.
struct SBankInfo_00a09e70 {
    long m_state;                                  // +0x00: 1 = unused
};
struct SInfoNode_00a09e70 {
    SInfoNode_00a09e70* m_pNext;                   // +0x00
    SInfoNode_00a09e70* m_pPrev;                   // +0x04
    unsigned char       _pad08[8];
    SBankInfo_00a09e70* m_pInfo;                   // +0x10
};
struct CListIterator_00a09e70 {                    // 4 bytes with user constructors: passed by value in the argument area, returned by hidden pointer
    SInfoNode_00a09e70* m_pNode;
    CListIterator_00a09e70(SInfoNode_00a09e70* node) : m_pNode(node) { }
    CListIterator_00a09e70(const CListIterator_00a09e70& other) : m_pNode(other.m_pNode) { }
};
struct CRetailList_00a09e70 {
    SInfoNode_00a09e70* m_pHead;                   // the sentinel
    CListIterator_00a09e70 erase(CListIterator_00a09e70 where);   // 0x00A0A650
};

struct CMeshBankManager {
    long                 m_0;
    long                 m_4;
    CRetailList_00a09e70 m_infos;                  // +0x08
    void FlushUnusedBanks();
};

void CMeshBankManager::FlushUnusedBanks()
{
    for (SInfoNode_00a09e70* node = m_infos.m_pHead->m_pNext; node != m_infos.m_pHead; node = node->m_pNext) {
        SBankInfo_00a09e70* info = node->m_pInfo;
        if (info && info->m_state == 1) {
            m_infos.erase(CListIterator_00a09e70(node));
            return;
        }
    }
}
