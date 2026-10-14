// CGraphicBankManager::ProcessLoading(long budget, double time) -- two passes over the banks (sentinel at +4, bank at the node's +0x38): first each bank's loader step (0x009D68B0), then each bank's resource list at +0x164 processed with the budget and the time (0x009FC410)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F8230, length 96 bytes.
// Callees: the bank step @ 0x009D68B0 (labelled "_Dest_val<...>", propagated), the resource-list processing @ 0x009FC410 (on bank+0x164: the budget, the double;
// labelled "_Cons_val<...>", propagated). Shape: two sentinel walks, the budget in ebx, the double re-read from the argument each iteration and pushed as two words, `ret 0Ch`.
struct CResourceList_009f8230 {
    void ProcessLoading(long budget, double time); // 0x009FC410
};
struct CGraphicDataBank_009f8200 {
    unsigned char           _pad[0x164];
    CResourceList_009f8230  m_resources;           // +0x164
    void StepLoading();                            // 0x009D68B0
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
    void ProcessLoading(long budget, double time);
};

void CGraphicBankManager::ProcessLoading(long budget, double time)
{
    SBankNode_009f8200* node;
    for (node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext)
        node->m_pBank->StepLoading();
    for (node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext)
        node->m_pBank->m_resources.ProcessLoading(budget, time);
}
