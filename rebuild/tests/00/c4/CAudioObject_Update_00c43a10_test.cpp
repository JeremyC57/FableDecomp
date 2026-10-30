// Behaviour test for CAudioObject::Update (0x00C43A10): which voice slot is asked for each state, the state change,
// the bit-9 callback, the stop-time queue flush and the dirty-flag fold/clear, through a copy of the logic.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
struct AudioObject;
struct Voice {
    unsigned long answer; int last; i64 t;
    virtual ~Voice() {}
    virtual unsigned long Play(AudioObject*) { last = 7; return answer; }
    virtual unsigned long UpdateQueue(AudioObject*, i64 time) { last = 8; t = time; return answer; }
    virtual unsigned long UpdatePlaying(AudioObject*, i64 time) { last = 9; t = time; return answer; }
    virtual unsigned long UpdatePending(i64 time) { last = 10; t = time; return answer; }
};
static unsigned long g_audioDirtyFlags = 0;
static int g_callbacks = 0; static unsigned long g_cbFlags = 0;
static void Callback(AudioObject*, unsigned long flags, void*) { ++g_callbacks; g_cbFlags = flags; }
struct AudioObject {
    unsigned long m_flags, m_dirtyFlags; Voice* m_pVoice; unsigned long m_capacity, m_count; void* m_pCurrentBuffer;
    void (*m_stateCallback)(AudioObject*, unsigned long, void*); void* m_data; int flushed, reported; unsigned long seenDirty;
    unsigned long GetState() const { return m_flags & 0x7f; }
    bool IsVirtual() const { return ((m_flags >> 9) & 1) != 0; }
    void SetState(unsigned long s) { m_flags = (m_flags & ~0x7fUL) | (s & 0x7f); seenDirty = m_dirtyFlags; }
    void FlushQueue() { ++flushed; }
    void ReportQueueInfo() { ++reported; }
    bool QueueEmpty() const { return m_capacity != 0 && m_count == 0; }
    void Update(i64 time)
    {
        unsigned long state = GetState();
        m_dirtyFlags |= g_audioDirtyFlags;
        unsigned long flags = state;
        if (state == 1) flags = m_pCurrentBuffer ? m_pVoice->Play(this) : m_pVoice->UpdateQueue(this, time);
        else if (state == 0x10) flags = m_pVoice->UpdatePending(time);
        if ((flags & 6) || (state & 6) || IsVirtual()) flags = m_pVoice->UpdatePlaying(this, time);
        if (flags != (m_flags & 0xffff)) {
            SetState(flags);
            if ((flags & 0x200) && !IsVirtual()) {
                m_flags |= 0x200;
                if (m_stateCallback) m_stateCallback(this, m_flags & 0xffff, m_data);
            }
            if (flags == 0 && m_capacity != 0 && !QueueEmpty()) { FlushQueue(); ReportQueueInfo(); }
        }
        m_dirtyFlags = 0;
    }
};
static AudioObject Make(Voice* v, unsigned long state)
{
    AudioObject o; o.m_flags = state; o.m_dirtyFlags = 1; o.m_pVoice = v; o.m_capacity = 4; o.m_count = 2;
    o.m_pCurrentBuffer = 0; o.m_stateCallback = Callback; o.m_data = 0; o.flushed = o.reported = 0; o.seenDirty = 0;
    return o;
}
int main()
{
    bool ok = true;
    Voice v; v.last = 0; v.t = 0;
    g_audioDirtyFlags = 0x40;
    // state 1 with a current buffer: slot 7, then playing (2) -> slot 9 decides
    AudioObject a = Make(&v, 1); int dummy; a.m_pCurrentBuffer = &dummy; v.answer = 2;
    a.Update(123);
    ok = ok && v.last == 9 && v.t == 123 && a.GetState() == 2 && a.seenDirty == 0x41 && a.m_dirtyFlags == 0;
    // state 1 without a buffer: slot 8, answer 0 (stopped) with a non-empty queue -> flush + report
    AudioObject b = Make(&v, 1); v.answer = 0; v.last = 0;
    b.Update(77);
    ok = ok && v.last == 8 && b.GetState() == 0 && b.flushed == 1 && b.reported == 1;
    // stop with an empty queue: no flush
    AudioObject c = Make(&v, 1); c.m_count = 0; v.answer = 0;
    c.Update(5);
    ok = ok && c.flushed == 0;
    // state 0x10: slot 10; answer 0x201 (virtual bit appears) -> callback with bit 9 set
    AudioObject d = Make(&v, 0x10); v.answer = 0x201; v.last = 0; g_callbacks = 0;
    d.Update(9);
    ok = ok && v.last == 10 && g_callbacks == 1 && (g_cbFlags & 0x200) && (d.m_flags & 0x200);
    // unchanged flags: no state change, dirty flags still cleared
    AudioObject e = Make(&v, 8); v.answer = 8; e.seenDirty = 99;
    e.Update(1);
    ok = ok && e.seenDirty == 99 && e.m_dirtyFlags == 0;
    if (!ok) { std::printf("00c43a10_TEST FAIL\n"); return 1; }
    std::printf("00c43a10_TEST PASS\n");
    return 0;
}