// CGraphicBankManager::FrameEnd() -- for every bank in the list whose sentinel is at +4 (the bank pointer at the node's +0x38), run CGraphicDataBank::FrameEnd (0x009FD660)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F8200, length 48 bytes.
// Callees: CGraphicDataBank::FrameEnd @ 0x009FD660. Shape: the sentinel-based list walk, `nop`-aligned loop top.
struct CGraphicDataBank_009f8200 {
    void FrameEnd();                               // 0x009FD660
};
struct SBankNode_009f8200 {                        // a list node: next, prev, the payload
    SBankNode_009f8200* m_pNext;
    SBankNode_009f8200* m_pPrev;
    unsigned char       _pad08[0x30];
    CGraphicDataBank_009f8200* m_pBank;             // +0x38 (after the entry record)
};
struct CGraphicBankManager {
    unsigned char       _pad[4];
    SBankNode_009f8200* m_pHead;                   // +0x04 (the sentinel)
    void FrameEnd();
};

void CGraphicBankManager::FrameEnd()
{
    for (SBankNode_009f8200* node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext)
        node->m_pBank->FrameEnd();
}
