// Behaviour test for 0x00E5D9A0 (CV_BodyGuardScript::HenchmanDeductWages): wages, the can't-afford wait, the timer re-arm.
#include <cstdio>
#include <cstring>
// Host model: the game logs 'F' frame, 'g<n>' gold change, 'p<n>' pause, 't<i>=<n>' timer set; the henchman count drops
// by one per frame once 'leaveFrom' frames have passed; terminating is a countdown of checks.
struct Data { long OverrideState; bool StandAndWait; bool InLimbo; };
struct Game {
    char log[512]; long gold; int frames; int henchmen; int leaveFrom;
    Game() : gold(0), frames(0), henchmen(0), leaveFrom(1000) { log[0] = 0; }
    void Add(const char* s) { if (std::strlen(log) + std::strlen(s) < 500) std::strcat(log, s); }
    void NewScriptFrame() { Add("F"); ++frames; if (frames >= leaveFrom && henchmen > 0) --henchmen; }
    long GetHeroGold() { return gold; }
    void GiveHeroGold(long n) { char b[32]; std::sprintf(b, "g%ld", n); Add(b); gold += n; }
    void Pause(float s) { char b[32]; std::sprintf(b, "p%d", (int)s); Add(b); }
    void SetTimer(long i, long t) { char b[32]; std::sprintf(b, "t%ld=%ld", i, t); Add(b); }
};
static long g_delay = 30;
struct Script {
    Game* G; Data* D; long TotalWages; bool HeroCannotAffordHelp; long FreakTimer; int termAfter; int checks; bool sawCannot;
    Script(Game* g, Data* d, long w, int after)
        : G(g), D(d), TotalWages(w), HeroCannotAffordHelp(false), FreakTimer(7), termAfter(after), checks(0), sawCannot(false) {}
    bool IsActiveThreadTerminating() { return ++checks > termAfter; }
    unsigned long size() const { return (unsigned long)G->henchmen; }
    void HenchmanDeductWages()
    {
        while (!IsActiveThreadTerminating()) {
            if (TotalWages > 0 && !D->StandAndWait && !D->InLimbo) {
                if (IsActiveThreadTerminating()) return;
                if (G->GetHeroGold() >= TotalWages) {
                    if (IsActiveThreadTerminating()) return;
                    if (D->OverrideState != 1) {
                        if (IsActiveThreadTerminating()) return;
                        G->GiveHeroGold(-TotalWages);
                        G->Pause((float)g_delay);
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    HeroCannotAffordHelp = true; sawCannot = true;
                    while (size() != 0) {
                        G->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    HeroCannotAffordHelp = false;
                }
            }
            if (size() == 0) {
                if (IsActiveThreadTerminating()) return;
                G->SetTimer(FreakTimer, 180);
            }
            G->NewScriptFrame();
        }
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    Data d = { 0, false, false };
    // rich hero, henchmen present: pay + pause each pass (1 + 3 + 0 checks per pass, frame)
    { Game g; g.gold = 100; g.henchmen = 2; Script s(&g, &d, 40, 9); s.HenchmanDeductWages();
      CHECK(std::strcmp(g.log, "g-40p30Fg-40p30F") == 0); CHECK(g.gold == 20); }
    // override state 1: no payment
    { Data d1 = { 1, false, false }; Game g; g.gold = 100; g.henchmen = 1; Script s(&g, &d1, 40, 7); s.HenchmanDeductWages();
      CHECK(std::strcmp(g.log, "FF") == 0); }
    // no wages, no henchmen: the timer is re-armed every pass
    { Game g; Script s(&g, &d, 0, 4); s.HenchmanDeductWages(); CHECK(std::strcmp(g.log, "t7=180Ft7=180F") == 0); }
    // stand-and-wait blocks payment
    { Data d2 = { 0, true, false }; Game g; g.gold = 100; g.henchmen = 1; Script s(&g, &d2, 40, 3); s.HenchmanDeductWages();
      CHECK(std::strcmp(g.log, "FFF") == 0); }
    // poor hero: flag set, wait until the henchmen have left, flag cleared, then timer
    { Game g; g.gold = 10; g.henchmen = 2; g.leaveFrom = 1; Script s(&g, &d, 40, 7); s.HenchmanDeductWages();
      CHECK(s.sawCannot && !s.HeroCannotAffordHelp); CHECK(std::strcmp(g.log, "FFt7=180F") == 0); }
    // terminating while waiting: the flag stays set
    { Game g; g.gold = 10; g.henchmen = 3; Script s(&g, &d, 40, 4); s.HenchmanDeductWages();
      CHECK(s.HeroCannotAffordHelp); CHECK(std::strcmp(g.log, "FF") == 0); }
    if (g_fail) { std::printf("00e5d9a0_TEST FAIL\n"); return 1; }
    std::printf("00e5d9a0_TEST PASS\n");
    return 0;
}