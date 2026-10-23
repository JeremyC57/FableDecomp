// Behaviour test for CInputProcessInventoryMapScreen::ProcessInput (0x0068AE30): input routing model.
#include <cstdio>
#include <cmath>
struct Vec2 { float X, Y; Vec2() {} Vec2(float x, float y) : X(x), Y(y) {} };
struct FrontEnd { long lastEvent; Vec2 zoom; Vec2 mouse; long scroll; int sends; };
static FrontEnd g_fe;
static void Send(long e) { g_fe.lastEvent = e; g_fe.sends++; }
struct MapTC { Vec2 pan; int pans; void PanMap(Vec2 d) { pan = d; pans++; } };
struct GameEvent { long Type; signed char Player; };
static GameEvent g_lastGameEvent; static int g_gameEvents;
enum { PRESSED = 1, HELD = 2, RELEASED = 3, OTHER = 4 };
struct Event { int kind; long action; Vec2 Position; float wheel; };
struct Proc {
    MapTC* heroMap; bool hasMap; signed char playerNumber;
    bool IsEventButtonPressed(const Event& e) { return e.kind == PRESSED; }
    bool IsEventButtonHeld(const Event& e) { return e.kind == HELD; }
    bool IsEventButtonReleased(const Event& e) { return e.kind == RELEASED; }
    bool IsEventGameAction(long a, const Event& e) { return e.action == a; }
    bool ProcessInput(const Event& event)
    {
        MapTC* map = 0;
        if (hasMap) map = heroMap;
        if (IsEventButtonPressed(event)) {
            if (IsEventGameAction(0x19, event)) Send(0x1a);
            else if (IsEventGameAction(0x4c, event)) Send(5);
            else if (IsEventGameAction(0x58, event)) Send(6);
            else if (IsEventGameAction(0x59, event)) Send(7);
            else if (IsEventGameAction(0x25, event)) Send(0);
            else if (IsEventGameAction(0x26, event)) Send(1);
            else if (IsEventGameAction(0x27, event)) Send(5);
            else if (IsEventGameAction(0x28, event)) Send(4);
            else return false;
        } else if (IsEventButtonHeld(event)) {
            if (IsEventGameAction(0x19, event)) Send(0x1c);
            else if (IsEventGameAction(0x4c, event)) Send(0x1d);
            else if (IsEventGameAction(0x48, event)) {
                GameEvent ge; ge.Type = 0x1b; ge.Player = -1; ge.Player = playerNumber;
                g_lastGameEvent = ge; g_gameEvents++;
            } else return false;
        } else if (IsEventButtonReleased(event)) {
            if (IsEventGameAction(0x19, event)) Send(0x1f);
            else if (IsEventGameAction(0x4c, event)) Send(0x20);
            else if (IsEventGameAction(0x58, event)) Send(6);
            else if (IsEventGameAction(0x59, event)) Send(7);
            else if (IsEventGameAction(0x67, event)) { g_fe.zoom = Vec2(1.0f, 1.0f); Send(0xc); }
            else if (IsEventGameAction(0x66, event)) { g_fe.zoom = Vec2(-1.0f, -1.0f); Send(0xd); }
            else if (IsEventGameAction(0x6b, event)) { if (map) map->PanMap(Vec2(0.0f, -1.0f)); }
            else if (IsEventGameAction(0x6a, event)) { if (map) map->PanMap(Vec2(0.0f, 1.0f)); }
            else if (IsEventGameAction(0x68, event)) { if (map) map->PanMap(Vec2(-1.0f, 0.0f)); }
            else if (IsEventGameAction(0x69, event)) { if (map) map->PanMap(Vec2(1.0f, 0.0f)); }
            else return false;
        } else if (IsEventGameAction(0x47, event)) { g_fe.mouse = event.Position; Send(0x19); }
        else if (IsEventGameAction(0x65, event)) g_fe.scroll = (long)std::ceil(event.wheel * 0.008333334f);
        else return false;
        return true;
    }
};
static Event Ev(int k, long a) { Event e; e.kind = k; e.action = a; e.Position = Vec2(3.0f, 4.0f); e.wheel = 240.0f; return e; }
int main()
{
    MapTC m; m.pans = 0;
    Proc p; p.heroMap = &m; p.hasMap = true; p.playerNumber = 2;
    g_fe.sends = 0; g_gameEvents = 0;
    bool ok = true;
    ok = ok && p.ProcessInput(Ev(PRESSED, 0x28)) && g_fe.lastEvent == 4;
    ok = ok && !p.ProcessInput(Ev(PRESSED, 0x99));
    ok = ok && p.ProcessInput(Ev(HELD, 0x48)) && g_gameEvents == 1 && g_lastGameEvent.Type == 0x1b && g_lastGameEvent.Player == 2;
    ok = ok && p.ProcessInput(Ev(RELEASED, 0x66)) && g_fe.lastEvent == 0xd && g_fe.zoom.X == -1.0f;
    ok = ok && p.ProcessInput(Ev(RELEASED, 0x68)) && m.pans == 1 && m.pan.X == -1.0f && m.pan.Y == 0.0f;
    p.hasMap = false;
    ok = ok && p.ProcessInput(Ev(RELEASED, 0x69)) && m.pans == 1;
    ok = ok && p.ProcessInput(Ev(OTHER, 0x47)) && g_fe.mouse.Y == 4.0f && g_fe.lastEvent == 0x19;
    ok = ok && p.ProcessInput(Ev(OTHER, 0x65)) && g_fe.scroll == 2;
    ok = ok && !p.ProcessInput(Ev(OTHER, 0x19));
    if (!ok) { std::printf("0068ae30_TEST FAIL\n"); return 1; }
    std::printf("0068ae30_TEST PASS\n");
    return 0;
}