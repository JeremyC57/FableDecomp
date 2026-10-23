// Behaviour test for CSummonerAttacker::Main (0x00DF2380): wave gating, the health bar for summoner 1, the
// hit test (hit, or any special ability except 14) and the wardog respawn of whichever minion is dead.
#include <cstdio>

struct Sim {
    // world
    bool waveActive; long waveSummoner; long index;
    int heroFarFrames;        // frames the hero stays more than 15 units away
    int hitFrame;             // frame the hero hits Me (-1 never)
    bool anySpecial, special14;
    bool minion1Alive, minion2Alive;
    int terminateAt;
    // results
    int frame, healthBars, spawns, orbAttacks, markersAdded, markersRemoved;
    bool terminating() const { return frame >= terminateAt; }
    void newFrame() { ++frame; }
    bool heroFar() const { return heroFarFrames > 0; }
    bool hitThisFrame() const
    {
        if (frame != hitFrame) return false;
        return true;
    }
    void Main()
    {
        newFrame();
        if (terminating()) return;
        while (!(waveActive && waveSummoner == index)) {
            newFrame();
            if (terminating()) return;
        }
        ++markersAdded;
        if (index == 1) ++healthBars;
        if (heroFar()) {
            do {
                newFrame();
                if (terminating()) return;
                ++orbAttacks;
                bool hitBy = hitThisFrame();
                bool special = anySpecial && frame == hitFrame + 100;
                if (hitBy || (special && !special14)) {
                    if (!minion1Alive) { minion1Alive = true; ++spawns; }
                    if (!minion2Alive) { minion2Alive = true; ++spawns; }
                }
                --heroFarFrames;
            } while (heroFar());
        }
        ++markersRemoved;
    }
};

static Sim make(bool active, long wave, long idx, int far, int hit, bool m1, bool m2)
{
    Sim s;
    s.waveActive = active; s.waveSummoner = wave; s.index = idx;
    s.heroFarFrames = far; s.hitFrame = hit; s.anySpecial = false; s.special14 = false;
    s.minion1Alive = m1; s.minion2Alive = m2; s.terminateAt = 1000;
    s.frame = 0; s.healthBars = 0; s.spawns = 0; s.orbAttacks = 0; s.markersAdded = 0; s.markersRemoved = 0;
    return s;
}

int main()
{
    int fail = 0;
    Sim a = make(true, 1, 1, 5, 3, false, true);
    a.Main();
    if (a.healthBars != 1 || a.spawns != 1 || a.orbAttacks != 5 || a.markersAdded != 1 || a.markersRemoved != 1) fail = 1;

    Sim b = make(true, 2, 2, 4, -1, false, false);
    b.Main();
    if (b.healthBars != 0 || b.spawns != 0 || b.orbAttacks != 4) fail = 1;

    Sim c = make(true, 1, 2, 4, 2, false, false);   // another summoner's wave: never activates
    c.terminateAt = 20;
    c.Main();
    if (c.markersAdded != 0 || c.spawns != 0) fail = 1;

    Sim d = make(true, 3, 3, 3, 2, false, false);   // both minions dead: both respawn
    d.Main();
    if (d.spawns != 2) fail = 1;

    if (fail) { std::printf("00df2380_TEST FAIL\n"); return 1; }
    std::printf("00df2380_TEST PASS\n");
    return 0;
}