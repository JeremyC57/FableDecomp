// Behaviour test for 0x009F8A20 graphic-bank list Sizeof: 4 bytes per node plus 4, then 8 per node plus the bank's own size
// when the node holds a bank.
#include <cstdio>

static int gSizeofCalls = 0;

struct CGraphicDataBank {
    long m_size;
    long Sizeof() { ++gSizeofCalls; return m_size; }
};
struct SGraphicDataBankNode {
    SGraphicDataBankNode* m_pNext;
    SGraphicDataBankNode* m_pPrev;
    unsigned char m_init[0x30];
    CGraphicDataBank* m_pBank;
};
struct CGraphicDataBankList {
    SGraphicDataBankNode* m_pHead;
    long Sizeof()
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
};

static void Link(SGraphicDataBankNode* head, SGraphicDataBankNode* node)
{
    node->m_pNext = head;
    node->m_pPrev = head->m_pPrev;
    head->m_pPrev->m_pNext = node;
    head->m_pPrev = node;
}

int main()
{
    SGraphicDataBankNode head, a, b, c;
    head.m_pNext = head.m_pPrev = &head;
    CGraphicDataBankList list;
    list.m_pHead = &head;
    bool ok = list.Sizeof() == 4;
    CGraphicDataBank bankA = { 100 }, bankC = { 30 };
    a.m_pBank = &bankA; b.m_pBank = 0; c.m_pBank = &bankC;
    Link(&head, &a); Link(&head, &b); Link(&head, &c);
    ok = ok && list.Sizeof() == 3 * 4 + 4 + (100 + 8) + 8 + (30 + 8) && gSizeofCalls == 2;
    if (!ok) { std::printf("009f8a20_TEST FAIL\n"); return 1; }
    std::printf("009f8a20_TEST PASS\n");
    return 0;
}