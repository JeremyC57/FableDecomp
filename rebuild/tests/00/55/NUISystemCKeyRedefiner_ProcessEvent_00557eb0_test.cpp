#include <cstdio>
// Behaviour test for NUISystem::CKeyRedefiner::ProcessEvent (0x00557EB0): only the selected redefiner reacts; Escape (1)
// cancels, a redefinable key becomes a keyboard control, mouse events map to buttons, then redefine + end selection;
// everything else (or a non-redefinable key) falls through to the clickable handler.
static bool g_flag58 = false;
static bool CanRedefineAction(long category)
{
    switch (category) {
    case 1: case 89: case 90: case 91: case 98: case 118: case 119: return false;
    case 58: if (!g_flag58) break; return false;
    default: break;
    }
    return true;
}
struct Control { long GameAction, ControllerType, KeyboardKey, MouseButton; };
struct CKeyRedefiner;
static CKeyRedefiner* s_pSelected = 0;
static long g_lastKey = 0;
struct CKeyRedefiner {
    long Action; int Cancelled, Redefined, Clicked, Ended; Control Last;
    void CancelSelection() { if (s_pSelected == this) { s_pSelected = 0; Cancelled++; } }
    void Redefine(const Control& c) { Last = c; Redefined++; }
    void ProcessEvent(long event)
    {
        Control control = { Action, 0, 0, 0 };
        if (s_pSelected == this) {
            switch (event) {
            case 0x21: {
                long key = g_lastKey;
                if (key == 1) { CancelSelection(); return; }
                if (CanRedefineAction(key)) { control.ControllerType = 2; control.KeyboardKey = key; }
            } break;
            case 0x1a: control.ControllerType = 3; control.MouseButton = 1; break;
            case 0x1b: control.ControllerType = 3; control.MouseButton = 2; break;
            case 0x23: control.ControllerType = 3; control.MouseButton = 3; break;
            case 0x26: control.ControllerType = 3; control.MouseButton = 8; break;
            case 0x27: control.ControllerType = 3; control.MouseButton = 9; break;
            case 0x28: control.ControllerType = 3; control.MouseButton = 0xa; break;
            case 0x29: control.ControllerType = 3; control.MouseButton = 0xb; break;
            case 0x2a: control.ControllerType = 3; control.MouseButton = 0xc; break;
            }
            if (control.ControllerType != 0) {
                Redefine(control);
                if (s_pSelected == this) { s_pSelected = 0; Ended++; }
                return;
            }
        }
        Clicked++;
    }
};
static int Fail() { std::printf("00557eb0_TEST FAIL\n"); return 1; }
int main()
{
    CKeyRedefiner r = { 7, 0, 0, 0, 0, { 0, 0, 0, 0 } };
    r.ProcessEvent(0x21);
    if (r.Clicked != 1) return Fail();                          // not selected
    s_pSelected = &r; g_lastKey = 30; r.ProcessEvent(0x21);
    if (r.Redefined != 1 || r.Last.ControllerType != 2 || r.Last.KeyboardKey != 30 || r.Last.GameAction != 7 || s_pSelected) return Fail();
    s_pSelected = &r; g_lastKey = 1; r.ProcessEvent(0x21);
    if (r.Cancelled != 1 || s_pSelected) return Fail();
    s_pSelected = &r; g_lastKey = 90; r.ProcessEvent(0x21);
    if (r.Clicked != 2 || s_pSelected != &r) return Fail();     // not redefinable -> clickable
    g_lastKey = 58; g_flag58 = true; r.ProcessEvent(0x21);
    if (r.Clicked != 3) return Fail();
    g_flag58 = false; r.ProcessEvent(0x21);
    if (r.Redefined != 2 || r.Last.KeyboardKey != 58) return Fail();
    s_pSelected = &r; r.ProcessEvent(0x28);
    if (r.Last.ControllerType != 3 || r.Last.MouseButton != 0xa || r.Ended != 3) return Fail();
    s_pSelected = &r; r.ProcessEvent(0x50);
    if (r.Clicked != 4) return Fail();
    std::printf("00557eb0_TEST PASS\n");
    return 0;
}