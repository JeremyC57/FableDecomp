#include <cstdio>
// behaviour test for CThingBuilding::DrawPreparePrimitivesForRendering (0x0082D080)
class CEnginePrimitivePrepareInfo { public: int id; };
static int g_prepared = 0;
static CEnginePrimitivePrepareInfo* g_last = 0;
static bool g_baseResult = false;
struct CTCGraphicAppearance {
    void PrepareForDraw(CEnginePrimitivePrepareInfo* info) { ++g_prepared; g_last = info; }
};
class CThingPhysical { public:
    CTCGraphicAppearance* m_pGraphicAppearance;
    bool DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info) { return g_baseResult; }
};
class CThingBuilding : public CThingPhysical { public:
    bool DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info);
};
bool CThingBuilding::DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info)
{
    if (CThingPhysical::DrawPreparePrimitivesForRendering(info)) {
        m_pGraphicAppearance->PrepareForDraw(info);
        return true;
    }
    return false;
}
int main()
{
    CTCGraphicAppearance g; CThingBuilding b; b.m_pGraphicAppearance = &g;
    CEnginePrimitivePrepareInfo info;
    g_baseResult = false;
    if (b.DrawPreparePrimitivesForRendering(&info) || g_prepared != 0) { std::printf("0082d080_TEST FAIL\n"); return 1; }
    g_baseResult = true;
    if (!b.DrawPreparePrimitivesForRendering(&info) || g_prepared != 1 || g_last != &info) { std::printf("0082d080_TEST FAIL\n"); return 1; }
    std::printf("0082d080_TEST PASS\n");
    return 0;
}