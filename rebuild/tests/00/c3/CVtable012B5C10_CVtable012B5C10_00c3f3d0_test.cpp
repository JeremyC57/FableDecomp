// Behaviour test for the CVtable_012B5C10 destructor (0x00C3F3D0): Stop() runs only when the
// stopped flag is clear, and always before the list-node base destructor.
#include <cstdio>

static int g_events[8];
static int g_count = 0;

struct CListNode {
    virtual ~CListNode() { g_events[g_count++] = 2; }
    CListNode* m_prev;
    CListNode* m_next;
};

struct CVtable_012B5C10 : CListNode {
    unsigned char _body[0x20];
    unsigned long m_state   : 31;
    unsigned long m_stopped : 1;
    virtual void v01() {}
    virtual void Stop() { m_state = 0; m_stopped = 1; g_events[g_count++] = 1; }
    bool IsRunning() const { return !m_stopped; }
    virtual ~CVtable_012B5C10();
};

CVtable_012B5C10::~CVtable_012B5C10()
{
    if (IsRunning())
        Stop();
}

int main()
{
    CVtable_012B5C10* a = new CVtable_012B5C10;
    a->m_state = 5; a->m_stopped = 0;
    delete a;
    bool ok = g_count == 2 && g_events[0] == 1 && g_events[1] == 2;
    CVtable_012B5C10* b = new CVtable_012B5C10;
    b->m_state = 0; b->m_stopped = 1;
    delete b;
    ok = ok && g_count == 3 && g_events[2] == 2;
    if (!ok) {
        std::printf("00c3f3d0_TEST FAIL\n");
        return 1;
    }
    std::printf("00c3f3d0_TEST PASS\n");
    return 0;
}