// behaviour test for 0x00C21E60 CSoundEventSet::FindItem: the first pending event when its key is below the argument
#include <cstdio>

struct CSoundEvent { unsigned long m_key; };

struct CSoundEventQueue {
    CSoundEvent* m_first;
    virtual CSoundEvent* GetFirst() { return m_first; }        // slot 11 in retail
    virtual ~CSoundEventQueue() {}
};

struct CSoundEventSet {
    CSoundEventQueue m_pending;
    CSoundEvent* FindItem(unsigned long key);
};

CSoundEvent* CSoundEventSet::FindItem(unsigned long key)
{
    CSoundEvent* event = m_pending.GetFirst();
    if (event && event->m_key < key)
        return event;
    return 0;
}

int main()
{
    CSoundEvent event; event.m_key = 10;
    CSoundEventSet set; set.m_pending.m_first = 0;
    if (set.FindItem(100) != 0) { std::printf("00c21e60_TEST FAIL\n"); return 1; }
    set.m_pending.m_first = &event;
    if (set.FindItem(11) != &event || set.FindItem(10) != 0 || set.FindItem(0) != 0 || set.FindItem(0xFFFFFFFFu) != &event) { std::printf("00c21e60_TEST FAIL\n"); return 1; }
    std::printf("00c21e60_TEST PASS\n");
    return 0;
}