// Behaviour test for NScript::CV_ChapelOfEvilScript::Main (0x00E693F0): three entity bindings, a wait for a region whose name contains
// "Darkwood", seven threads, the quest activated once (guild master message, quest card, first objective), then each time DarkwoodChapel
// loads and the hero is inside: the first visit plays the three intro lines (when the altar lives), sets the second objective and
// records the initial morality; later visits play WELCOME_BACK or NOT_WELCOME (after an attack) and clear WeaponWarningComplete.
#include <cstdio>
#include <cstring>
#include <string>
struct Sim {
    std::string log; int frame, termFrame, bindings, threads, insideFrame;
    const char* regions[16]; int nRegions;
    bool QuestActivated, ChapelIntroComplete, InsideChapel, AttackedByHero, WeaponWarningComplete, altarAlive;
    float MoralityInitial, morality;
    bool Term() const { return frame >= termFrame; }
    void Ev(const char* e) { log += e; log += ';'; }
    void Frame() { ++frame; InsideChapel = frame >= insideFrame; }
    bool MsgOnRegionLoaded(std::string& region) { if (frame < nRegions && regions[frame]) { region = regions[frame]; return true; } return false; }
    void Main()
    {
        bindings = 3; Ev("POST");
        std::string region;
        for (;;) {
            if (Term()) return;
            while (!MsgOnRegionLoaded(region)) { Frame(); if (Term()) return; }
            if (Term()) return;
            if (std::strstr(region.c_str(), "Darkwood")) break;
            Frame();
        }
        if (Term()) return;
        threads = 5;
        if (!QuestActivated) { if (Term()) return; Ev("ACTIVATE"); Ev("GM_INTRO"); Ev("CARD"); Ev("OBJ01"); QuestActivated = true; }
        threads += 2;
        for (;;) {
            if (Term()) return;
            while (!MsgOnRegionLoaded(region)) { Frame(); if (Term()) return; }
            if (Term()) return;
            if (region == "DarkwoodChapel") {
                if (Term()) return;
                while (!InsideChapel) { Frame(); if (Term()) return; }
                if (Term()) return;
                if (!ChapelIntroComplete) {
                    if (Term()) return;
                    if (altarAlive) { if (Term()) return; Ev("INTRO_10"); Ev("INTRO_20"); Ev("INTRO_30"); }
                    ChapelIntroComplete = true; Ev("OBJ02"); MoralityInitial = morality;
                } else {
                    if (Term()) return;
                    if (altarAlive) {
                        if (Term()) return;
                        if (AttackedByHero) { if (Term()) return; Ev("NOT_WELCOME"); } else { if (Term()) return; Ev("WELCOME_BACK"); }
                    }
                    WeaponWarningComplete = false;
                }
            }
            Frame();
        }
    }
};
static Sim Make()
{
    Sim s; s.frame = 0; s.termFrame = 12; s.bindings = s.threads = 0; s.insideFrame = 0; s.nRegions = 0;
    std::memset(s.regions, 0, sizeof(s.regions));
    s.QuestActivated = s.ChapelIntroComplete = s.InsideChapel = s.AttackedByHero = false;
    s.WeaponWarningComplete = true; s.altarAlive = true; s.MoralityInitial = 0.0f; s.morality = 0.25f;
    return s;
}
int main()
{
    Sim a = Make(); a.nRegions = 10;
    a.regions[1] = "Oakvale"; a.regions[3] = "DarkwoodMarshes"; a.regions[4] = "DarkwoodChapel"; a.insideFrame = 6;
    a.regions[7] = "Darkwood"; a.regions[8] = "DarkwoodChapel";
    a.Main();
    bool ok = a.log == "POST;ACTIVATE;GM_INTRO;CARD;OBJ01;INTRO_10;INTRO_20;INTRO_30;OBJ02;WELCOME_BACK;" && a.bindings == 3
        && a.threads == 7 && a.ChapelIntroComplete && a.MoralityInitial == 0.25f && !a.WeaponWarningComplete && a.frame == 12;
    Sim b = Make(); b.nRegions = 4; b.regions[0] = "Darkwood"; b.regions[2] = "DarkwoodChapel";
    b.QuestActivated = true; b.ChapelIntroComplete = true; b.AttackedByHero = true; b.termFrame = 5;
    b.Main();
    ok = ok && b.log == "POST;NOT_WELCOME;" && !b.WeaponWarningComplete;
    Sim c = Make(); c.nRegions = 4; c.regions[1] = "Bowerstone"; c.termFrame = 3; c.Main();
    ok = ok && c.log == "POST;" && c.threads == 0;
    std::printf(ok ? "00e693f0_TEST PASS\n" : "00e693f0_TEST FAIL\n");
    return ok ? 0 : 1;
}