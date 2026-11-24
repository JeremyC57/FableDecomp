#include <cstdio>
// CAudioSource::UpdateStream (0x00C73050): behaviour model of the row's control flow with stub OpenAL entry points.
// Checks: lazy resolve (both resolver arms, cached after first use); SourcePlay for types 1 / 0x10 only; due position-B request -> 8;
// queueless sound -> 2 while playing else stop (0); processed buffers unqueued one by one (m_queued, DequeueNext, ReportQueueInfo);
// stop on empty queue / null tail item / wrap on a non-looping sound; m_current tracks the tail item; QueueNextBuffer failure -> 0;
// restart when stalled with buffers queued -> 2.
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
typedef void (*PFN_Play)(unsigned long);
typedef void (*PFN_Geti)(unsigned long, long, long*);
typedef void (*PFN_Unq)(unsigned long, long, unsigned long*);

static int g_plays, g_unqueues, g_resolves, g_sysResolves; static long g_processed, g_state;
static void StubPlay(unsigned long) { ++g_plays; }
static void StubGeti(unsigned long, long param, long* v) { *v = param == 0x1016 ? g_processed : g_state; }
static void StubUnq(unsigned long, long count, unsigned long* b) { g_unqueues += (int)count; *b = 7; }
static void* Lookup(const char* n) { return n[2] == 'S' && n[8] == 'P' ? (void*)StubPlay : n[2] == 'G' ? (void*)StubGeti : (void*)StubUnq; }

struct Ctx {
    PFN_Play pfnPlay; PFN_Geti pfnGeti; PFN_Unq pfnUnq; bool UseAL;
    void* GetProcAddress(const char* n) { ++g_resolves; return Lookup(n); }
    void* SysGetProcAddress(const char* n) { ++g_sysResolves; return Lookup(n); }
    void SourcePlay(unsigned long s) {
        if (!pfnPlay) { void* proc = UseAL ? GetProcAddress("alSourcePlay") : SysGetProcAddress("alSourcePlay"); pfnPlay = (PFN_Play)proc; }
        pfnPlay(s);
    }
    void GetSourcei(unsigned long s, long p, long* v) {
        if (!pfnGeti) { void* proc = UseAL ? GetProcAddress("alGetSourcei") : SysGetProcAddress("alGetSourcei"); pfnGeti = (PFN_Geti)proc; }
        pfnGeti(s, p, v);
    }
    void SourceUnqueueBuffers(unsigned long s, long c, unsigned long* b) {
        if (!pfnUnq) { void* proc = UseAL ? GetProcAddress("alSourceUnqueueBuffers") : SysGetProcAddress("alSourceUnqueueBuffers"); pfnUnq = (PFN_Unq)proc; }
        pfnUnq(s, c, b);
    }
};
static Ctx g_ctx;

