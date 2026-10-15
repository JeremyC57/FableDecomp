#include <cstdio>

struct CSink { virtual void v00() { } virtual ~CSink() { } };
struct SNode { SNode* m_pNext; SNode* m_pPrev; };

struct CDispatcher {
    CSink* m_pSink; SNode m_head; long m_count; long m_pending; long m_reserved14; long m_reserved18;
    SNode* m_pCursor; SNode* m_pLast;
    CDispatcher(CSink* sink) : m_pSink(sink), m_reserved14(14), m_reserved18(18)
    {
        m_pCursor = &m_head;
        m_pLast = &m_head;
        m_head.m_pNext = &m_head;
        m_head.m_pPrev = &m_head;
        m_count = 0;
        m_pending = 0;
    }
};

int main()
{
    CSink s; CDispatcher d(&s);
    if (d.m_pSink != &s || d.m_head.m_pNext != &d.m_head || d.m_head.m_pPrev != &d.m_head || d.m_count || d.m_pending
        || d.m_pCursor != &d.m_head || d.m_pLast != &d.m_head || d.m_reserved14 != 14 || d.m_reserved18 != 18)
    { std::printf("00c1ae20_TEST FAIL\n"); return 1; }
    std::printf("00c1ae20_TEST PASS\n");
    return 0;
}