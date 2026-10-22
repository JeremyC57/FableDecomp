#include <cstdio>
// Behaviour model of CInputProcessFireheartMinigame::ProcessInput (0x00691870).
struct Minigame { long lastInput; int quits; int answers; bool lastAnswer; bool Quitting;
    void ProcessInput(long i) { lastInput = i; } void DelayedQuit() { ++quits; } void Answer(bool y) { ++answers; lastAnswer = y; } };
struct Event { long action; bool pressed; float x, y; };
static int g_handled = 0, g_events = 0;
static void Process(const Event& e, Minigame* m, bool hasC6)
{
    if (e.action == 0x3c) {
        float X = e.x, Y = e.y;
        if (Y > 0.5) { m->ProcessInput(0); ++g_handled; }
        else if (Y < -0.5) { m->ProcessInput(2); ++g_handled; }
        else if (X > 0.5) { m->ProcessInput(1); ++g_handled; }
        else if (X < -0.5) { m->ProcessInput(3); ++g_handled; }
    }
    if (!e.pressed) return;
    if (e.action == 0x2c && !m->Quitting) { m->DelayedQuit(); ++g_handled; return; }
    if (e.action == 0x16) { m->Answer(true); ++g_handled; return; }
    if (e.action == 0x17) { m->Answer(false); ++g_handled; return; }
    if (e.action == 3 && hasC6) ++g_events;
}
int main()
{
    Minigame m; m.lastInput = -1; m.quits = 0; m.answers = 0; m.lastAnswer = false; m.Quitting = false;
    Event up = { 0x3c, false, 0.0f, 0.9f }; Process(up, &m, false);
    int ok = m.lastInput == 0 && g_handled == 1;
    Event left = { 0x3c, false, -0.9f, 0.1f }; Process(left, &m, false); ok = ok && m.lastInput == 3;
    Event dead = { 0x3c, false, 0.2f, 0.1f }; Process(dead, &m, false); ok = ok && g_handled == 2;
    Event quit = { 0x2c, true, 0, 0 }; Process(quit, &m, false); ok = ok && m.quits == 1;
    m.Quitting = true; Process(quit, &m, false); ok = ok && m.quits == 1 && g_handled == 3;
    Event no = { 0x17, true, 0, 0 }; Process(no, &m, false); ok = ok && m.answers == 1 && !m.lastAnswer;
    Event ev = { 3, true, 0, 0 }; Process(ev, &m, true); Process(ev, &m, false); ok = ok && g_events == 1;
    if (!ok) { std::printf("00691870_TEST FAIL\n"); return 1; }
    std::printf("00691870_TEST PASS\n");
    return 0;
}
