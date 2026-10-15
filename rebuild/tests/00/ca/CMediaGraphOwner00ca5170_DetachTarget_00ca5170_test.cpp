#include <cstdio>

struct CMediaGraph {
    long result; int calls;
    CMediaGraph() : result(0), calls(0) { }
    virtual long Detach() { ++calls; return result; }
    virtual ~CMediaGraph() { }
};

struct COwner {
    CMediaGraph* m_pGraph; int releases;
    long ReleaseInterface() { ++releases; return 0; }
    long DetachTarget()
    {
        long result = m_pGraph->Detach();
        if (result < 0)
            return result;
        return ReleaseInterface();
    }
};

int main()
{
    CMediaGraph g; COwner o; o.m_pGraph = &g; o.releases = 0;
    if (o.DetachTarget() != 0 || o.releases != 1 || g.calls != 1) { std::printf("00ca5170_TEST FAIL\n"); return 1; }
    g.result = -5;
    if (o.DetachTarget() != -5 || o.releases != 1 || g.calls != 2) { std::printf("00ca5170_TEST FAIL\n"); return 1; }
    g.result = 3;
    if (o.DetachTarget() != 0 || o.releases != 2) { std::printf("00ca5170_TEST FAIL\n"); return 1; }
    std::printf("00ca5170_TEST PASS\n");
    return 0;
}