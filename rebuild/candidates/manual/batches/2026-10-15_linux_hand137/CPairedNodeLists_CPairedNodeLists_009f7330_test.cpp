// Behaviour test for 0x009F7330 ~CPairedNodeLists: teardown first, then the second list, then the first (nodes, then head).
#include <cstdio>
#include <cstdlib>

static int g_order[8]; static int g_n = 0;
struct CNodeList {
    void* m_pHead; int id;
    void FreeNodes() { g_order[g_n++] = id; }
    ~CNodeList() { FreeNodes(); void* head = m_pHead; if (head != 0) { g_order[g_n++] = id + 10; std::free(head); } }
};
struct CPairedNodeLists {
    long m_word00; CNodeList m_first; CNodeList m_second;
    void Teardown() { g_order[g_n++] = 0; }
    ~CPairedNodeLists() { Teardown(); }
};

int main()
{
    {
        CPairedNodeLists p;
        p.m_first.id = 1; p.m_first.m_pHead = std::malloc(16);
        p.m_second.id = 2; p.m_second.m_pHead = 0;
    }
    if (g_n != 4 || g_order[0] != 0 || g_order[1] != 2 || g_order[2] != 1 || g_order[3] != 11) { std::printf("009f7330_TEST FAIL\n"); return 1; }
    std::printf("009f7330_TEST PASS\n");
    return 0;
}
