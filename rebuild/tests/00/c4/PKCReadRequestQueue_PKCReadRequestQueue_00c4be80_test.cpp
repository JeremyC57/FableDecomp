// Behaviour test for 00c4be80: PKCReadRequestQueue::PKCReadRequestQueue -- capacity stored, capacity*4 bytes from scalar operator new, count and head zeroed
#include <cstdio>
#include <cstdlib>

static unsigned int g_lastSize = 0;
static void* Allocate(unsigned int size) { g_lastSize = size; return std::malloc(size); }
struct SReadRequest { unsigned int m_id; };
class PKCReadRequestQueue {
public:
    PKCReadRequestQueue(long capacity)
        : m_capacity(capacity), m_pRecords((SReadRequest*)Allocate(capacity * sizeof(SReadRequest))), m_count(0), m_head(0) { }
    virtual ~PKCReadRequestQueue() { std::free(m_pRecords); }
    long          m_capacity;
    SReadRequest* m_pRecords;
    long          m_count;
    long          m_head;
};

int main()
{
    PKCReadRequestQueue q(5);
    if (q.m_capacity != 5 || q.m_pRecords == 0 || q.m_count != 0 || q.m_head != 0 || g_lastSize != 20) {
        std::printf("00c4be80_TEST FAIL\n"); return 1;
    }
    std::printf("00c4be80_TEST PASS\n");
    return 0;
}