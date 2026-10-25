// Behaviour test for 00c4d000: CAudioPlatform::CAudioPlatform -- four pools built in order, each initialised idle
#include <cstdio>
static int g_order[4]; static int g_n = 0;
class CPKObject { public: virtual ~CPKObject() { } };
class CNodePool : public CPKObject {
public:
    void* m_pActiveHead; void* m_pFreeHead; long m_freeCount; long m_activeCount;
    CNodePool() : m_pActiveHead(0), m_pFreeHead(0), m_freeCount(-1), m_activeCount(0) { g_order[g_n++] = (int)(long)this; }
};
class CAudioPlatform : public CPKObject {
public:
    CNodePool m_pool04, m_pool18, m_poolA, m_poolB;
    CAudioPlatform() { }
    virtual void Shutdown() { }
};
int main()
{
    CAudioPlatform p;
    int ok = g_n == 4 && g_order[0] == (int)(long)&p.m_pool04 && g_order[1] == (int)(long)&p.m_pool18
          && g_order[2] == (int)(long)&p.m_poolA && g_order[3] == (int)(long)&p.m_poolB
          && p.m_poolB.m_freeCount == -1 && p.m_pool04.m_activeCount == 0;
    std::printf(ok ? "00c4d000_TEST PASS\n" : "00c4d000_TEST FAIL\n");
    return ok ? 0 : 1;
}
