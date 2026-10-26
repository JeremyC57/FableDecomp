// CGraphicDataBankList::Sizeof() -- persisted size of the graphic-bank list: four bytes per node plus four, then for every node eight bytes plus its bank's own size (CGraphicDataBank::Sizeof @ 0x009FD8D0) when the node holds one
// Leaf: ?Sizeof@CGraphicDataBankList@@QAEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F8A20, 71 bytes, thiscall on the list itself, plain `ret`. No direct callers.
// Descriptive name (no PDB name for this out-of-line copy): it is the body CGraphicBankManager::Sizeof @ 0x009F8380 inlines,
//   with `this` the STLport list<CGraphicBankManager::CGraphicDataBankInfo> (head sentinel at +0, re-read by the second walk's
//   `cmp esi,[ebx]`) instead of the manager (list at +4).
// Layout: Ego_r.pdb CGraphicDataBankInfo (0x38 bytes): Init +0x00, BankHandle +0x2C, GraphicDataBank CCountedPointer +0x30
//   (node + 8 + 0x30 = node + 0x38).
class CGraphicDataBank { public:
    long Sizeof();                                  // 0x009FD8D0
};
struct SGraphicDataBankNode {                       // STLport _List_node<CGraphicDataBankInfo> (0x40 bytes)
    SGraphicDataBankNode* m_pNext;
    SGraphicDataBankNode* m_pPrev;
    unsigned char         m_init[0x30];             // CGraphicDataBankInfo::Init + BankHandle
    CGraphicDataBank*     m_pBank;                  // +0x38 GraphicDataBank
};
struct CGraphicDataBankList {
    SGraphicDataBankNode* m_pHead;                  // +0x00 the sentinel
    long Sizeof();
};

long CGraphicDataBankList::Sizeof()
{
    long count = 0;
    SGraphicDataBankNode* node;
    for (node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext)
        ++count;
    long size = count * 4 + 4;
    for (node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext) {
        long entry = node->m_pBank ? node->m_pBank->Sizeof() + 8 : 8;
        size += entry;
    }
    return size;
}