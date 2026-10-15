// behaviour test for 0x0067BFB0 CPersistContext::TransferReactionMatchList_0067bfb0
#include <cstdio>
#include <cstdlib>
static int g_out = 0, g_in = 0, g_heads = 0, g_crc = 0;
struct SNode { SNode* m_next; SNode* m_prev; void* m_data; };
struct CReactionMatchList {
    SNode* m_node;
    CReactionMatchList() { SNode* head = (SNode*)std::malloc(sizeof(SNode)); head->m_next = head; head->m_prev = head; m_node = head; ++g_heads; }
    ~CReactionMatchList() { std::free(m_node); --g_heads; }
};
struct CDataOutputStream { int id; };
struct CMemoryDataInputStream { int id; };
struct CPersistTraitsReactionMatchList {
    CReactionMatchList m_scratch;
    void TransferBinaryOut(CDataOutputStream* s, CReactionMatchList* v) { if (s && v && m_scratch.m_node->m_next == m_scratch.m_node) ++g_out; }
    void TransferBinaryIn(CMemoryDataInputStream* s, CReactionMatchList* v) { if (s && v) ++g_in; }
};
struct CPersistContext {
    long m_mode; CMemoryDataInputStream* m_pLoadStream; CDataOutputStream* m_pSaveStream;
    void CheckCRC(const char* name) { if (name[0] == 0) ++g_crc; }
    void Transfer(CReactionMatchList* value)
    {
        CheckCRC("");
        CPersistTraitsReactionMatchList traits;
        switch (m_mode) {
        case 3: traits.TransferBinaryOut(m_pSaveStream, value); break;
        case 2: traits.TransferBinaryIn(m_pLoadStream, value); break;
        }
    }
};
int main()
{
    CDataOutputStream out = { 1 }; CMemoryDataInputStream in = { 2 };
    CPersistContext c = { 3, &in, &out };
    CReactionMatchList list;
    c.Transfer(&list);
    int bad = g_out != 1 || g_in != 0 || g_heads != 1;
    c.m_mode = 2; c.Transfer(&list);
    bad |= g_out != 1 || g_in != 1 || g_heads != 1;
    c.m_mode = 0; c.Transfer(&list);
    bad |= g_out != 1 || g_in != 1 || g_heads != 1 || g_crc != 3;
    if (bad) { std::printf("0067bfb0_TEST FAIL\n"); return 1; }
    std::printf("0067bfb0_TEST PASS\n");
    return 0;
}
