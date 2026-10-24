// Behaviour test for 00c127c0: TracePlaybacks -- under the module lock, refreshes the events, then for every playback slot holding an event builds (and drops) the event description and the slot status for the compiled-out two-argument trace sink
#include <cstdio>

static int g_lockDepth = 0, g_updates = 0, g_described = 0, g_status = 0, g_live = 0, g_order = 0, g_badOrder = 0;
struct CScopedLock {
    CScopedLock(void*) { ++g_lockDepth; }
    ~CScopedLock() { --g_lockDepth; }
};
static unsigned char g_section[0x18];

struct CString {
    int m_id;
    CString(int id) : m_id(id) { ++g_live; }
    CString(const CString& o) : m_id(o.m_id) { ++g_live; }
    ~CString() { --g_live; }
};
inline void TraceOutput(const CString&, const CString&) { }

struct CSoundEventManager;
struct CSoundEvent {
    CString Describe(CSoundEventManager*) { ++g_described; g_order = 1; return CString(1); }
};
struct CPlaybackSlot {
    CSoundEvent* m_pEvent;
    CString GetStatus() { ++g_status; return CString(2); }
};
struct CPlaybackPool {
    CPlaybackSlot m_items[4];
    unsigned long m_count;
    CPlaybackSlot* Get(unsigned long index) { return &m_items[index]; }
};

struct CSoundEventManager {
    CPlaybackPool m_playbacks;
    unsigned long GetNumPlaybacks() { return m_playbacks.m_count; }
    CPlaybackSlot* GetPlayback(unsigned long index) { return m_playbacks.Get(index); }
    void UpdateEvents() { if (g_lockDepth == 1) ++g_updates; }
    void TracePlaybacks()
    {
        CScopedLock lock(g_section);
        UpdateEvents();
        for (unsigned long i = 0; i < GetNumPlaybacks(); ++i) {
            CPlaybackSlot* slot = GetPlayback(i);
            if (slot->m_pEvent)
                TraceOutput(slot->GetStatus(), slot->m_pEvent->Describe(this));
        }
    }
};

int main()
{
    CSoundEventManager m;
    CSoundEvent e;
    m.m_playbacks.m_items[0].m_pEvent = &e;
    m.m_playbacks.m_items[1].m_pEvent = 0;
    m.m_playbacks.m_items[2].m_pEvent = &e;
    m.m_playbacks.m_items[3].m_pEvent = &e;
    m.m_playbacks.m_count = 3;
    m.TracePlaybacks();
    if (g_updates != 1 || g_described != 2 || g_status != 2 || g_live != 0 || g_lockDepth != 0) { std::printf("00c127c0_TEST FAIL\n"); return 1; }
    std::printf("00c127c0_TEST PASS\n");
    return 0;
}