// Behaviour test for CTCTrapBase::Trigger (0x00788229): no explosion def, only the trap-specific trigger and the
// triggered flag; with an explosion every live hit and immune thing is made immune to it; with an obstruction def too,
// the obstruction (explosion physics radius, 100 s) is remembered and ignores the live immune things; a dead explosion
// skips all of that.
#include <cstdio>
#include <string>
#include <vector>

struct Thing { bool alive; char id; };
struct Trap {
    long explosionIndex, obstructionIndex; bool explosionAlive, obstructionAlive; float radius;
    std::vector<Thing*> hit, immune; bool triggered; Thing* remembered; std::string log;
    void Trigger()
    {
        if (explosionIndex >= 0 && explosionAlive) {
            for (size_t i = 0; i < hit.size(); ++i) if (hit[i] && hit[i]->alive) { log += "x"; log += hit[i]->id; }
            for (size_t i = 0; i < immune.size(); ++i) if (immune[i] && immune[i]->alive) { log += "x"; log += immune[i]->id; }
            if (obstructionIndex >= 0) {
                static Thing obstruction = { true, 'O' };
                char buf[32]; std::sprintf(buf, "O%g", radius); log += buf;
                if (obstructionAlive) {
                    remembered = &obstruction; log += "L100";
                    for (size_t i = 0; i < immune.size(); ++i) if (immune[i] && immune[i]->alive) { log += "i"; log += immune[i]->id; }
                }
            }
        }
        log += "T"; triggered = true;
    }
};

static Trap Make(long explosion, long obstruction, bool explosionAlive, Thing** hit, int hits, Thing** immune, int immunes)
{
    Trap t;
    t.explosionIndex = explosion; t.obstructionIndex = obstruction; t.explosionAlive = explosionAlive;
    t.obstructionAlive = true; t.radius = 2.5f; t.triggered = false; t.remembered = 0;
    t.hit.assign(hit, hit + hits); t.immune.assign(immune, immune + immunes);
    return t;
}

int main()
{
    int ok = 1;
    Thing a = { true, 'a' }, b = { false, 'b' }, c = { true, 'c' };
    Thing* h1[] = { &a }; Thing* i1[] = { &c };
    Trap t = Make(-1, 0, true, h1, 1, i1, 1);
    t.Trigger(); ok &= t.log == "T" && t.triggered;
    Thing* h2[] = { &a, &b, 0 }; Thing* i2[] = { &b, &c };
    Trap u = Make(3, -1, true, h2, 3, i2, 2);
    u.Trigger(); ok &= u.log == "xaxcT" && u.remembered == 0;
    Thing* i3[] = { &c, &b };
    Trap v = Make(3, 5, true, h1, 1, i3, 2);
    v.Trigger(); ok &= v.log == "xaxcO2.5L100icT" && v.remembered != 0;
    Trap w = Make(3, 5, false, h1, 1, i1, 1);
    w.Trigger(); ok &= w.log == "T" && w.triggered;
    std::printf(ok ? "00788229_TEST PASS\n" : "00788229_TEST FAIL\n");
    return ok ? 0 : 1;
}