struct Queue {
    int* m_items; unsigned long m_capacity, m_tail, m_stats0; int dequeues;
    bool IsEmpty() const { return m_capacity != 0 && m_stats0 == 0; }
    void DequeueNext() { ++dequeues; }
};
struct Sound { unsigned long m_flags; Queue m_queue; int reports; bool IsLooping() const { return (m_flags >> 8) & 1; } void ReportQueueInfo() { ++reports; } };
struct Source {
    int m_current; unsigned long m_requests; i64 m_positionB; unsigned long SourceId, m_queued; int applies, setB; long queueResult; int queueCalls; bool lastForce;
    void ApplySound(Sound*, i64) { ++applies; }
    bool SetPositionB(unsigned long, unsigned long) { ++setB; m_requests &= ~0x80u; return true; }
    long QueueNextBuffer(Queue*, i64, bool force) { ++queueCalls; lastForce = force; return queueResult; }
    bool IsPlaying() { long state; g_ctx.GetSourcei(SourceId, 0x1010, &state); return state == 0x1012; }
    long UpdateStream(Sound* sound, i64 now) {
        ApplySound(sound, now);
        Queue* queue = &sound->m_queue;
        unsigned long type = sound->m_flags & 0x7f;
        if (type == 1 || type == 0x10) g_ctx.SourcePlay(SourceId);
        if ((unsigned char)(m_requests >> 7) & 1 && m_positionB <= now) { SetPositionB(0, 0); return 8; }
        if (queue->m_capacity) {
            long processed;
            g_ctx.GetSourcei(SourceId, 0x1016, &processed);
            if (processed != 0) {
                for (; processed > 0; --processed) {
                    --m_queued; unsigned long buffer; g_ctx.SourceUnqueueBuffers(SourceId, 1, &buffer);
                    queue->DequeueNext(); sound->ReportQueueInfo();
                }
                if (queue->IsEmpty() || queue->m_items[queue->m_tail] == 0 || (queue->m_tail == 0 && !sound->IsLooping())) { m_current = 0; return 0; }
                m_current = queue->m_capacity ? queue->m_items[queue->m_tail] : 0;
            }
            if (QueueNextBuffer(queue, now, sound->IsLooping()) != 0) return 0;
            if (!IsPlaying() && m_queued > 0) g_ctx.SourcePlay(SourceId);
            return 2;
        } else {
            if (IsPlaying()) return 2;
        }
        m_current = 0;
        return 0;
    }
};
static int fail(int n) { std::printf("00c73050_TEST FAIL %d\n", n); return 1; }
int main()
{
    int items[3] = { 11, 12, 0 };
    Sound snd; snd.m_flags = 1; snd.m_queue.m_items = items; snd.m_queue.m_capacity = 0; snd.m_queue.m_tail = 1; snd.m_queue.m_stats0 = 2;
    snd.m_queue.dequeues = 0; snd.reports = 0;
    Source src; src.m_current = 99; src.m_requests = 0x80; src.m_positionB = 5; src.SourceId = 3; src.m_queued = 4; src.applies = 0;
    src.setB = 0; src.queueResult = 0; src.queueCalls = 0; src.lastForce = false;
    g_ctx.pfnPlay = 0; g_ctx.pfnGeti = 0; g_ctx.pfnUnq = 0; g_ctx.UseAL = true; g_state = 0x1012; g_processed = 0;
    // type 1 plays (resolved through the context), the due request returns 8
    if (src.UpdateStream(&snd, 5) != 8 || g_plays != 1 || src.setB != 1 || g_resolves != 1 || src.applies != 1) return fail(1);
    // not yet due: queueless sound playing -> 2; type 0x10 plays again (cached, no new resolve)
    snd.m_flags = 0x10; src.m_requests = 0x80; src.m_positionB = 9;
    if (src.UpdateStream(&snd, 5) != 2 || g_plays != 2 || g_resolves != 2 || src.setB != 1) return fail(2);
    // type 2: no play; queueless and stopped -> 0, m_current cleared
    snd.m_flags = 2; g_state = 0x1014;
    if (src.UpdateStream(&snd, 5) != 0 || g_plays != 2 || src.m_current != 0) return fail(3);
    // queue, nothing processed: QueueNextBuffer ok, stalled with buffers queued -> replay, 2 (system resolver path once)
    snd.m_queue.m_capacity = 3; g_ctx.pfnPlay = 0; g_ctx.UseAL = false; snd.m_flags = 0x102;
    if (src.UpdateStream(&snd, 5) != 2 || g_plays != 3 || g_sysResolves != 1 || src.queueCalls != 1 || !src.lastForce) return fail(4);
    // two processed: two unqueues / dequeues / reports, m_queued -2, tail item 12 tracked, QueueNextBuffer failure -> 0
    g_processed = 2; src.queueResult = 1; snd.m_flags = 2;
    if (src.UpdateStream(&snd, 5) != 0 || g_unqueues != 2 || src.m_queued != 2 || snd.m_queue.dequeues != 2 || snd.reports != 2
        || src.m_current != 12 || src.lastForce || g_sysResolves != 2) return fail(5);
    // empty queue -> stop
    snd.m_queue.m_stats0 = 0; src.m_current = 99;
    if (src.UpdateStream(&snd, 5) != 0 || src.m_current != 0 || src.queueCalls != 2) return fail(6);
    // null tail item -> stop
    snd.m_queue.m_stats0 = 1; snd.m_queue.m_tail = 2; src.m_current = 99;
    if (src.UpdateStream(&snd, 5) != 0 || src.m_current != 0) return fail(7);
    // wrapped (tail 0) on a non-looping sound -> stop; on a looping one -> continue, 2 while playing
    snd.m_queue.m_tail = 0; src.m_current = 99;
    if (src.UpdateStream(&snd, 5) != 0 || src.m_current != 0) return fail(8);
    snd.m_flags = 0x102; src.queueResult = 0; g_state = 0x1012; g_processed = 1;
    if (src.UpdateStream(&snd, 5) != 2 || src.m_current != 11 || !src.lastForce || g_plays != 3) return fail(9);
    std::printf("00c73050_TEST PASS\n");
    return 0;
}