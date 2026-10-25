#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
typedef long long __int64;
#endif
struct CItem;

class CircularQueue {
public:
    CItem**           m_items;             // +0x00
    __int64*          m_times;             // +0x04
    unsigned long     m_capacity;          // +0x08
    bool GetItemAtIndex(unsigned long index);   // 0x00C44430
    CItem* GetItem(unsigned long index);        // 0x00C442C0
    __int64 GetTime(unsigned long index);       // 0x00C442D0
};

struct CAudioContext {
    long GetError();                                                                // 0x00C4D960
    void SourceQueueBuffers(unsigned long source, long count, const unsigned long* buffers);   // 0x00C57690
    long CheckError();                                                              // 0x00C4DBE0
};
extern CAudioContext g_AudioContext;       // 0x0143BDB0

struct CAudioSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual long PrepareBuffer(CItem* item, unsigned long* bufferId);    // slot 19 (+0x4C)
    unsigned char  _pad04[0x54];
    unsigned long  SourceId;               // +0x58
    unsigned long  m_next;                 // +0x5C
    unsigned long  m_queued;               // +0x60
    long QueueNextBuffer(CircularQueue* queue, __int64 now, bool force);
};

long CAudioSource::QueueNextBuffer(CircularQueue* queue, __int64 now, bool force)
{
    if (m_queued != queue->m_capacity && queue->GetItemAtIndex(m_next) && (m_next > 0 || force)) {
        CItem* item = queue->GetItem(m_next);
        if (queue->GetTime(m_next) <= now) {
            unsigned long bufferId;
            long result = PrepareBuffer(item, &bufferId);
            if (result < 0)
                return result;
            g_AudioContext.GetError();
            g_AudioContext.SourceQueueBuffers(SourceId, 1, &bufferId);
            result = g_AudioContext.CheckError();
            if (result < 0)
                return result;
            ++m_queued;
            m_next = (m_next + 1) % queue->m_capacity;
        }
        return 0;
    }
    return 0;
}
static int fail() { std::printf("00c72c40_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
bool CircularQueue::GetItemAtIndex(unsigned long index) { return m_items[index] != 0; }
CItem* CircularQueue::GetItem(unsigned long index) { return m_items[index]; }
__int64 CircularQueue::GetTime(unsigned long index) { return m_times[index]; }
CAudioContext g_AudioContext;
static int g_getErrors = 0, g_queues = 0; static unsigned long g_source, g_buffer; static long g_count; static long g_checkResult = 0;
long CAudioContext::GetError() { ++g_getErrors; return 0; }
void CAudioContext::SourceQueueBuffers(unsigned long source, long count, const unsigned long* buffers)
{ ++g_queues; g_source = source; g_count = count; g_buffer = buffers[0]; }
long CAudioContext::CheckError() { return g_checkResult; }
void CAudioSource::v00() { } void CAudioSource::v01() { } void CAudioSource::v02() { } void CAudioSource::v03() { }
void CAudioSource::v04() { } void CAudioSource::v05() { } void CAudioSource::v06() { } void CAudioSource::v07() { }
void CAudioSource::v08() { } void CAudioSource::v09() { } void CAudioSource::v10() { } void CAudioSource::v11() { }
void CAudioSource::v12() { } void CAudioSource::v13() { } void CAudioSource::v14() { } void CAudioSource::v15() { }
void CAudioSource::v16() { } void CAudioSource::v17() { } void CAudioSource::v18() { }
static int g_prepares = 0; static long g_prepareResult = 0; static CItem* g_prepared;
long CAudioSource::PrepareBuffer(CItem* item, unsigned long* bufferId) { ++g_prepares; g_prepared = item; *bufferId = 77; return g_prepareResult; }
int main()
{
    static int a, b, c;
    CItem* items[3] = { reinterpret_cast<CItem*>(&a), reinterpret_cast<CItem*>(&b), reinterpret_cast<CItem*>(&c) };
    __int64 times[3] = { 100, 200, 300 };
    CircularQueue q; q.m_items = items; q.m_times = times; q.m_capacity = 3;
    CAudioSource s; s.SourceId = 9; s.m_next = 0; s.m_queued = 0;
    if (s.QueueNextBuffer(&q, 1000, false) != 0 || g_prepares != 0) return fail();          // slot 0 needs force
    if (s.QueueNextBuffer(&q, 50, true) != 0 || g_prepares != 0) return fail();            // not due yet
    if (s.QueueNextBuffer(&q, 100, true) != 0 || g_prepares != 1 || g_prepared != items[0]) return fail();
    if (g_queues != 1 || g_source != 9 || g_count != 1 || g_buffer != 77 || g_getErrors != 1) return fail();
    if (s.m_queued != 1 || s.m_next != 1) return fail();
    g_prepareResult = -5;
    if (s.QueueNextBuffer(&q, 1000, false) != -5 || g_queues != 1 || s.m_next != 1) return fail();   // prepare failed
    g_prepareResult = 0; g_checkResult = -7;
    if (s.QueueNextBuffer(&q, 1000, false) != -7 || g_queues != 2 || s.m_queued != 1 || s.m_next != 1) return fail();
    g_checkResult = 0;
    s.m_next = 2;
    if (s.QueueNextBuffer(&q, 1000, false) != 0 || s.m_next != 0 || s.m_queued != 2) return fail();   // wraps to slot 0
    s.m_queued = 3; s.m_next = 1; g_prepares = 0;
    if (s.QueueNextBuffer(&q, 1000, true) != 0 || g_prepares != 0) return fail();          // queue full
    s.m_queued = 0; items[1] = 0;
    if (s.QueueNextBuffer(&q, 1000, true) != 0 || g_prepares != 0) return fail();          // empty slot
    std::printf("00c72c40_TEST PASS\n");
    return 0;
}