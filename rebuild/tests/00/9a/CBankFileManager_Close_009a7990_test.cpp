// Behaviour test for CBankFileManager::Close (0x009A7990): every retail bank is popped (references
// released, last one destroys) and the bank map is cleared.
#include <cstdio>
#include <cstdlib>

static int g_destroyed = 0, g_erased = 0;
struct SCountBlock { long m_references; };
struct CPtr {
    SCountBlock* m_pCount;
    ~CPtr() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) ++g_destroyed; } m_pCount = 0; }
};
struct SNode { SNode* m_pNext; SNode* m_pPrev; CPtr m_value; };
struct CList {
    SNode head;
    bool empty() const { return head.m_pNext == &head; }
    void push_back(SCountBlock* b) { SNode* n = (SNode*)std::malloc(sizeof(SNode)); n->m_value.m_pCount = b; n->m_pNext = &head; n->m_pPrev = head.m_pPrev; head.m_pPrev->m_pNext = n; head.m_pPrev = n; }
    void pop_back() { SNode* node = head.m_pPrev; SNode* next = node->m_pNext; SNode* prev = node->m_pPrev; prev->m_pNext = next; next->m_pPrev = prev; node->m_value.~CPtr(); std::free(node); }
};
struct CTree { unsigned int m_size; void clear() { if (m_size) { g_erased += (int)m_size; m_size = 0; } } };
struct CBankFileManager {
    CTree Banks; CList RetailBanks;
    void Close() { while (!RetailBanks.empty()) RetailBanks.pop_back(); Banks.clear(); }
};

int main()
{
    CBankFileManager m; m.RetailBanks.head.m_value.m_pCount = 0; m.RetailBanks.head.m_pNext = m.RetailBanks.head.m_pPrev = &m.RetailBanks.head; m.Banks.m_size = 4;
    SCountBlock a = { 1 }, b = { 2 };
    m.RetailBanks.push_back(&a); m.RetailBanks.push_back(&b);
    m.Close();
    if (!m.RetailBanks.empty() || g_destroyed != 1 || b.m_references != 1 || g_erased != 4 || m.Banks.m_size != 0) { std::printf("009a7990_TEST FAIL\n"); return 1; }
    m.Close();
    if (g_erased != 4) { std::printf("009a7990_TEST FAIL\n"); return 1; }
    std::printf("009a7990_TEST PASS\n");
    return 0;
}