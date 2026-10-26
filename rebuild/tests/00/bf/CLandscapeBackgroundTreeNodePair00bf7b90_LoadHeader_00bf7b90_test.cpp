#include <cstdio>
// Behaviour test for 00bf7b90: both embedded tree nodes load their headers from the same stream, first then second.
class CDataInputStream { public: int m_reads; };
static int g_order[4]; static int g_n = 0;
class CLandscapeBackgroundTreeNode { public:
    int m_id;
    CDataInputStream* m_pLoadedFrom;
    void LoadHeader(CDataInputStream* stream) { m_pLoadedFrom = stream; ++stream->m_reads; g_order[g_n++] = m_id; }
};
struct CLandscapeBackgroundTreeNodePair_00bf7b90 {
    CLandscapeBackgroundTreeNode m_first;
    CLandscapeBackgroundTreeNode m_second;
    void LoadHeader(CDataInputStream* stream);
};
void CLandscapeBackgroundTreeNodePair_00bf7b90::LoadHeader(CDataInputStream* stream)
{
    m_first.LoadHeader(stream);
    m_second.LoadHeader(stream);
}
int main()
{
    CDataInputStream s; s.m_reads = 0;
    CLandscapeBackgroundTreeNodePair_00bf7b90 p;
    p.m_first.m_id = 1; p.m_first.m_pLoadedFrom = 0;
    p.m_second.m_id = 2; p.m_second.m_pLoadedFrom = 0;
    p.LoadHeader(&s);
    if (s.m_reads != 2 || p.m_first.m_pLoadedFrom != &s || p.m_second.m_pLoadedFrom != &s || g_n != 2 || g_order[0] != 1 || g_order[1] != 2) {
        std::printf("00bf7b90_TEST FAIL\n"); return 1;
    }
    std::printf("00bf7b90_TEST PASS\n");
    return 0;
}