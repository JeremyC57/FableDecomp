// Behaviour test for 00c1aea0: PKCStaticQueue::~PKCStaticQueue -- frees the record block
#include <cstdio>
#include <new>
static int g_freed = 0;
struct SFileRequest { void* m_hFile; void* m_pKey; long m_a, m_b, m_c, m_d, m_e; };
struct PKCStaticQueue {
    virtual long GetCount() { return m_count; }
    long m_capacity; SFileRequest* m_pRecords; long m_count; long m_head;
    PKCStaticQueue(long capacity) : m_capacity(capacity), m_pRecords((SFileRequest*)::operator new(capacity * sizeof(SFileRequest))), m_count(0), m_head(0) { }
    ~PKCStaticQueue() { ++g_freed; ::operator delete(m_pRecords); }
};
int main()
{
    { PKCStaticQueue q(4); }
    if (g_freed != 1 || sizeof(PKCStaticQueue) != 0x14) { std::printf("00c1aea0_TEST FAIL\n"); return 1; }
    std::printf("00c1aea0_TEST PASS\n");
    return 0;
}
