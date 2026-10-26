#include <cstdio>
#include <cstdlib>
// Behaviour model of CMeshBankManager::CMeshBankManager @ 0x00A0A110: the list member gets a self-linked sentinel.
struct CBase { virtual void v00() {} CBase() {} };
struct CMeshBankListener { virtual void v00() {} CMeshBankListener() {} };
struct CRetailList0x14 {
    struct SNode { SNode* next; SNode* prev; unsigned char value[12]; };
    SNode* head; long size;
    CRetailList0x14() : head(0) { SNode* node = (SNode*)std::malloc(0x14); node->next = node; node->prev = node; head = node; }
};
class CMeshBankManager : public CBase, public CMeshBankListener { public: CRetailList0x14 m_banks; virtual void v00() {} CMeshBankManager() : CBase(), CMeshBankListener(), m_banks() {} };
int main()
{
    CMeshBankManager m;
    bool ok = m.m_banks.head != 0 && m.m_banks.head->next == m.m_banks.head && m.m_banks.head->prev == m.m_banks.head;
    std::free(m.m_banks.head);
    if (!ok) { std::printf("00a0a110_TEST FAIL\n"); return 1; }
    std::printf("00a0a110_TEST PASS\n");
    return 0;
}