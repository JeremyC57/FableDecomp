// Behaviour test for 0x00689DF0 CInputProcessInventory::ProcessInput: models the routing decisions -- typed characters while a text
// field or key redefiner is active (key-pressed events 0x21 / 0x22), the key-redefiner event map, the redefiner wheel (0x24 / 0x25),
// the left stick (map pan when panning a zoomed map, else NUISystem up/down/left/right with the exclusive-observer extra event)
// and the nine inventory shortcut actions (0x5a..0x5d, 0x64, 0x61..0x63, 0x60 -> shortcut slots 0..8).
#include <stdio.h>

static int g_events[8]; static int g_nevents; static int g_performed; static int g_panned; static int g_shortcut;
static void SetEvent(int e) { g_events[g_nevents++] = e; }
static void Reset() { g_nevents = 0; g_performed = 0; g_panned = 0; g_shortcut = -1; }

static bool Typed(int type, bool editActive, bool redefinerSelected)
{
    bool typing = editActive || redefinerSelected;
    if (type == 1 && typing) { SetEvent(0x21); g_performed = 1; return true; }
    if (type == 0xf && typing) { SetEvent(0x22); g_performed = 1; return true; }
    return false;
}
static bool Redefiner(int type, bool selected, float wheel)
{
    const float DELTA_NOTIONAL_ZERO = 0.0001f;
    if (selected) {
        int e;
        switch (type) {
        case 4: e = 0x1a; break; case 7: e = 0x23; break; case 10: e = 0x1b; break; case 0x16: e = 0x26; break;
        case 0x17: e = 0x27; break; case 0x18: e = 0x28; break; case 0x19: e = 0x29; break; case 0x1a: e = 0x2a; break;
        default: return false;
        }
        SetEvent(e); g_performed = 1; return true;
    }
    if (type == 0xe) {
        if (wheel > DELTA_NOTIONAL_ZERO) SetEvent(0x24);
        else if (wheel < -DELTA_NOTIONAL_ZERO) SetEvent(0x25);
        else return false;
        g_performed = 1; return true;
    }
    return false;
}
static void LeftStick(float x, float y, bool panZoomed, bool exclusive)
{
    if (panZoomed) { g_panned = 1; g_performed = 1; }
    else if (y > 0.6f) { SetEvent(1); if (exclusive) SetEvent(0x11); g_performed = 1; }
    else if (y < -0.6f) { SetEvent(0); if (exclusive) SetEvent(0x10); g_performed = 1; }
    else if (x > 0.6f) { if (exclusive) SetEvent(0xf); g_performed = 1; }
    else if (x < -0.6f) { if (exclusive) SetEvent(0xe); g_performed = 1; }
}
static void Shortcut(int action)
{
    static const int actions[9] = { 0x5a, 0x5b, 0x5c, 0x5d, 0x64, 0x61, 0x62, 0x63, 0x60 };
    for (int i = 0; i < 9; ++i)
        if (actions[i] == action) { g_shortcut = i; g_performed = 1; return; }
}

int main()
{
    int ok = 1;
    Reset(); ok &= Typed(1, true, false) && g_nevents == 1 && g_events[0] == 0x21 && g_performed;
    Reset(); ok &= Typed(0xf, false, true) && g_events[0] == 0x22;
    Reset(); ok &= !Typed(1, false, false) && g_nevents == 0;
    Reset(); ok &= Redefiner(0x18, true, 0.0f) && g_events[0] == 0x28;
    Reset(); ok &= !Redefiner(5, true, 0.0f) && !g_performed;
    Reset(); ok &= Redefiner(0xe, false, 120.0f) && g_events[0] == 0x24;
    Reset(); ok &= Redefiner(0xe, false, -120.0f) && g_events[0] == 0x25;
    Reset(); ok &= !Redefiner(0xe, false, 0.0f);
    Reset(); LeftStick(0.0f, 0.9f, true, true); ok &= g_panned && g_nevents == 0;
    Reset(); LeftStick(0.0f, 0.9f, false, true); ok &= g_nevents == 2 && g_events[0] == 1 && g_events[1] == 0x11;
    Reset(); LeftStick(-0.9f, 0.0f, false, false); ok &= g_nevents == 0 && g_performed;
    Reset(); Shortcut(0x64); ok &= g_shortcut == 4;
    Reset(); Shortcut(0x60); ok &= g_shortcut == 8;
    Reset(); Shortcut(0x5e); ok &= g_shortcut == -1 && !g_performed;
    if (ok) printf("00689df0_TEST PASS\n"); else printf("00689df0_TEST FAIL\n");
    return ok ? 0 : 1;
}