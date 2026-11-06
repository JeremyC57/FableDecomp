// Behaviour test for NScript::CQ_HeroSoulsGuildmasterScript::Main (0x00D84D60): autosave unless Briar was killed, the arrival cut-scene
// once, Jack's second taunt when the hero leaves the guild or the 200 timer runs out (once), the guild-master start cut-scene, a single
// taunt conversation when three wood guards remain, then the GM_CHOICE outcome: GMKILLED completes the quest, else it fails, the hero is
// teleported to the graveyard and the quest is deactivated once Graveyard loads. Each arm ends with FadeScreenIn.
#include <cstdio>
#include <string>
#include <map>
struct Sim {
    std::string log; int frame, termFrame, timer;
    bool KilledBriar, KilledGM, PlayedGuildCS, PlayedGuildMasterCS;
    int guildFrame, leaveGuildFrame, woodsFrame, graveFrame, convFrames; long GuardInWoodsCount;
    std::map<std::string, bool> cs_flags;
    bool Term() const { return frame >= termFrame; }
    void Ev(const char* e) { log += e; log += ';'; }
    void Frame() { ++frame; if (timer > 0) timer -= 50; if (frame > woodsFrame && GuardInWoodsCount > 0) --GuardInWoodsCount; }
    void Main()
    {
        if (!KilledBriar) { if (Term()) return; Ev("AUTOSAVE"); }
        Ev("OBJ11");
        if (!PlayedGuildCS) {
            if (Term()) return;
            while (frame < guildFrame) { Frame(); if (Term()) return; }
            if (Term()) return;
            Ev("CS_ARRIVAL"); PlayedGuildCS = true; Ev("TAUNT10");
        }
        Ev("OBJ12"); Ev("ACT_NOSTRO");
        bool tauntGiven = false; timer = 200;
        while (frame < woodsFrame) {
            Frame(); if (Term()) return;
            if (!tauntGiven && (frame >= leaveGuildFrame || timer == 0)) { if (Term()) return; Ev("TAUNT20"); tauntGiven = true; }
        }
        if (Term()) return;
        if (!PlayedGuildMasterCS) { if (Term()) return; Ev("CS_GM_START"); PlayedGuildMasterCS = true; }
        Ev("CARD"); Ev("START"); Ev("OBJ14"); Ev("FADE");
        bool messageGiven = false;
        while (GuardInWoodsCount > 0) {
            Frame(); if (Term()) return;
            if (!messageGiven && GuardInWoodsCount == 3) {
                if (Term()) return;
                Ev("GM_TAUNT"); int end = frame + convFrames;
                while (frame < end) { ++frame; if (Term()) return; }
                if (Term()) return;
                messageGiven = true;
            }
        }
        if (Term()) return;
        Ev("CS_GM_CHOICE");
        if (cs_flags["GMKILLED"]) {
            if (Term()) return;
            KilledGM = true; Ev("LOG375"); Ev("COMPLETE"); Ev("KILLED"); Ev("MORALITY"); Ev("REMOVE"); Ev("FADE");
        } else {
            if (Term()) return;
            Ev("FAILED"); Ev("TELEPORT"); Ev("REMOVE_CARD"); Ev("OBJ12A");
            while (frame < graveFrame) { ++frame; if (Term()) return; }
            if (Term()) return;
            Ev("DEACTIVATE"); Ev("FADE");
        }
    }
};
static Sim Make()
{
    Sim s; s.frame = 0; s.termFrame = 1000; s.timer = 0; s.KilledBriar = false; s.KilledGM = false;
    s.PlayedGuildCS = false; s.PlayedGuildMasterCS = false; s.guildFrame = 2; s.leaveGuildFrame = 100; s.woodsFrame = 9;
    s.graveFrame = 0; s.convFrames = 3; s.GuardInWoodsCount = 4;
    return s;
}
int main()
{
    Sim a = Make(); a.cs_flags["GMKILLED"] = true; a.Main();
    bool ok = a.log == "AUTOSAVE;OBJ11;CS_ARRIVAL;TAUNT10;OBJ12;ACT_NOSTRO;TAUNT20;CS_GM_START;CARD;START;OBJ14;FADE;GM_TAUNT;"
        "CS_GM_CHOICE;LOG375;COMPLETE;KILLED;MORALITY;REMOVE;FADE;" && a.KilledGM;
    Sim b = Make(); b.KilledBriar = true; b.PlayedGuildCS = true; b.PlayedGuildMasterCS = true; b.leaveGuildFrame = 1;
    b.GuardInWoodsCount = 0; b.graveFrame = 40; b.Main();
    ok = ok && b.log == "OBJ11;OBJ12;ACT_NOSTRO;TAUNT20;CARD;START;OBJ14;FADE;CS_GM_CHOICE;FAILED;TELEPORT;REMOVE_CARD;OBJ12A;DEACTIVATE;FADE;"
        && !b.KilledGM && b.frame == 40;
    Sim c = Make(); c.termFrame = 1; c.Main();
    ok = ok && c.log == "AUTOSAVE;OBJ11;";
    std::printf(ok ? "00d84d60_TEST PASS\n" : "00d84d60_TEST FAIL\n");
    return ok ? 0 : 1;
}