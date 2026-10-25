// Behaviour test for 00c4bb00: PKCStaticQueue<SReadRequest>::~PKCStaticQueue -- frees the one-word records
#include <cstdio>
static int g_freed = 0;
struct SReadRequest { unsigned int m_id; };
template<class T> struct PKCStaticQueue {
    virtual unsigned int GetCount() { return (unsigned int)m_count; }
    long m_capacity; T* m_pRecords; long m_count; long m_head;
    PKCStaticQueue(long capacity) : m_capacity(capacity), m_pRecords(new T[capacity]), m_count(0), m_head(0) { }
    ~PKCStaticQueue() { ++g_freed; delete[] m_pRecords; }
};
int main()
{
    { PKCStaticQueue<SReadRequest> q(4); q.m_pRecords[3].m_id = 7; }
    if (g_freed != 1 || sizeof(PKCStaticQueue<SReadRequest>) != 0x14 || sizeof(SReadRequest) != 4) { std::printf("00c4bb00_TEST FAIL\n"); return 1; }
    std::printf("00c4bb00_TEST PASS\n");
    return 0;
}
