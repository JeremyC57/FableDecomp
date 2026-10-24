// Behaviour test for 0x009F5600 CInputManager::AddEventToStore: the event is copy-constructed at EventStore's end while there
// is room, otherwise handed to _M_insert_overflow(finish, x, tag, 1, true); NoStoredEvents counts every call; the result is true.
#include <cstdio>
#include <new>

static int gCopies = 0, gOverflows = 0;
static const void* gOverflowX = 0;
static unsigned int gOverflowLen = 0;
static bool gOverflowAtEnd = false;

class CInputEvent {
public:
    long m_words[13];
    CInputEvent() { for (int i = 0; i < 13; ++i) m_words[i] = 0; }
    CInputEvent(const CInputEvent& other) { ++gCopies; for (int i = 0; i < 13; ++i) m_words[i] = other.m_words[i]; }
};

struct __false_type { __false_type() {} };

template <class T> class CRetailVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLen, bool atEnd)
    {
        ++gOverflows; gOverflowX = &x; gOverflowLen = fillLen; gOverflowAtEnd = atEnd && position == _M_finish;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

class CInputManager {
public:
    CRetailVector<CInputEvent> ProcessedEvents;
    CRetailVector<CInputEvent> EventStore;
    long NoStoredEvents;
    bool AddEventToStore(const CInputEvent& event)
    {
        EventStore.push_back(event);
        ++NoStoredEvents;
        return true;
    }
};

int main()
{
    CInputEvent storage[2];
    CInputManager m;
    m.EventStore._M_start = storage; m.EventStore._M_finish = storage; m.EventStore._M_end_of_storage = storage + 2;
    m.NoStoredEvents = 0;
    CInputEvent e; e.m_words[0] = 7; e.m_words[12] = 9;
    gCopies = 0;
    bool ok = m.AddEventToStore(e) && m.AddEventToStore(e);
    ok = ok && gCopies == 2 && m.EventStore._M_finish == storage + 2 && storage[1].m_words[0] == 7 && storage[1].m_words[12] == 9;
    ok = ok && m.AddEventToStore(e) && gOverflows == 1 && gOverflowX == &e && gOverflowLen == 1 && gOverflowAtEnd
            && gCopies == 2 && m.NoStoredEvents == 3;
    if (!ok) { std::printf("009f5600_TEST FAIL\n"); return 1; }
    std::printf("009f5600_TEST PASS\n");
    return 0;
}