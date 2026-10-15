// Behaviour test for CThingDeadCreature::DrawPreparePrimitivesForRendering (0x00835AC0)
#include <cstdio>
struct CEnginePrimitivePrepareInfo { int frame; };
static int g_prepared = 0; static const CEnginePrimitivePrepareInfo* g_info = 0;
struct CTCGraphicAppearance {
    void PrepareForDraw(const CEnginePrimitivePrepareInfo& info) { ++g_prepared; g_info = &info; }
};
struct CThingPhysical {
    CTCGraphicAppearance* m_pGraphicAppearance;
    bool m_baseResult;
    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { return m_baseResult; }
};
struct CThingDeadCreature : CThingPhysical {
    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
    {
        if (CThingPhysical::DrawPreparePrimitivesForRendering(info)) {
            m_pGraphicAppearance->PrepareForDraw(info);
            return true;
        }
        return false;
    }
};
#define CHECK(c) if (!(c)) { std::printf("00835ac0_TEST FAIL\n"); return 1; }
int main()
{
    CTCGraphicAppearance g; CEnginePrimitivePrepareInfo info = { 3 };
    CThingDeadCreature t; t.m_pGraphicAppearance = &g; t.m_baseResult = false;
    CHECK(!t.DrawPreparePrimitivesForRendering(info) && g_prepared == 0);
    t.m_baseResult = true;
    CHECK(t.DrawPreparePrimitivesForRendering(info) && g_prepared == 1 && g_info == &info);
    std::printf("00835ac0_TEST PASS\n");
    return 0;
}