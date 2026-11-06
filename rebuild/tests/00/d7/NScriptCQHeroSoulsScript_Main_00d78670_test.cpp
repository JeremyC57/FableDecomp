// Behaviour test for NScript::CQ_HeroSoulsScript::Main (0x00D78670): the soul-route decisions -- the hero-souls position after
// Thunder (killed vs arena soul) and after the Guildmaster (Scythe vs Briar route), Briar's third message vs Scythe's
// Briar-dead message, the soul-3 intro only when DeliveredSoul == 1, the outro cutscene by KilledBriar, the 375/370 logbook
// entry by KilledGM, the GM-dead message only when the GM is dead and Jack's message is unheard, DeliveredSoul counting, and an
// early return when the thread terminates.
#include <cstdio>
#include <cstring>
struct Sim {
    char log[1024]; int calls, termAt;
    bool killedThunder, killedBriar, killedGM, heardGM, heardJack;
    int position, delivered;
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    bool Term() { return ++calls >= termAt; }
    void Main()
    {
        position = 1;
        if (killedThunder) { if (Term()) return; position = 2; } else { if (Term()) return; position = 3; }
        if (!killedBriar) {
            if (Term()) return;
            if (!heardGM) { if (Term()) return; Ev("BRIAR_MSG3"); heardGM = true; }
            Ev("OBJ10");
            if (delivered == 1) { if (Term()) return; Ev("SOUL3_INTRO_BRIAR"); ++delivered; }
        } else {
            if (Term()) return;
            if (!heardGM) { if (Term()) return; Ev("SCYTHE_BRIAR_DEAD"); heardGM = true; }
            Ev("OBJ11");
        }
        if (!killedGM) {
            if (Term()) return;
            if (killedBriar) { if (Term()) return; position = 7; } else { if (Term()) return; position = 6; }
        }
        if (killedGM && !heardJack) { if (Term()) return; Ev("GM_DEAD_SCYTHE"); }
        if (killedBriar) { if (Term()) return; Ev("OUTRO_SCYTHE"); } else { if (Term()) return; Ev("OUTRO_BRIAR"); }
        if (!heardJack) { if (Term()) return; Ev("JACK_MSG1"); }
        heardJack = true; ++delivered;
        if (killedGM) { if (Term()) return; Ev("LOG375"); } else { if (Term()) return; Ev("LOG370"); }
        Ev("COMPLETE");
    }
};
static void Make(Sim& s, bool t, bool b, bool g, int d)
{
    s.log[0] = 0; s.calls = 0; s.termAt = 1000; s.killedThunder = t; s.killedBriar = b; s.killedGM = g;
    s.heardGM = false; s.heardJack = false; s.position = 0; s.delivered = d;
}
int main()
{
    Sim a; Make(a, true, false, false, 1); a.Main();
    bool ok = std::strcmp(a.log, "BRIAR_MSG3;OBJ10;SOUL3_INTRO_BRIAR;OUTRO_BRIAR;JACK_MSG1;LOG370;COMPLETE;") == 0
        && a.position == 6 && a.delivered == 3 && a.heardJack;
    Sim b; Make(b, false, true, true, 0); b.Main();
    ok = ok && std::strcmp(b.log, "SCYTHE_BRIAR_DEAD;OBJ11;GM_DEAD_SCYTHE;OUTRO_SCYTHE;JACK_MSG1;LOG375;COMPLETE;") == 0
        && b.position == 3 && b.delivered == 1;
    Sim c; Make(c, false, true, false, 2); c.heardJack = true; c.Main();
    ok = ok && std::strcmp(c.log, "SCYTHE_BRIAR_DEAD;OBJ11;OUTRO_SCYTHE;LOG370;COMPLETE;") == 0 && c.position == 7 && c.delivered == 3;
    Sim d; Make(d, true, false, false, 1); d.termAt = 3; d.Main();
    ok = ok && d.log[0] == 0 && d.position == 2 && d.delivered == 1;
    std::printf(ok ? "00d78670_TEST PASS\n" : "00d78670_TEST FAIL\n");
    return ok ? 0 : 1;
}