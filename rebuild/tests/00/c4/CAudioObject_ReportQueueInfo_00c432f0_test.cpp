// Behaviour test for 0x00C432F0 CAudioObject::ReportQueueInfo (NF217, 2026-11-21, q12379): links the row object; the queue's GetInfo
//   0x00C442F0 is stubbed (fills five words), the callback records its arguments. No callback set: nothing is called.
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
    SQueueInfo& GetInfo(SQueueInfo& info);          // 0x00C442F0
    CSoundBuffer**    m_items;                      // +0x00
    unsigned __int64* m_times;                      // +0x04
    unsigned long     m_capacity;                   // +0x08
    unsigned long     m_head;                       // +0x0c
    unsigned long     m_tail;                       // +0x10
    CSoundBuffer*     m_lastItem;                   // +0x14
    unsigned long     m_stats[3];                   // +0x18
};

class CAudioObject;
typedef void (__stdcall* TQueueCallback)(CAudioObject* object, SQueueInfo* info, void* context);

class CVoice;

class CAudioObject {
public:
    void*          m_vptr;                          // +0x00 (vtable 0x012B5A88)
    unsigned char  m_pad04[0x50 - 4];
    CVoice*        m_pVoice;                        // +0x50
    CircularQueue  m_queue;                         // +0x54
    unsigned char  m_pad78[0xd4 - 0x78];
    TQueueCallback m_pfnQueueCallback;              // +0xd4
    void*          m_pQueueContext;                 // +0xd8
    void ReportQueueInfo();
};

static int g_infoCalls = 0;
SQueueInfo& CircularQueue::GetInfo(SQueueInfo& info)
{
    ++g_infoCalls;
    info.m_capacity = m_capacity; info.m_items = m_items; info.m_times = m_times; info.m_tail = m_tail; info.m_lastItem = m_lastItem;
    return info;
}
static int g_calls = 0; static CAudioObject* g_object = 0; static void* g_context = 0; static SQueueInfo g_info;
static void __stdcall OnQueue(CAudioObject* object, SQueueInfo* info, void* context)
{
    ++g_calls; g_object = object; g_context = context; g_info = *info;
}
int main()
{
    static CAudioObject o;
    std::memset(&o, 0, sizeof(o));
    int context = 0;
    o.m_queue.m_capacity = 8; o.m_queue.m_tail = 3; o.m_pQueueContext = &context;
    o.ReportQueueInfo();
    if (g_calls != 0 || g_infoCalls != 0) { std::printf("00c432f0_TEST FAIL\n"); return 1; }
    o.m_pfnQueueCallback = OnQueue;
    o.ReportQueueInfo();
    if (g_calls != 1 || g_infoCalls != 1 || g_object != &o || g_context != &context || g_info.m_capacity != 8 || g_info.m_tail != 3) {
        std::printf("00c432f0_TEST FAIL\n"); return 1;
    }
    std::printf("00c432f0_TEST PASS\n");
    return 0;
}
