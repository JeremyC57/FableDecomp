// Behaviour test for 00c48f80: PKCStaticQueue<SDecoderMessage>::PKCStaticQueue -- capacity stored, capacity records allocated, count and head zeroed
#include <cstdio>
static int g_ctors = 0, g_dtors = 0;
struct SDecoderMessage { unsigned char m_body[0x40]; SDecoderMessage() { m_body[0] = 0; ++g_ctors; } ~SDecoderMessage() { ++g_dtors; } };
template<class T> struct PKCStaticQueue {
    virtual unsigned int GetCount() { return (unsigned int)m_count; }
    long m_capacity; T* m_pRecords; long m_count; long m_head;
    PKCStaticQueue(long capacity) : m_capacity(capacity), m_pRecords(new T[capacity]), m_count(0), m_head(0) { }
    ~PKCStaticQueue() { delete[] m_pRecords; }
};
int main()
{
    if (sizeof(SDecoderMessage) != 0x40 || sizeof(PKCStaticQueue<SDecoderMessage >) != 0x14) { std::printf("00c48f80_TEST FAIL\n"); return 1; }
    {
        PKCStaticQueue<SDecoderMessage > q(8);
        if (q.m_capacity != 8 || q.m_pRecords == 0 || q.m_count != 0 || q.m_head != 0 || q.GetCount() != 0) { std::printf("00c48f80_TEST FAIL\n"); return 1; }
    }
    if (g_ctors != 8 || g_dtors != 8) { std::printf("00c48f80_TEST FAIL\n"); return 1; }
    std::printf("00c48f80_TEST PASS\n");
    return 0;
}
