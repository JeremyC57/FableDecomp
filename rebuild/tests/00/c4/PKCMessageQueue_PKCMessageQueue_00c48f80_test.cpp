// Behaviour test for 00c48f80: PKCMessageQueue::PKCMessageQueue -- capacity stored, capacity records built by new[] (one ctor each), count and head zeroed
#include <cstdio>

static int g_ctors = 0, g_dtors = 0;
struct SDecoderMessage {
    SDecoderMessage() : m_type(0) { ++g_ctors; }
    ~SDecoderMessage() { ++g_dtors; }
    int m_type;
    int m_rest[15];
};
class PKCMessageQueue {
public:
    PKCMessageQueue(int capacity) : m_capacity(capacity), m_pRecords(new SDecoderMessage[capacity]), m_count(0), m_head(0) { }
    virtual ~PKCMessageQueue() { delete[] m_pRecords; }
    int             m_capacity;
    SDecoderMessage* m_pRecords;
    int             m_count;
    int             m_head;
};

int main()
{
    if (sizeof(SDecoderMessage) != 0x40) { std::printf("00c48f80_TEST FAIL\n"); return 1; }
    {
        PKCMessageQueue q(8);
        if (q.m_capacity != 8 || q.m_pRecords == 0 || q.m_count != 0 || q.m_head != 0 || g_ctors != 8 || q.m_pRecords[7].m_type != 0) {
            std::printf("00c48f80_TEST FAIL\n"); return 1;
        }
    }
    if (g_dtors != 8) { std::printf("00c48f80_TEST FAIL\n"); return 1; }
    std::printf("00c48f80_TEST PASS\n");
    return 0;
}