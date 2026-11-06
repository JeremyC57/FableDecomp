// Behaviour test for NScript::CQ_HeroSoulsArenaScript::Main (0x00D7C890): the ArenaLevelReached state machine (exterior door made
// usable, Hall of Heroes cut-scene, Jack's pre-arena message once, cell exits made usable), then the loop until Thunder is killed: the
// arena-start block once (card, start screen, objective 04, theme, bodyguards teleported by count), and the success cut-scene that
// leaves the loop; a collected soul completes the quest with the PLATINUM_350 log entry.
#include <cstdio>
#include <string>
#include <vector>
enum { ALR_NOWHERE, ALR_ARENA_EXTERIOR, ALR_ARENA_HOH, ALR_ARENA_CELLS };
struct Sim {
    std::string log; int frame, termFrame, ArenaLevelReached, bodyguardCount, threads;
    bool PlayedHoHCS, JackTauntNearHOH, PlayedArenaCS, FirstWaveAttack, KilledThunder, CollectedSoulFromArena;
    int arenaFrame, soulFrame;
    bool Term() const { return frame >= termFrame; }
    void Ev(const char* e) { log += e; log += ';'; }
    void Main()
    {
        Ev("POST");
        if (ArenaLevelReached == ALR_NOWHERE) { if (Term()) return; Ev("DOOR"); ArenaLevelReached = ALR_ARENA_EXTERIOR; }
        ++threads;
        if (!PlayedHoHCS) { if (Term()) return; Ev("CS_GUARDS"); PlayedHoHCS = true; ArenaLevelReached = ALR_ARENA_HOH; }
        ++threads;
        if (!JackTauntNearHOH) { if (Term()) return; Ev("PRE_ARENA"); JackTauntNearHOH = true; }
        if (ArenaLevelReached == ALR_ARENA_HOH) { if (Term()) return; Ev("CELLS"); ArenaLevelReached = ALR_ARENA_CELLS; }
        while (!KilledThunder) {
            ++frame; if (frame >= soulFrame) CollectedSoulFromArena = true;
            if (Term()) return;
            if (!PlayedArenaCS && frame >= arenaFrame) {
                if (Term()) return;
                Ev("CS_START"); PlayedArenaCS = true; Ev("CARD"); Ev("START"); Ev("OBJ04"); Ev("THEME");
                std::vector<int> bodyguards(bodyguardCount, 0);
                if (bodyguards.size() == 1) { if (Term()) return; Ev("BG1"); }
                if (bodyguards.size() == 2) { if (Term()) return; Ev("BG1"); Ev("BG2"); }
                Ev("FADE"); FirstWaveAttack = true; ++threads;
            }
            if (CollectedSoulFromArena) { if (Term()) return; Ev("CS_SUCCESS"); Ev("THEME_DEFAULT"); break; }
        }
        if (Term()) return;
        if (CollectedSoulFromArena) { if (Term()) return; Ev("LOG350"); Ev("COMPLETE"); }
    }
};
static Sim Make()
{
    Sim s; s.frame = 0; s.termFrame = 1000; s.ArenaLevelReached = ALR_NOWHERE; s.bodyguardCount = 2; s.threads = 0;
    s.PlayedHoHCS = s.JackTauntNearHOH = s.PlayedArenaCS = s.FirstWaveAttack = s.KilledThunder = s.CollectedSoulFromArena = false;
    s.arenaFrame = 2; s.soulFrame = 5;
    return s;
}
int main()
{
    Sim a = Make(); a.Main();
    bool ok = a.log == "POST;DOOR;CS_GUARDS;PRE_ARENA;CELLS;CS_START;CARD;START;OBJ04;THEME;BG1;BG2;FADE;CS_SUCCESS;THEME_DEFAULT;LOG350;COMPLETE;"
        && a.threads == 3 && a.FirstWaveAttack && a.ArenaLevelReached == ALR_ARENA_CELLS && a.frame == 5;
    Sim b = Make(); b.ArenaLevelReached = ALR_ARENA_CELLS; b.PlayedHoHCS = b.JackTauntNearHOH = b.PlayedArenaCS = true;
    b.KilledThunder = true; b.Main();
    ok = ok && b.log == "POST;" && b.threads == 2;
    Sim c = Make(); c.bodyguardCount = 1; c.soulFrame = 100; c.termFrame = 4; c.Main();
    ok = ok && c.log == "POST;DOOR;CS_GUARDS;PRE_ARENA;CELLS;CS_START;CARD;START;OBJ04;THEME;BG1;FADE;" && c.frame == 4;
    std::printf(ok ? "00d7c890_TEST PASS\n" : "00d7c890_TEST FAIL\n");
    return ok ? 0 : 1;
}