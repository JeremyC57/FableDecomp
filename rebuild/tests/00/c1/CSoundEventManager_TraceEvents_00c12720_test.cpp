// Behaviour test for 00c12720: TraceEvents -- under the module lock, refreshes the events, then builds (and drops) the description of every active event for the compiled-out trace sink
#include <cstdio>

static int g_lockDepth = 0, g_updates = 0, g_described = 0, g_live = 0;
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
inline void TraceOutput(const CString&) { }

struct CSoundEventManager;
struct CSoundEvent {
    long m_active;
    long IsActive() { return m_active; }
    CString Describe(CSoundEventManager*) { ++g_described; return CString(g_described); }
};

struct CEventPool {
    CSoundEvent m_items[4];
    unsigned long m_count;
    CSoundEvent* Get(unsigned long index) { return &m_items[index]; }
};

struct CSoundEventManager {
    CEventPool m_events;
    unsigned long GetNumEvents() { return m_events.m_count; }
    CSoundEvent* GetEvent(unsigned long index) { return m_events.Get(index); }
    void UpdateEvents() { if (g_lockDepth == 1) ++g_updates; }
    void TraceEvents()
    {
        CScopedLock lock(g_section);
        UpdateEvents();
        for (unsigned long i = 0; i < GetNumEvents(); ++i) {
            CSoundEvent* event = GetEvent(i);
            if (event->IsActive())
                TraceOutput(event->Describe(this));
        }
    }
};

int main()
{
    CSoundEventManager m;
    long active[4] = { 1, 0, 1, 1 };
    for (int i = 0; i < 4; ++i) m.m_events.m_items[i].m_active = active[i];
    m.m_events.m_count = 3;
    m.TraceEvents();
    if (g_updates != 1 || g_described != 2 || g_live != 0 || g_lockDepth != 0) { std::printf("00c12720_TEST FAIL\n"); return 1; }
    std::printf("00c12720_TEST PASS\n");
    return 0;
}