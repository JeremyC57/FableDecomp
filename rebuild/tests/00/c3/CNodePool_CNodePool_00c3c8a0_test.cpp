// Behaviour test for 00c3c8a0: CNodePool::CNodePool -- an idle pool (empty chains, free count -1, active count 0)
#include <cstdio>
class CPKObject { public: virtual ~CPKObject() { } };
class CNodePool : public CPKObject {
public:
    void* m_pActiveHead; void* m_pFreeHead; long m_freeCount; long m_activeCount;
    CNodePool() : m_pActiveHead(0), m_pFreeHead(0), m_freeCount(-1), m_activeCount(0) { }
};
int main()
{
    CNodePool p;
    int ok = p.m_pActiveHead == 0 && p.m_pFreeHead == 0 && p.m_freeCount == -1 && p.m_activeCount == 0 && sizeof(CNodePool) == 0x14;
    std::printf(ok ? "00c3c8a0_TEST PASS\n" : "00c3c8a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
