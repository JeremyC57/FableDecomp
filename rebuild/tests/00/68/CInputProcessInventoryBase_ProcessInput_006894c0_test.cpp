#include <cstdio>
// Behaviour test for CInputProcessInventoryBase::ProcessInput (0x006894C0): stick past +-0.6 on Y changes the selection,
// past +-0.9 on X pages left/right, else the current movement clears; actions 0x25..0x28 on press/release go to the screen.
struct CScreen { int Up, Down, Left, Right, PrevPage, NextPage, PrevTab, NextTab; };
struct CEvent { long Type; float X, Y; bool Pressed, Released; long Action; };
struct CProcessed { int Handled; void SetHandled() { Handled++; } };
struct CInv {
    CScreen* Screen; long CurrentMovement; bool Allow;
    CScreen* GetScreen() { return Screen; }
    bool ChangeSelection(long m) { CurrentMovement = m; return Allow; }
    void ProcessInput(const CEvent& event, CProcessed& processed)
    {
        CScreen* screen = GetScreen();
        if (event.Type == 0x11) {
            if (event.Y > 0.6f) { if (ChangeSelection(1)) screen->Up++; processed.SetHandled(); }
            else if (event.Y < -0.6f) { if (ChangeSelection(2)) screen->Down++; processed.SetHandled(); }
            else if (event.X < -0.9f) { screen->Left++; processed.SetHandled(); }
            else if (event.X > 0.9f) { screen->Right++; processed.SetHandled(); }
            else CurrentMovement = 0;
        }
        if (event.Pressed || event.Released) {
            if (event.Action == 0x25) { if (ChangeSelection(1)) GetScreen()->PrevPage++; processed.SetHandled(); }
            else if (event.Action == 0x26) { if (ChangeSelection(1)) GetScreen()->NextPage++; processed.SetHandled(); }
            else if (event.Action == 0x27) { GetScreen()->PrevTab++; processed.SetHandled(); }
            else if (event.Action == 0x28) { GetScreen()->NextTab++; processed.SetHandled(); }
        }
    }
};
static int Fail(int n) { std::printf("006894c0_TEST FAIL %d\n", n); return 1; }
int main()
{
    CScreen s = { 0, 0, 0, 0, 0, 0, 0, 0 };
    CInv inv = { &s, 5, true };
    CProcessed p = { 0 };
    CEvent up = { 0x11, 0.0f, 0.7f, false, false, 0 };
    inv.ProcessInput(up, p);
    if (s.Up != 1 || p.Handled != 1 || inv.CurrentMovement != 1) return Fail(1);
    CEvent down = { 0x11, 0.0f, -0.7f, false, false, 0 };
    inv.Allow = false;
    inv.ProcessInput(down, p);
    if (s.Down != 0 || p.Handled != 2 || inv.CurrentMovement != 2) return Fail(2);
    inv.Allow = true;
    CEvent left = { 0x11, -0.95f, 0.0f, false, false, 0 }, right = { 0x11, 0.95f, 0.5f, false, false, 0 };
    inv.ProcessInput(left, p); inv.ProcessInput(right, p);
    if (s.Left != 1 || s.Right != 1 || p.Handled != 4) return Fail(3);
    CEvent idle = { 0x11, 0.5f, 0.5f, false, false, 0 };
    inv.ProcessInput(idle, p);
    if (inv.CurrentMovement != 0 || p.Handled != 4) return Fail(4);
    CEvent a25 = { 1, 0, 0, true, false, 0x25 }, a26 = { 1, 0, 0, false, true, 0x26 }, a27 = { 1, 0, 0, true, false, 0x27 };
    CEvent a28 = { 1, 0, 0, true, false, 0x28 }, held28 = { 1, 0, 0, false, false, 0x28 }, other = { 1, 0, 0, true, false, 3 };
    inv.ProcessInput(a25, p); inv.ProcessInput(a26, p); inv.ProcessInput(a27, p); inv.ProcessInput(a28, p);
    if (s.PrevPage != 1 || s.NextPage != 1 || s.PrevTab != 1 || s.NextTab != 1 || p.Handled != 8) return Fail(5);
    inv.ProcessInput(held28, p); inv.ProcessInput(other, p);
    if (s.NextTab != 1 || p.Handled != 8) return Fail(6);
    std::printf("006894c0_TEST PASS\n");
    return 0;
}