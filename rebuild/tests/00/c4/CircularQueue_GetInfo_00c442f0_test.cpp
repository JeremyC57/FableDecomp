// Behaviour test for 0x00C442F0 CircularQueue::GetInfo (NF218, 2026-11-21, q12385): links the row object; checks the five
//   copied words and that the caller's record is returned.
#include <cstdio>
#include <cstring>
struct SQueueInfo {                                 // 20 bytes, filled by CircularQueue::GetInfo 0x00C442F0
    unsigned long m_capacity;                       // +0x00 (queue +0x08)
    void*         m_items;                          // +0x04 (queue +0x00)
    void*         m_times;                          // +0x08 (queue +0x04)
    unsigned long m_tail;                           // +0x0c (queue +0x10)
    void*         m_lastItem;                       // +0x10 (queue +0x14)
};

class CSoundBuffer;

class CircularQueue {                               // the audio object's buffer queue (+0x54)
public:
    SQueueInfo& GetInfo(SQueueInfo& info);
    CSoundBuffer**    m_items;                      // +0x00
    unsigned __int64* m_times;                      // +0x04
    unsigned long     m_capacity;                   // +0x08
    unsigned long     m_head;                       // +0x0c
    unsigned long     m_tail;                       // +0x10
    CSoundBuffer*     m_lastItem;                   // +0x14
    unsigned long     m_stats[3];                   // +0x18
};

int main()
{
    CSoundBuffer* items[4]; unsigned __int64 times[4]; char last;
    CircularQueue q; std::memset(&q, 0, sizeof q);
    q.m_items = items; q.m_times = times; q.m_capacity = 4; q.m_head = 99; q.m_tail = 3; q.m_lastItem = (CSoundBuffer*)&last;
    SQueueInfo info; std::memset(&info, 0, sizeof info);
    SQueueInfo& r = q.GetInfo(info);
    if (&r != &info || info.m_capacity != 4 || info.m_items != (void*)items || info.m_times != (void*)times || info.m_tail != 3
        || info.m_lastItem != (void*)&last) { std::printf("00c442f0_TEST FAIL\n"); return 1; }
    std::printf("00c442f0_TEST PASS\n");
    return 0;
}
