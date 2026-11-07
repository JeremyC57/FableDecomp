// Behaviour test for 0x0047ED76 CTCInGameMenu::SetToSave: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct Menu { long SaveGameIndex, FramesToWaitToSave; int comps; bool between; bool textFound; bool tagSet; float px, py; double t; };
static void SetToSave(Menu& m, long idx, float w, float h, float sx, float sy, double now)
{
    m.SaveGameIndex = idx; m.FramesToWaitToSave = 2; m.comps++;
    if (m.between && m.textFound) m.tagSet = true;
    m.px = w * 0.5f - sx * 204.0f; m.py = h * 0.5f - sy * 52.0f; m.t = now;
}
int main() {
    Menu m = { 0, 0, 0, true, true, false, 0, 0, 0 };
    SetToSave(m, 3, 1024.0f, 768.0f, 1.0f, 1.0f, 5.0);
    int ok = m.SaveGameIndex == 3 && m.FramesToWaitToSave == 2 && m.comps == 1 && m.tagSet && m.px == 308.0f && m.py == 332.0f && m.t == 5.0;
    Menu n = { 0, 0, 0, false, true, false, 0, 0, 0 }; SetToSave(n, 1, 800.0f, 600.0f, 2.0f, 2.0f, 1.0);
    ok &= !n.tagSet && n.px == 400.0f - 408.0f;
    std::printf(ok ? "0047ed76_TEST PASS\n" : "0047ed76_TEST FAIL\n");
    return ok ? 0 : 1;
}
