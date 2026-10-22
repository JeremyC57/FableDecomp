#include <cstdio>
// Behaviour model of CInputProcessInGameMenu::ProcessInput (0x00690480).
struct Event { long key; float x, y; int pressed; int released; long action; };
struct Menu { long subPage; bool slider, quit, listA, listB; int closed; bool heroAlive; };
struct Out { int handled; int sends; long last; int gameEvents; };
static void Send(Out& o, long e) { ++o.sends; o.last = e; }
static void Side(Menu& m, Out& o, long sliderEvent, long plainEvent)
{
    if (m.slider) Send(o, sliderEvent);
    else if (!m.quit && !m.listA && !m.listB) Send(o, plainEvent);
    ++o.handled;
}
static void Process(Menu& m, const Event& e, Out& o)
{
    if (m.subPage != -1) return;
    if (e.key == 0x11) {
        if (e.y > 0.6f) { Send(o, 1); ++o.handled; } else if (e.y < -0.6f) { Send(o, 0); ++o.handled; }
        if (e.x > 0.6f) { Send(o, 0x15); ++o.handled; } else if (e.x < -0.6f) { Send(o, 0x14); ++o.handled; }
    } else if (e.key == 0x12) {
        if (e.y > 0.6f) { Send(o, 7); ++o.handled; } else if (e.y < -0.6f) { Send(o, 6); ++o.handled; }
    } else if (e.pressed) {
        if (e.action == 0x25) { Send(o, 0); ++o.handled; }
        else if (e.action == 0x26) { Send(o, 1); ++o.handled; }
        else if (e.action == 0x27) Side(m, o, 0x14, 5);
        else if (e.action == 0x28) Side(m, o, 0x15, 4);
        else if (e.action == 0x21) { Send(o, 4); ++o.handled; }
        else if (e.action == 0x22) { Send(o, 5); ++o.handled; }
        else if (e.action == 3) { ++m.closed; ++o.handled; }
        else if (e.action == 2) { ++m.closed; if (m.heroAlive) ++o.gameEvents; ++o.handled; }
    } else if (e.released) {
        if (e.action == 0x25) { Send(o, 0); ++o.handled; }
        else if (e.action == 0x26) { Send(o, 1); ++o.handled; }
    }
}
int main()
{
    Menu m = { -1, false, false, false, false, 0, true };
    Out o = { 0, 0, -1, 0 };
    int ok = 1;
    Event stick = { 0x11, 0.9f, 0.9f, 0, 0, 0 }; Process(m, stick, o); ok = ok && o.sends == 2 && o.last == 0x15 && o.handled == 2;
    Event down = { 0x12, 0.0f, -0.7f, 0, 0, 0 }; Process(m, down, o); ok = ok && o.sends == 3 && o.last == 6;
    Event left = { 0, 0, 0, 1, 0, 0x27 }; Process(m, left, o); ok = ok && o.last == 5;
    m.slider = true; Process(m, left, o); ok = ok && o.last == 0x14;
    m.slider = false; m.quit = true; int before = o.sends; Process(m, left, o); ok = ok && o.sends == before;
    Event close = { 0, 0, 0, 1, 0, 2 }; Process(m, close, o); ok = ok && m.closed == 1 && o.gameEvents == 1;
    Event rel = { 0, 0, 0, 0, 1, 0x26 }; Process(m, rel, o); ok = ok && o.last == 1;
    m.subPage = 2; before = o.sends; Process(m, stick, o); ok = ok && o.sends == before;
    if (!ok) { std::printf("00690480_TEST FAIL\n"); return 1; }
    std::printf("00690480_TEST PASS\n");
    return 0;
}