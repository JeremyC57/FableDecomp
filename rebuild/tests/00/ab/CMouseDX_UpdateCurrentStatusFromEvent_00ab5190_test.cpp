// Behaviour test for CMouseDX::UpdateCurrentStatusFromEvent @ 0x00AB5190: button flags, clamped relative motion, absolute set.
#include <cstdio>

struct C2DVector { float X, Y; };
struct C2DExtentsI { long Width, Height; };
static C2DExtentsI GetBackBufferDimensions() { C2DExtentsI e; e.Width = 640; e.Height = 480; return e; }

enum EMouseEventType { LMB_DOWN = 1, RMB_DOWN, MMB_DOWN, LMB_UP, RMB_UP, MMB_UP, MOVE_X, MOVE_Y, WHEEL_UP, WHEEL_DOWN, SET_POSITION };
struct CMouseEvent { double Time; EMouseEventType Type; float Movement; float PosX; float PosY; };

struct CMouseDX {
    C2DVector Pos; bool LMBOn, MMBOn, RMBOn;
    void UpdateCurrentStatusFromEvent(const CMouseEvent* event);
};

void CMouseDX::UpdateCurrentStatusFromEvent(const CMouseEvent* event)
{
    C2DExtentsI extents = GetBackBufferDimensions();
    switch (event->Type) {
    case MOVE_X: {
        float x = event->Movement + Pos.X;
        Pos.X = x;
        if (x < 0.0f) Pos.X = 0.0f;
        else if (x > extents.Width - 1) Pos.X = (float)(extents.Width - 1);
        break;
    }
    case MOVE_Y: {
        float y = event->Movement + Pos.Y;
        Pos.Y = y;
        if (y < 0.0f) Pos.Y = 0.0f;
        else if (y > extents.Height - 1) Pos.Y = (float)(extents.Height - 1);
        break;
    }
    case SET_POSITION: Pos.X = event->PosX; Pos.Y = event->PosY; break;
    case LMB_DOWN: LMBOn = true; break;
    case LMB_UP: LMBOn = false; break;
    case RMB_DOWN: RMBOn = true; break;
    case RMB_UP: RMBOn = false; break;
    case MMB_DOWN: MMBOn = true; break;
    case MMB_UP: MMBOn = false; break;
    default: break;
    }
}

static CMouseEvent ev(EMouseEventType t, float m, float x = 0, float y = 0)
{
    CMouseEvent e; e.Time = 0; e.Type = t; e.Movement = m; e.PosX = x; e.PosY = y; return e;
}

int main()
{
    CMouseDX m; m.Pos.X = 100; m.Pos.Y = 100; m.LMBOn = m.MMBOn = m.RMBOn = false;
    CMouseEvent e = ev(LMB_DOWN, 0); m.UpdateCurrentStatusFromEvent(&e);
    e = ev(RMB_DOWN, 0); m.UpdateCurrentStatusFromEvent(&e);
    int ok = m.LMBOn && m.RMBOn && !m.MMBOn;
    e = ev(LMB_UP, 0); m.UpdateCurrentStatusFromEvent(&e);
    e = ev(MMB_DOWN, 0); m.UpdateCurrentStatusFromEvent(&e);
    ok = ok && !m.LMBOn && m.MMBOn;
    e = ev(MOVE_X, 25); m.UpdateCurrentStatusFromEvent(&e); ok = ok && m.Pos.X == 125;
    e = ev(MOVE_X, -500); m.UpdateCurrentStatusFromEvent(&e); ok = ok && m.Pos.X == 0;
    e = ev(MOVE_X, 1000); m.UpdateCurrentStatusFromEvent(&e); ok = ok && m.Pos.X == 639;
    e = ev(MOVE_Y, 1000); m.UpdateCurrentStatusFromEvent(&e); ok = ok && m.Pos.Y == 479;
    e = ev(MOVE_Y, -10); m.UpdateCurrentStatusFromEvent(&e); ok = ok && m.Pos.Y == 469;
    e = ev(WHEEL_UP, 3); m.UpdateCurrentStatusFromEvent(&e); ok = ok && m.Pos.Y == 469 && m.Pos.X == 639;
    e = ev(SET_POSITION, 0, 12, 34); m.UpdateCurrentStatusFromEvent(&e); ok = ok && m.Pos.X == 12 && m.Pos.Y == 34;
    if (!ok) { std::printf("00ab5190_TEST FAIL\n"); return 1; }
    std::printf("00ab5190_TEST PASS\n");
    return 0;
}