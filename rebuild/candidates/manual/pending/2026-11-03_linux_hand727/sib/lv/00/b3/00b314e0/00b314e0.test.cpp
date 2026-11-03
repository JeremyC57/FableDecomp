// Behaviour test for CEngineCamera::Process (0x00B314E0): the debug script short-circuits; otherwise the desc gets the
// override viewport or the converted game window, radian FOVs (vertical only when FOV flag 2 is set), the camera position
// and a column-wise rotation from right = forward x up, up and forward, and is applied with the caller's flag.
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; void Normalise() { float l = std::sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; } };
struct Box { float TLX, TLY, BRX, BRY; };
struct Desc { Box Window; float m[12]; V Pos; float HFOV, VFOV; bool Use2DFOV; bool StateValid; };
static Desc g_applied; static bool g_appliedFlag; static int g_debugRuns, g_applies;
struct Cam { V Pos, Up, Fwd; float h, v; unsigned long flags; };
static void Convert(const Box* v, Box* a) { a->TLX = v->TLX * 2; a->TLY = v->TLY * 2; a->BRX = v->BRX * 2; a->BRY = v->BRY * 2; }
struct EngineCamera {
    Cam* GameCamera; Box* GameWindow; long OW, OH; bool DebugScriptRunning;
    void Process(bool flag)
    {
        if (DebugScriptRunning) { ++g_debugRuns; return; }
        Desc d; d.StateValid = true;
        if (OW > 0) { d.Window.TLX = 0; d.Window.TLY = 0; d.Window.BRX = (float)OW; d.Window.BRY = (float)OH; }
        else Convert(GameWindow, &d.Window);
        d.HFOV = GameCamera->h * 360.0f * (1.0f / 360.0f) * 6.2831855f;
        if (GameCamera->flags & 2) { d.VFOV = GameCamera->v * 360.0f * (1.0f / 360.0f) * 6.2831855f; d.Use2DFOV = true; }
        else { d.VFOV = 0; d.Use2DFOV = false; }
        d.Pos = GameCamera->Pos;
        V up = GameCamera->Up, fwd = GameCamera->Fwd;
        up.Normalise(); fwd.Normalise();
        V r; r.X = fwd.Y * up.Z - fwd.Z * up.Y; r.Y = fwd.Z * up.X - fwd.X * up.Z; r.Z = fwd.X * up.Y - fwd.Y * up.X;
        r.Normalise();
        d.m[0] = r.X; d.m[1] = up.X; d.m[2] = fwd.X; d.m[3] = r.Y; d.m[4] = up.Y; d.m[5] = fwd.Y;
        d.m[6] = r.Z; d.m[7] = up.Z; d.m[8] = fwd.Z; d.m[9] = d.m[10] = d.m[11] = 0;
        g_applied = d; g_appliedFlag = flag; ++g_applies;
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Cam c = { { 1, 2, 3 }, { 0, 0, 2 }, { 0, 3, 0 }, 0.25f, 0.125f, 2 };
    Box w = { 1, 2, 3, 4 };
    EngineCamera e = { &c, &w, 0, 0, false };
    e.Process(true);
    bool ok = g_applies == 1 && g_appliedFlag && g_applied.Window.BRX == 6.0f && Near(g_applied.HFOV, 1.5707964f);
    ok = ok && Near(g_applied.VFOV, 0.7853982f) && g_applied.Use2DFOV && g_applied.Pos.Z == 3.0f;
    // forward (0,1,0) x up (0,0,1) = (1,0,0)
    ok = ok && Near(g_applied.m[0], 1) && Near(g_applied.m[7], 1) && Near(g_applied.m[5], 1) && g_applied.m[9] == 0;
    e.OW = 640; e.OH = 480; c.flags = 0;
    e.Process(false);
    ok = ok && g_applied.Window.BRX == 640.0f && g_applied.Window.TLX == 0 && !g_applied.Use2DFOV && g_applied.VFOV == 0 && !g_appliedFlag;
    e.DebugScriptRunning = true;
    e.Process(true);
    ok = ok && g_debugRuns == 1 && g_applies == 2;
    if (!ok) { std::printf("00b314e0_TEST FAIL\n"); return 1; }
    std::printf("00b314e0_TEST PASS\n");
    return 0;
}