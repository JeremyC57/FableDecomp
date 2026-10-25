// Behaviour test for 00c49af0: PKCStaticQueue<CCodaFrame*>::~PKCStaticQueue -- frees the pointer slots with delete[]
#include <cstdio>
static int g_freed = 0;
struct CCodaFrame { int m_id; };
template<class T> struct PKCStaticQueue {
    virtual unsigned int GetCount() { return (unsigned int)m_count; }
    long m_capacity; T* m_pRecords; long m_count; long m_head;
    PKCStaticQueue(long capacity) : m_capacity(capacity), m_pRecords(new T[capacity]), m_count(0), m_head(0) { }
    ~PKCStaticQueue() { ++g_freed; delete[] m_pRecords; }
};
int main()
{
    { PKCStaticQueue<CCodaFrame*> q(8); q.m_pRecords[0] = 0; }
    if (g_freed != 1 || sizeof(PKCStaticQueue<CCodaFrame*>) != 0x14) { std::printf("00c49af0_TEST FAIL\n"); return 1; }
    std::printf("00c49af0_TEST PASS\n");
    return 0;
}
