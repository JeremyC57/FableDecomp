// Behaviour test for 00c3e2a0: CAudioPlatform::~CAudioPlatform -- Shutdown first, then the four pools in reverse order
#include <cstdio>
#include <cstring>
static char g_seq[16] = "";
class CPKObject { public: virtual ~CPKObject() { } };
class CNodePool : public CPKObject {
public:
    char m_tag; void* m_pActiveHead; void* m_pFreeHead; long m_freeCount;
    ~CNodePool() { std::size_t n = std::strlen(g_seq); g_seq[n] = m_tag; g_seq[n + 1] = 0; }
};
class CAudioPlatform : public CPKObject {
public:
    CNodePool m_pool04, m_pool18, m_poolA, m_poolB;
    CAudioPlatform() { m_pool04.m_tag = '1'; m_pool18.m_tag = '2'; m_poolA.m_tag = 'A'; m_poolB.m_tag = 'B'; }
    virtual void Shutdown() { std::strcat(g_seq, "S"); }
    ~CAudioPlatform() { Shutdown(); }
};
int main()
{
    { CAudioPlatform p; }
    if (std::strcmp(g_seq, "SBA21")) { std::printf("00c3e2a0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; CPKObject* q = new CAudioPlatform; delete q;
    if (std::strcmp(g_seq, "SBA21")) { std::printf("00c3e2a0_TEST FAIL\n"); return 1; }
    std::printf("00c3e2a0_TEST PASS\n");
    return 0;
}
