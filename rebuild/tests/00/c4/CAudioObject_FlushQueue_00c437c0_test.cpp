// Test: CAudioObject::FlushQueue -- reset, clear the queue, then report the (now empty) queue info to the queue callback with its context, if one is set.
#include <cstdio>
#include <cstring>
static char g_seq[8]; static int g_tail = -1; static void* g_ctx = 0;
struct SQueueInfo { unsigned int capacity; void* items; void* times; unsigned int tail; void* last; };
struct CQueue { unsigned int tail; void Clear() { std::strcat(g_seq, "C"); tail = 0; }
    SQueueInfo& GetInfo(SQueueInfo& i) { i.tail = tail; return i; } };
struct CObj; typedef void (*TCallback)(CObj*, SQueueInfo*, void*);
struct CObj { CQueue m_queue; TCallback m_pfnQueueCallback; void* m_pQueueContext;
    void Reset() { std::strcat(g_seq, "R"); }
    void FlushQueue() { Reset(); m_queue.Clear(); if (m_pfnQueueCallback) { SQueueInfo info; m_queue.GetInfo(info); m_pfnQueueCallback(this, &info, m_pQueueContext); } } };
static void Cb(CObj*, SQueueInfo* i, void* ctx) { std::strcat(g_seq, "B"); g_tail = (int)i->tail; g_ctx = ctx; }
int main()
{
    CObj o; o.m_queue.tail = 9; o.m_pfnQueueCallback = 0; o.m_pQueueContext = &o;
    o.FlushQueue(); bool ok = !std::strcmp(g_seq, "RC") && o.m_queue.tail == 0;
    g_seq[0] = 0; o.m_queue.tail = 4; o.m_pfnQueueCallback = Cb; o.FlushQueue();
    ok = ok && !std::strcmp(g_seq, "RCB") && g_tail == 0 && g_ctx == &o;
    std::printf(ok ? "00c437c0_TEST PASS\n" : "00c437c0_TEST FAIL\n");
    return ok ? 0 : 1;
}