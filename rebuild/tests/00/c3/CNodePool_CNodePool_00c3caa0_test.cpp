// Behaviour test for 00c3caa0: CNodePool::~CNodePool -- Clear runs only when nodes are active or recycling is on
#include <cstdio>
static int g_clears = 0;
class CPKObject { public: virtual ~CPKObject() { } };
class CNodePool : public CPKObject {
public:
    void* m_pActiveHead; void* m_pFreeHead; long m_freeCount; long m_activeCount;
    bool IsRecycling() const { return m_freeCount >= 0; }
    void Clear() { ++g_clears; m_activeCount = 0; m_freeCount = -1; }
    ~CNodePool() { if (m_activeCount != 0 || IsRecycling()) Clear(); }
};
int main()
{
    { CNodePool a; a.m_freeCount = -1; a.m_activeCount = 0; }
    bool idle = g_clears == 0;
    { CNodePool b; b.m_freeCount = -1; b.m_activeCount = 3; }
    bool active = g_clears == 1;
    { CNodePool c; c.m_freeCount = 0; c.m_activeCount = 0; }
    if (!idle || !active || g_clears != 2) { std::printf("00c3caa0_TEST FAIL\n"); return 1; }
    std::printf("00c3caa0_TEST PASS\n");
    return 0;
}
