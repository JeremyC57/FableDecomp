#include <cstdio>
#include <cstring>
// Behaviour copy of CArcheryCompetitionSignpost::Main (0x00cdec40): mirror the competition state onto the sign text each frame.
struct Data { long ShownState, CompetitionState; };
struct Game {
    int frames, doneAt; char tag[64]; int sets; Data* d; long schedule[8];
    bool IsDone() const { return frames >= doneAt; }
    void NewScriptFrame() { ++frames; d->CompetitionState = schedule[frames < 8 ? frames : 7]; }
    void SetTag(const char* t) { std::strcpy(tag, t); ++sets; }
};
static void Main(Game& g)
{
    g.SetTag("COME_BACK_LATER");
    g.d->CompetitionState = 2;
    while (!g.IsDone()) {
        if (g.d->ShownState != g.d->CompetitionState) {
            if (g.IsDone()) return;
            g.d->ShownState = g.d->CompetitionState;
            long state = g.d->ShownState;
            if (state == 0) { if (g.IsDone()) return; g.SetTag("OPEN"); }
            else if (state == 2) { if (g.IsDone()) return; g.SetTag("COME_BACK_LATER"); }
            else if (state == 1) { if (g.IsDone()) return; g.SetTag("CLOSED"); }
        }
        g.NewScriptFrame();
    }
}
int main()
{
    Data d = { 5, 0 };
    Game g = { 0, 6, "", 0, &d, { 2, 2, 0, 0, 1, 7, 7, 7 } };
    Main(g);
    // frame0: shown 5 != 2 -> COME_BACK_LATER; frame2: OPEN; frame4: CLOSED; frame5: state 7 -> no tag change
    bool ok = g.sets == 4 && std::strcmp(g.tag, "CLOSED") == 0 && d.ShownState == 7 && g.frames == 6;
    Data d2 = { 2, 0 }; Game h = { 0, 0, "", 0, &d2, { 0 } };
    Main(h);
    ok = ok && h.sets == 1 && std::strcmp(h.tag, "COME_BACK_LATER") == 0 && d2.ShownState == 2;
    std::printf(ok ? "00cdec40_TEST PASS\n" : "00cdec40_TEST FAIL\n");
    return ok ? 0 : 1;
}
