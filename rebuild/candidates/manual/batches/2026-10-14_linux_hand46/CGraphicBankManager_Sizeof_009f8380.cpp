// CGraphicBankManager::Sizeof() -- four bytes per list node plus four, plus for every node eight bytes and the bank's own size (CGraphicDataBank::Sizeof @ 0x009FD8D0) when the node holds a bank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F8380, length 80 bytes.
// Callees: CGraphicDataBank::Sizeof @ 0x009FD8D0. Shape: a counting walk (`inc edx`) folded into `lea edi,[edx*4+4]`, then the summing walk with the `+8` on both arms.
struct CGraphicDataBank_009f8200 {
    long Sizeof();                                 // 0x009FD8D0
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
    long Sizeof();
};

long CGraphicBankManager::Sizeof()
{
    long count = 0;
    SBankNode_009f8200* node;
    for (node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext)
        ++count;
    long size = count * 4 + 4;
    for (node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext) {
        long entry = node->m_pBank ? node->m_pBank->Sizeof() + 8 : 8;   // the entry record and its bank (both arms land in eax, then one add)
        size += entry;
    }
    return size;
}
