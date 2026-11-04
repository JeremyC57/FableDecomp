// Behaviour test for CTCGhostSword::FrameUpdate (0x007C6460), g++ model: fade-in alpha per live minion, then the component goes when no minion lives.
#include <cstdio>
#include <vector>
struct Minion { bool alive; bool hasAppearance; int alpha; };
static bool FrameUpdate(std::vector<long>& timers, std::vector<Minion>& minions, long fadeIn, int& particleUpdates)
{
    for (long i = 0; i < (long)timers.size(); ++i) {
        if (timers[i] > 0) {
            timers[i]--;
            if (minions[i].alive && minions[i].hasAppearance)
                minions[i].alpha = (unsigned char)((1.0f - (float)timers[i] / fadeIn) * 255.0f);
        }
    }
    ++particleUpdates;
    for (unsigned j = 0; j < minions.size(); ++j) if (minions[j].alive) return false;
    return true;   // RemoveTC(0x9A)
}
int main()
{
    std::vector<long> t; t.push_back(10); t.push_back(0); t.push_back(5);
    std::vector<Minion> m(3); Minion a = { true, true, -1 }, b = { true, true, -1 }, c = { false, true, -1 }; m[0] = a; m[1] = b; m[2] = c;
    int pu = 0;
    bool removed = FrameUpdate(t, m, 10, pu);
    int ok = !removed && t[0] == 9 && t[1] == 0 && t[2] == 4 && m[0].alpha == (unsigned char)((1.0f - 0.9f) * 255.0f) && m[1].alpha == -1 && m[2].alpha == -1 && pu == 1;
    m[0].alive = m[1].alive = false;
    ok = ok && FrameUpdate(t, m, 10, pu) && pu == 2;
    std::printf(ok ? "007c6460_TEST PASS\n" : "007c6460_TEST FAIL\n");
    return ok ? 0 : 1;
}
