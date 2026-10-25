// Behaviour test for 00c1dd50: PKCOneShotQueue constructor -- default reply (success true, value 0), not filled, polymorphic 0x10-byte object
#include <cstdio>
struct SFileReply { bool m_bSuccess; long m_value; SFileReply() : m_bSuccess(true), m_value(0) {} };
template<class T> class PKCQueue { public: virtual long IsFull() = 0; };
template<class T> class PKCOneShotQueue : public PKCQueue<T> {
public:
    T m_item; long m_bFilled;
    PKCOneShotQueue() : m_bFilled(0) {}
    virtual long IsFull() { return m_bFilled; }
};
static bool Check()
{
    PKCOneShotQueue<SFileReply> q; PKCQueue<SFileReply>* p = &q;
    return q.m_item.m_bSuccess && q.m_item.m_value == 0 && p->IsFull() == 0 && sizeof(q) == 0x10;
}
int main()
{
    if (!Check()) { std::printf("00c1dd50_TEST FAIL\n"); return 1; }
    std::printf("00c1dd50_TEST PASS\n");
    return 0;
}
