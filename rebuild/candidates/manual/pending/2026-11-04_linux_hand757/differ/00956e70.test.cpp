// Behaviour test for 0x00956E70 CAIStateGroup_BriarRose::Update: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct G { bool runnable; bool runnableAfter; long next; int terminated; int shields; long duration; int actions; };
static void Update(G& g, long frame, long fps, float wait)
{
    if (!g.runnable) { g.terminated++; return; }
    g.next = (long)((float)frame + (float)fps * wait);
    g.shields++; g.duration = (long)((float)fps * 30.0f); g.actions++;
    if (g.runnableAfter) g.next = (long)((float)frame + 10.0f + (float)fps * wait);
    g.terminated++;
}
int main() {
    G g = { true, false, 0, 0, 0, 0, 0 }; Update(g, 100, 30, 2.0f);
    int ok = g.next == 160 && g.duration == 900 && g.terminated == 1 && g.actions == 1;
    G h = { false, false, 0, 0, 0, 0, 0 }; Update(h, 1, 30, 1.0f); ok &= h.terminated == 1 && h.shields == 0;
    std::printf(ok ? "00956e70_TEST PASS\n" : "00956e70_TEST FAIL\n");
    return ok ? 0 : 1;
}
