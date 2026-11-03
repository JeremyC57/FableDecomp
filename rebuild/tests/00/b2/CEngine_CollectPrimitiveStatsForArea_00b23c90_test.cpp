// Behaviour test for 0x00B23C90 CEngine::CollectPrimitiveStatsForArea: the forwarder must call CEnginePrimitiveRenderer::CollectPrimitiveStatsForArea on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class C2DBoxF { public: int v; };
enum EEnginePrimitiveStatIndex { STAT_INDEX_THREE = 3 };
class CPrimitiveStatMap { public: int n; };   // stands in for the STLport map (only its address travels)
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEnginePrimitiveRenderer { public:
    void CollectPrimitiveStatsForArea(CPrimitiveStatMap& stats, const C2DBoxF& area, EEnginePrimitiveStatIndex statIndex, unsigned long flags) const
    {
        ++gCalls; gThis = this;
        gPtr[0] = &stats;
        gPtr[1] = &area;
        gVal[2] = (double)statIndex;
        gVal[3] = (double)flags;
    }
};
static CEnginePrimitiveRenderer gComponent;
CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer = &gComponent;
class CEngine { public:
    virtual void CollectPrimitiveStatsForArea(CPrimitiveStatMap& stats, const C2DBoxF& area, EEnginePrimitiveStatIndex statIndex, unsigned long flags) const;
};
void CEngine::CollectPrimitiveStatsForArea(CPrimitiveStatMap& stats, const C2DBoxF& area, EEnginePrimitiveStatIndex statIndex, unsigned long flags) const
{
    g_pEnginePrimitiveRenderer->CollectPrimitiveStatsForArea(stats, area, statIndex, flags);
}
int main()
{
    CPrimitiveStatMap o0;
    C2DBoxF o1;
    CEngine engine;
    engine.CollectPrimitiveStatsForArea(o0, o1, STAT_INDEX_THREE, 43u);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0 || gPtr[1] != &o1 || gVal[2] != 3.0 || gVal[3] != 43.0) {
        std::printf("00b23c90_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23c90_TEST PASS\n");
    return 0;
}