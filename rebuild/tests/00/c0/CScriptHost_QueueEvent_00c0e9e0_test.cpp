// behaviour test for 0x00C0E9E0 CScriptHost::QueueEvent: allocate a {type, time, data} record and add it to the event list at +0x60
#include <cstdio>

struct CQueuedEvent {
    long m_type;
    long m_time;
    long m_data;
};

struct CEventList {                             // test list: array behind the typed-list virtuals
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
    void QueueEvent(long type, long time, long data);
};

void CScriptHost::QueueEvent(long type, long time, long data)
{
    CQueuedEvent* event = new CQueuedEvent;
    event->m_time = time;
    event->m_data = data;
    event->m_type = type;
    m_events.Add(event);
}

int main()
{
    CScriptHost host;
    host.QueueEvent(3, 100, 7);
    host.QueueEvent(4, 200, -1);
    if (host.m_events.m_count != 2) { std::printf("00c0e9e0_TEST FAIL\n"); return 1; }
    CQueuedEvent* a = host.m_events.m_items[0];
    CQueuedEvent* b = host.m_events.m_items[1];
    if (a->m_type != 3 || a->m_time != 100 || a->m_data != 7) { std::printf("00c0e9e0_TEST FAIL\n"); return 1; }
    if (b->m_type != 4 || b->m_time != 200 || b->m_data != -1) { std::printf("00c0e9e0_TEST FAIL\n"); return 1; }
    delete a; delete b;
    std::printf("00c0e9e0_TEST PASS\n");
    return 0;
}