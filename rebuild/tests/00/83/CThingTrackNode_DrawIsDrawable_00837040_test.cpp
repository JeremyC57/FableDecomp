#include <cstdio>
// Behaviour test for CThingTrackNode::DrawIsDrawable (0x00837040): returns the display engine's slot-9 answer.
struct CDisplayEngine {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
    virtual void v08() {}
    virtual bool IsDrawingEnabled() { return m_enabled; }
    bool m_enabled;
};
static CDisplayEngine g_engine;
struct CThingTrackNode {
    CDisplayEngine* GetDisplayEngine() { return &g_engine; }
    bool DrawIsDrawable() { return GetDisplayEngine()->IsDrawingEnabled(); }
};
int main()
{
    CThingTrackNode n;
    g_engine.m_enabled = true;
    if (!n.DrawIsDrawable()) { std::printf("00837040_TEST FAIL\n"); return 1; }
    g_engine.m_enabled = false;
    if (n.DrawIsDrawable()) { std::printf("00837040_TEST FAIL\n"); return 1; }
    std::printf("00837040_TEST PASS\n");
    return 0;
}