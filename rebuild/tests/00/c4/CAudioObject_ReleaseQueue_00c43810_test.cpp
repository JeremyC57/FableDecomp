// Behaviour test for 00c43810: CAudioObject::ReleaseQueue -- Reset, Clear, callback with the queue state, Release
#include <cstdio>
#include <cstring>
static char g_seq[16] = ""; static void* g_object = 0; static unsigned int g_info[5]; static void* g_ctx = 0;
struct SQueueInfo { unsigned int m_words[5]; };
struct CircularQueue {
    unsigned int m_words[9];
    void Clear() { std::strcat(g_seq, "Q"); }
    void GetInfo(SQueueInfo& info) { std::strcat(g_seq, "G"); for (int i = 0; i < 5; ++i) info.m_words[i] = 51 + i; }
    void Release() { std::strcat(g_seq, "R"); }
};
struct CAudioObject;
typedef void (*TQueueCallback)(CAudioObject* object, SQueueInfo* info, void* context);
static void OnQueue(CAudioObject* object, SQueueInfo* info, void* context) { std::strcat(g_seq, "K"); g_object = object; std::memcpy(g_info, info->m_words, sizeof g_info); g_ctx = context; }
struct CAudioObject {
    unsigned char _pad[0x54]; CircularQueue m_queue; unsigned char _pad78[0xd4 - 0x78]; TQueueCallback m_pfnQueueCallback; void* m_pQueueContext;
    void Reset() { std::strcat(g_seq, "C"); }
    void ReleaseQueue() { Reset(); m_queue.Clear(); if (m_pfnQueueCallback) { SQueueInfo info; m_queue.GetInfo(info); m_pfnQueueCallback(this, &info, m_pQueueContext); } m_queue.Release(); }
};
int main()
{
    CAudioObject o; std::memset(&o, 0, sizeof o); int ctx; o.m_pfnQueueCallback = OnQueue; o.m_pQueueContext = &ctx; o.ReleaseQueue();
    if (std::strcmp(g_seq, "CQGKR") || g_object != &o || g_info[4] != 55 || g_ctx != &ctx) { std::printf("00c43810_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; o.m_pfnQueueCallback = 0; o.ReleaseQueue(); if (std::strcmp(g_seq, "CQR")) { std::printf("00c43810_TEST FAIL\n"); return 1; }
    std::printf("00c43810_TEST PASS\n");
    return 0;
}
