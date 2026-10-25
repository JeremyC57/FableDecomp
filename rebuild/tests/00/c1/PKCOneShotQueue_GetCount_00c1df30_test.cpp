// Behaviour test for 00c1df30: PKCOneShotQueue GetCount / IsFull follow the filled flag
#include <cstdio>
struct SFileReply { bool m_bSuccess; long m_value; SFileReply() : m_bSuccess(true), m_value(0) {} };
template<class T> class PKCQueue { public: virtual long GetCount() = 0; virtual long IsFull() = 0; };
template<class T> class PKCOneShotQueue : public PKCQueue<T> {
public:
    T m_item; long m_bFilled;
    PKCOneShotQueue() : m_bFilled(0) {}
    virtual long GetCount() { return m_bFilled != 0; }
    virtual long IsFull() { return m_bFilled; }
};
int main()
{
    PKCOneShotQueue<SFileReply> q; PKCQueue<SFileReply>* p = &q;
    bool ok = p->GetCount() == 0 && p->IsFull() == 0;
    q.m_bFilled = 1; ok = ok && p->GetCount() == 1 && p->IsFull() == 1;
    if (!ok) { std::printf("00c1df30_TEST FAIL\n"); return 1; }
    std::printf("00c1df30_TEST PASS\n");
    return 0;
}
