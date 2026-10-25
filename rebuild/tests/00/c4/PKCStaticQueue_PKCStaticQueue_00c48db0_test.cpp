// Behaviour test for 00c48db0: PKCStaticQueue<SDecoderMessage>::~PKCStaticQueue -- destroys every record, then frees the block
#include <cstdio>
static int g_dtors = 0;
struct SDecoderMessage { unsigned char m_body[0x40]; SDecoderMessage() { } ~SDecoderMessage() { ++g_dtors; } };
template<class T> struct PKCStaticQueue {
    virtual unsigned int GetCount() { return (unsigned int)m_count; }
    long m_capacity; T* m_pRecords; long m_count; long m_head;
    PKCStaticQueue(long capacity) : m_capacity(capacity), m_pRecords(new T[capacity]), m_count(0), m_head(0) { }
    ~PKCStaticQueue() { delete[] m_pRecords; }
};
int main()
{
    { PKCStaticQueue<SDecoderMessage> q(3); }
    if (g_dtors != 3 || sizeof(PKCStaticQueue<SDecoderMessage>) != 0x14 || sizeof(SDecoderMessage) != 0x40) { std::printf("00c48db0_TEST FAIL\n"); return 1; }
    { PKCStaticQueue<SDecoderMessage> q(1); q.m_pRecords = 0; delete[] new SDecoderMessage[1]; }
    if (g_dtors != 4) { std::printf("00c48db0_TEST FAIL\n"); return 1; }
    std::printf("00c48db0_TEST PASS\n");
    return 0;
}
