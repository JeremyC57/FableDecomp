// Behaviour test for NScript::CQ_AwakeningTheOracleScript::CNecropolisGhost::Main (0x00CFBDF0): a Necropolis grave ghost. It takes
// the next grave number and the next free digging point and stands at that point's GhostPos (marked SpotTaken); while the spot is
// not dug it flashes every 8 frames and, when the hero is within 15 units and the grave number is below 13, adds the line
// TEXT_QST_B03_GHOST_(MALE_)NEAR_GRAVE<n> every 16 frames; once dug it flashes and moves to the first point whose GhostPos is not
// taken (new grave number) or removes itself. A frame-stepped model with digging points as indices.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Parent { int GraveNumbersGivenOut, PositionTaken, NPoints; bool Taken[4]; int DugAt[4]; };
struct Ghost {
    Parent* P; int Frame, TerminateAt, Grave, Point; bool Male, Removed; float HeroDist;
    bool T() const { return Frame >= TerminateAt; }
    bool Enabled() const { return Frame < P->DugAt[Point]; }
    void Main()
    {
        ++Frame; if (T()) return;
        Point = 0;
        Grave = ++P->GraveNumbersGivenOut;
        if (P->NPoints > P->PositionTaken) Point = P->PositionTaken++;
        P->Taken[Point] = true;
        for (; !T() && !Removed; ++Frame) {
            int flash = 0, conv = 0;
            while (Enabled()) {
                ++Frame; if (flash) --flash; if (conv) --conv;
                if (T()) return;
                if (!flash) { Log("flash"); flash = 8; }
                if (!conv && HeroDist < 15.0f && Grave < 13) {
                    char line[64]; std::sprintf(line, Male ? "TEXT_QST_B03_GHOST_MALE_NEAR_GRAVE%d" : "TEXT_QST_B03_GHOST_NEAR_GRAVE%d", Grave);
                    Log("flash"); Log(line); conv = 16;
                }
            }
            if (T()) return;
            Log("flash");
            int i = 0;
            for (; i < P->NPoints; ++i) { Point = i; if (!P->Taken[i]) break; }
            if (P->NPoints && !P->Taken[Point]) { P->Taken[Point] = true; Grave = ++P->GraveNumbersGivenOut; Log("moved"); }
            else { Log("removed"); Removed = true; }
        }
    }
};
int main()
{
    Parent p = { 0, 0, 2, { false, false }, { 10, 30 } };
    Ghost g = { &p, 0, 100, 0, 0, true, false, 5.0f };
    g.Main();
    bool ok = !std::strcmp(g_log, "flash;flash;TEXT_QST_B03_GHOST_MALE_NEAR_GRAVE1;flash;flash;moved;flash;flash;TEXT_QST_B03_GHOST_MALE_NEAR_GRAVE2;flash;flash;flash;TEXT_QST_B03_GHOST_MALE_NEAR_GRAVE2;flash;removed;")
        && g.Removed && p.GraveNumbersGivenOut == 2;
    g_log[0] = 0;
    Parent q = { 12, 0, 1, { false }, { 1000 } };
    Ghost h = { &q, 0, 12, 0, 0, false, false, 1.0f };
    h.Main();
    ok = ok && !std::strcmp(g_log, "flash;flash;") && h.Grave == 13;
    std::printf(ok ? "00cfbdf0_TEST PASS\n" : "00cfbdf0_TEST FAIL\n");
    return ok ? 0 : 1;
}
