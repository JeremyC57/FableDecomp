// behaviour test for 0x00C0EA10 CScriptHost::ClearEvents: unlink and delete every queued event
#include <cstdio>

static int g_live = 0;

struct CQueuedEvent {
    long m_type;
    long m_time;
    long m_data;
    CQueuedEvent() { ++g_live; }                // counting only in the test
    ~CQueuedEvent() { --g_live; }
};

struct CEventList {
    CQueuedEvent* m_items[8];
    int m_count;
    CEventList() : m_count(0) { }
    virtual ~CEventList() { }
    virtual void Remove(CQueuedEvent* item)
    {
        for (int i = 0; i < m_count; ++i)
            if (m_items[i] == item) { for (int j = i + 1; j < m_count; ++j) m_items[j - 1] = m_items[j]; --m_count; return; }
    }
    virtual void Add(CQueuedEvent* item) { m_items[m_count++] = item; }
    virtual CQueuedEvent* GetFirst() { return m_count ? m_items[0] : 0; }
};

struct CScriptHost {
    CEventList m_events;
    void ClearEvents();
};

void CScriptHost::ClearEvents()
{
    CQueuedEvent* event = m_events.GetFirst();
    while (event) {
        m_events.Remove(event);
        delete event;
        event = m_events.GetFirst();
    }
}

int main()
{
    CScriptHost host;
    host.ClearEvents();                                           // empty: nothing to do
    for (int i = 0; i < 5; ++i) host.m_events.Add(new CQueuedEvent);
    if (g_live != 5) { std::printf("00c0ea10_TEST FAIL\n"); return 1; }
    host.ClearEvents();
    if (g_live != 0 || host.m_events.m_count != 0) { std::printf("00c0ea10_TEST FAIL\n"); return 1; }
    std::printf("00c0ea10_TEST PASS\n");
    return 0;
}