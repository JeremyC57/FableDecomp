// Behaviour test for CEngineStaticMapFileBlockManager::~CEngineStaticMapFileBlockManager (0x00B443B0): a non-empty map is erased
// and its head reset/freed, then the counted pointer is released (destroyed at zero) and cleared.
#include <cstdio>
#include <cstdlib>

static int g_erased = 0, g_destroyed = 0, g_headsFreed = 0;
struct SCountBlock { long m_references; };
struct CCountedPointer {
    void* m_pObject; SCountBlock* m_pCount;
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) { ++g_destroyed; delete m_pCount; }
        }
        m_pObject = 0; m_pCount = 0;
    }
};
struct SNode { SNode* m_pParent; SNode* m_pLeft; SNode* m_pRight; };
struct CTree {
    SNode* m_pHead; unsigned int m_size;
    void _Erase(SNode*) { g_erased += (int)m_size; }
    ~CTree()
    {
        if (m_size) {
            _Erase(m_pHead->m_pParent);
            m_pHead->m_pLeft = m_pHead; m_pHead->m_pParent = 0; m_pHead->m_pRight = m_pHead; m_size = 0;
        }
        if (m_pHead) { ++g_headsFreed; std::free(m_pHead); }
    }
};
class CEngineStaticMapFileBlockManager { public: CCountedPointer AsyncBankEntry; CTree Blocks; };

int main()
{
    {
        CEngineStaticMapFileBlockManager m;
        m.AsyncBankEntry.m_pObject = &m; m.AsyncBankEntry.m_pCount = new SCountBlock; m.AsyncBankEntry.m_pCount->m_references = 1;
        m.Blocks.m_pHead = (SNode*)std::malloc(sizeof(SNode)); m.Blocks.m_size = 3;
    }
    if (g_erased != 3 || g_destroyed != 1 || g_headsFreed != 1) { std::printf("00b443b0_TEST FAIL\n"); return 1; }
    {
        SCountBlock shared = { 2 };
        CEngineStaticMapFileBlockManager* m = new CEngineStaticMapFileBlockManager;
        m->AsyncBankEntry.m_pObject = 0; m->AsyncBankEntry.m_pCount = &shared;
        m->Blocks.m_pHead = 0; m->Blocks.m_size = 0;
        delete m;
        if (shared.m_references != 1 || g_destroyed != 1 || g_headsFreed != 1) { std::printf("00b443b0_TEST FAIL\n"); return 1; }
    }
    std::printf("00b443b0_TEST PASS\n");
    return 0;
}