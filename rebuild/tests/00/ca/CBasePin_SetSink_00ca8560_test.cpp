#include <cstdio>
// CBasePin::SetSink: under the object lock, store the quality sink (null allowed); NOERROR.
struct Lock { int depth; int entered; void Enter() { ++depth; ++entered; } void Leave() { --depth; } };
struct AutoLock { Lock* l; AutoLock(Lock* p) : l(p) { l->Enter(); } ~AutoLock() { l->Leave(); } };
struct Pin {
    Lock* m_pLock; void* m_pQSink; int depthAtStore;
    int SetSink(void* piqc)
    {
        AutoLock cObjectLock(m_pLock);
        m_pQSink = piqc;
        depthAtStore = m_pLock->depth;
        return 0;
    }
};
int main()
{
    Lock l; l.depth = 0; l.entered = 0;
    int sink;
    Pin p; p.m_pLock = &l; p.m_pQSink = 0; p.depthAtStore = 0;
    if (p.SetSink(&sink) != 0 || p.m_pQSink != &sink || p.depthAtStore != 1 || l.depth != 0) { std::printf("00ca8560_TEST FAIL\n"); return 1; }
    if (p.SetSink(0) != 0 || p.m_pQSink != 0 || l.entered != 2) { std::printf("00ca8560_TEST FAIL\n"); return 1; }
    std::printf("00ca8560_TEST PASS\n");
    return 0;
}