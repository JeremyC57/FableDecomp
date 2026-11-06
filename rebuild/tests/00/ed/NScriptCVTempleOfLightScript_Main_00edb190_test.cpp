// Behaviour test for NScript::CV_TempleOfLightScript::Main (0x00EDB190): two entity bindings, a wait for a region whose name contains
// "Witchwood", seven threads, the quest activated once (guild master message), then each time Witchwood3 loads and the hero is inside:
// the altar gets a minimap marker; the first visit plays the three intro lines (when the altar lives) and waits for the conversation,
// then gives the quest card, sets the first objective and records the initial morality; later visits play WELCOME_BACK or NOT_WELCOME
// (after an attack) and clear WeaponWarningComplete.
#include <cstdio>
#include <cstring>
#include <string>
struct Sim {
    std::string log; int frame, termFrame, bindings, threads, insideFrame, convFrames;
    const char* regions[16]; int nRegions;
    bool QuestActivated, TempleIntroComplete, InsideTemple, AttackedByHero, WeaponWarningComplete, altarAlive;
    float MoralityInitial, morality;
    bool Term() const { return frame >= termFrame; }
    void Ev(const char* e) { log += e; log += ';'; }
    void Frame() { ++frame; InsideTemple = frame >= insideFrame; }
    bool MsgOnRegionLoaded(std::string& region) { if (frame < nRegions && regions[frame]) { region = regions[frame]; return true; } return false; }
    void Main()
    {
        bindings = 2; Ev("POST");
        std::string region;
        for (;;) {
            if (Term()) return;
            while (!MsgOnRegionLoaded(region)) { Frame(); if (Term()) return; }
            if (Term()) return;
            if (std::strstr(region.c_str(), "Witchwood")) break;
            Frame();
        }
        if (Term()) return;
        threads = 5;
        if (!QuestActivated) { if (Term()) return; Ev("ACTIVATE"); Ev("GM_INTRO"); QuestActivated = true; }
        threads += 2;
        for (;;) {
            if (Term()) return;
            while (!MsgOnRegionLoaded(region)) { Frame(); if (Term()) return; }
            if (Term()) return;
            if (region == "Witchwood3") {
                if (Term()) return;
                while (!InsideTemple) { Frame(); if (Term()) return; }
                if (Term()) return;
                Ev("MARKER");
                if (!TempleIntroComplete) {
                    if (Term()) return;
                    if (altarAlive) {
                        if (Term()) return;
                        Ev("INTRO_10"); Ev("INTRO_20"); Ev("INTRO_30");
                        int end = frame + convFrames;
                        while (frame < end) { Frame(); if (Term()) return; }
                        if (Term()) return;
                    }
                    TempleIntroComplete = true; Ev("CARD"); Ev("OBJ01"); MoralityInitial = morality;
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
    Sim s; s.frame = 0; s.termFrame = 14; s.bindings = s.threads = 0; s.insideFrame = 0; s.convFrames = 2; s.nRegions = 0;
    std::memset(s.regions, 0, sizeof(s.regions));
    s.QuestActivated = s.TempleIntroComplete = s.InsideTemple = s.AttackedByHero = false;
    s.WeaponWarningComplete = true; s.altarAlive = true; s.MoralityInitial = 0.0f; s.morality = 0.75f;
    return s;
}
int main()
{
    Sim a = Make(); a.nRegions = 12;
    a.regions[1] = "Oakvale"; a.regions[3] = "Witchwood1"; a.regions[4] = "Witchwood3"; a.insideFrame = 6;
    a.regions[10] = "Witchwood3";
    a.Main();
    bool ok = a.log == "POST;ACTIVATE;GM_INTRO;MARKER;INTRO_10;INTRO_20;INTRO_30;CARD;OBJ01;MARKER;WELCOME_BACK;" && a.bindings == 2
        && a.threads == 7 && a.TempleIntroComplete && a.MoralityInitial == 0.75f && !a.WeaponWarningComplete && a.frame == 14;
    Sim b = Make(); b.nRegions = 4; b.regions[0] = "Witchwood"; b.regions[2] = "Witchwood3";
    b.QuestActivated = true; b.TempleIntroComplete = true; b.AttackedByHero = true; b.termFrame = 5;
    b.Main();
    ok = ok && b.log == "POST;MARKER;NOT_WELCOME;" && !b.WeaponWarningComplete;
    Sim c = Make(); c.nRegions = 4; c.regions[0] = "Witchwood2"; c.regions[1] = "Witchwood3"; c.altarAlive = false; c.termFrame = 3;
    c.Main();
    ok = ok && c.log == "POST;ACTIVATE;GM_INTRO;MARKER;CARD;OBJ01;" && c.MoralityInitial == 0.75f;
    std::printf(ok ? "00edb190_TEST PASS\n" : "00edb190_TEST FAIL\n");
    return ok ? 0 : 1;
}