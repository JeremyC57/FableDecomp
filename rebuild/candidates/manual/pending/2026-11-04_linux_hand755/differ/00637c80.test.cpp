// Behaviour test for CThingMapManager::DebugDraw (0x00637C80), g++ model of the parked source.
#include <cstdio>
#include <cmath>
struct C2DCoordI { long X, Y; };
struct C2DBoxI { long TLX, TLY, BRX, BRY; C2DCoordI GetTLPos() const { C2DCoordI c = { TLX, TLY }; return c; } };
struct C2DVector { float X, Y; };
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CThingMap { C2DBoxI Area; int drawCalls; long lastLayer; C2DVector lastOrigin; };
static double g_OriginX = 100.0, g_OriginY = 200.0;
static const double g_Scale = 0.05;
static long g_Frame;
static int g_boxCalls; static C2DBoxF g_lastBox; static unsigned g_lastColour;
static float AXToVX(float x) { return x * 2.0f; }
static float AYToVY(float y) { return y * 3.0f; }
static CThingMap* g_map;
static void DebugDraw(const C2DCoordI& pos, long layer)
{
    CThingMap* map = g_map;
    C2DVector origin;
    origin.X = (float)(g_OriginX * g_Scale);
    origin.Y = (float)(g_OriginY * g_Scale);
    if (map) {
        ++map->drawCalls; map->lastLayer = layer; map->lastOrigin = origin;
        if (g_Frame % 16 > 8) {
            float hw = AXToVX(3.0f), hh = AYToVY(3.0f);
            C2DCoordI tl = map->Area.GetTLPos();
            C2DCoordI local = { pos.X - tl.X, pos.Y - tl.Y };
            float y = AYToVY((float)local.Y) + origin.Y;
            float x = AXToVX((float)local.X) + origin.X;
            C2DBoxF b = { x - hw, y - hh, x + hw, y + hh };
            g_lastBox = b; g_lastColour = 0xffffffffu; ++g_boxCalls;
        }
        ++g_Frame;
    }
}
int main()
{
    CThingMap m = { { 10, 20, 50, 60 }, 0, 0, { 0, 0 } };
    C2DCoordI p = { 15, 27 };
    int ok = 1;
    g_map = 0; DebugDraw(p, 1); ok &= g_Frame == 0 && g_boxCalls == 0;
    g_map = &m; g_Frame = 3; DebugDraw(p, 2);
    ok &= m.drawCalls == 1 && m.lastLayer == 2 && g_boxCalls == 0 && g_Frame == 4;
    ok &= std::fabs(m.lastOrigin.X - 5.0f) < 1e-5f && std::fabs(m.lastOrigin.Y - 10.0f) < 1e-5f;
    g_Frame = 9; DebugDraw(p, 3);
    ok &= g_boxCalls == 1 && g_Frame == 10 && g_lastColour == 0xffffffffu;
    ok &= std::fabs(g_lastBox.TLX - (10.0f + 5.0f - 6.0f)) < 1e-4f && std::fabs(g_lastBox.BRX - (10.0f + 5.0f + 6.0f)) < 1e-4f;
    ok &= std::fabs(g_lastBox.TLY - (21.0f + 10.0f - 9.0f)) < 1e-4f && std::fabs(g_lastBox.BRY - (21.0f + 10.0f + 9.0f)) < 1e-4f;
    g_Frame = 24; DebugDraw(p, 3); ok &= g_boxCalls == 1;
    std::printf(ok ? "00637c80_TEST PASS\n" : "00637c80_TEST FAIL\n");
    return ok ? 0 : 1;
}
