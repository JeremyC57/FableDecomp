// Behaviour test for 0x00B23CA0 CEngine::ProcessPrimitiveStats: the forwarder must call CEnginePrimitiveRenderer::ProcessPrimitiveStats on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CCharString { public: int v; };
template <class T> class CArray { public: int n; };
enum EEnginePrimitiveStatIndex { STAT_INDEX_THREE = 3 };
class CPrimitiveStatMap { public: int n; };   // stands in for the STLport map (only its address travels)
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEnginePrimitiveRenderer { public:
    void ProcessPrimitiveStats(const CPrimitiveStatMap& stats, EEnginePrimitiveStatIndex statIndex, unsigned long& total, CArray<CCharString>& report) const
    {
        ++gCalls; gThis = this;
        gPtr[0] = &stats;
        gVal[1] = (double)statIndex;
        gPtr[2] = &total;
        gPtr[3] = &report;
    }
};
static CEnginePrimitiveRenderer gComponent;
CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer = &gComponent;
class CEngine { public:
    virtual void ProcessPrimitiveStats(const CPrimitiveStatMap& stats, EEnginePrimitiveStatIndex statIndex, unsigned long& total, CArray<CCharString>& report) const;
};
void CEngine::ProcessPrimitiveStats(const CPrimitiveStatMap& stats, EEnginePrimitiveStatIndex statIndex, unsigned long& total, CArray<CCharString>& report) const
{
    g_pEnginePrimitiveRenderer->ProcessPrimitiveStats(stats, statIndex, total, report);
}
int main()
{
    CPrimitiveStatMap o0;
    unsigned long o2 = 5;
    CArray<CCharString> o3;
    CEngine engine;
    engine.ProcessPrimitiveStats(o0, STAT_INDEX_THREE, o2, o3);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0 || gVal[1] != 3.0 || gPtr[2] != &o2 || gPtr[3] != &o3) {
        std::printf("00b23ca0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23ca0_TEST PASS\n");
    return 0;
}