// Behaviour test for 0x0083B3B0 CThingVillage::DrawIsDrawable
#include <cstdio>
struct CDisplayEngine {
    bool m_enabled;
    virtual bool IsDrawingEnabled() { return m_enabled; }
};
static CDisplayEngine g_engine;
static CDisplayEngine* GetDisplayEngine() { return &g_engine; }
struct CThing {
    bool m_drawable;
    bool DrawIsDrawable() { return m_drawable; }
};
struct CThingVillage : CThing {
    bool DrawIsDrawable() { return GetDisplayEngine()->IsDrawingEnabled() && CThing::DrawIsDrawable(); }
};
int main()
{
    CThingVillage v; v.m_drawable = true; g_engine.m_enabled = true;
    bool ok = v.DrawIsDrawable();
    g_engine.m_enabled = false; if (v.DrawIsDrawable()) ok = false;
    g_engine.m_enabled = true; v.m_drawable = false; if (v.DrawIsDrawable()) ok = false;
    if (!ok) { std::printf("0083b3b0_TEST FAIL\n"); return 1; }
    std::printf("0083b3b0_TEST PASS\n");
    return 0;
}