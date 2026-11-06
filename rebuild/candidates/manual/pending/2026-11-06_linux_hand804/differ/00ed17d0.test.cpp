// Behaviour test for NScript::CV_SingingStonesScript::CManWithDoorName::Main (0x00ED17D0): the man who knows the door's name. Every
// frame until the thread ends (alive predicate): a hero hit says HIT (while he has health) and sets DoorManAttackedByHero; once bribed
// and not yet complete he says SPEAKS, sets DoorManComplete, runs to the nearest REGION_EXIT_POINT and fades out on arrival; a talk says
// INTRO (first), then REMINDER, or ATTACKED once he was hit; an item shown before the bribe is taken when it is OBJECT_GEMSTONE_RUBY
// (DoorManHasBribe), otherwise WRONG_ITEM. A state model, run for the bribe path, the attacked path and a terminate exit.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct DoorMan {
    int Frames, TerminateAt, ArriveAt;
    int Talks[8]; int NTalk; int Hits[8]; int NHit; int Items[8]; const char* Item[8]; int NItem;
    bool IntroComplete, AttackedByHero, HasBribe, Complete, Alive, Running;
    void NewScriptFrame() { ++Frames; if (Running && Frames >= ArriveAt) Running = false; }
    bool T() const { return Frames >= TerminateAt || !Alive; }
    bool In(const int* v, int n) const { for (int i = 0; i < n; ++i) if (v[i] == Frames) return true; return false; }
    const char* Presented() const { for (int i = 0; i < NItem; ++i) if (Items[i] == Frames) return Item[i]; return 0; }
    void Main()
    {
        NewScriptFrame();
        if (T()) return;
        Log("wander");
        if (T()) return;
        do {
            if (In(Hits, NHit)) { Log("HIT"); AttackedByHero = true; }
            if (HasBribe && !Complete) {
                Log("SPEAKS");
                Complete = true;
                Log("run"); Running = true;
                while (Running) { NewScriptFrame(); if (T()) return; }
                if (T()) return;
                Log("fade"); Alive = false;
            }
            if (In(Talks, NTalk)) {
                if (!IntroComplete) { Log("INTRO"); IntroComplete = true; }
                else if (!AttackedByHero) { Log("REMINDER"); IntroComplete = true; }
                else Log("ATTACKED");
            }
            const char* item = Presented();
            if (!HasBribe && item) {
                if (!std::strcmp(item, "OBJECT_GEMSTONE_RUBY")) { Log("ruby"); HasBribe = true; }
                else Log("WRONG_ITEM");
            }
            NewScriptFrame();
        } while (!T());
    }
};
int main()
{
    DoorMan a = { 0, 100, 9, { 2, 3 }, 2, { 0 }, 0, { 4, 5 }, { "OBJECT_GOLD", "OBJECT_GEMSTONE_RUBY" }, 2, false, false, false, false, true, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "wander;INTRO;REMINDER;WRONG_ITEM;ruby;SPEAKS;run;fade;") && a.Complete && a.HasBribe && !a.Alive;
    g_log[0] = 0;
    DoorMan b = { 0, 7, 99, { 2, 4 }, 2, { 3 }, 1, { 0 }, { 0 }, 0, false, false, false, false, true, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "wander;INTRO;HIT;ATTACKED;") && b.AttackedByHero && b.Frames == 7;
    g_log[0] = 0;
    DoorMan c = { 0, 1, 99, { 1 }, 1, { 0 }, 0, { 0 }, { 0 }, 0, false, false, false, false, true, false };
    c.Main();
    ok = ok && g_log[0] == 0 && c.Frames == 1;
    std::printf(ok ? "00ed17d0_TEST PASS\n" : "00ed17d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
