// Behaviour test for 0x007FC210 CTCKraken::CreateTentacles: a standalone model (editor guard, four carry slots with their facing
// angles, the phase-clamped tentacle def, each tentacle linked / non-targetable / appended, the cursor reset to the list end).
#include <cstdio>
#include <list>
#include <vector>

struct Tentacle { long def; float angle; long slot; bool linked, targetable, persisted, drawable; };
struct Kraken { std::vector<long> tentacleDefs; unsigned long phase; std::list<Tentacle> tentacles; bool targetable; bool cursorAtEnd; };

static void CreateTentacles(Kraken& k, bool editor)
{
    if (editor) return;
    std::vector<std::pair<long, float> > slots;
    slots.push_back(std::pair<long, float>(1, 0.375f)); slots.push_back(std::pair<long, float>(2, 0.625f));
    slots.push_back(std::pair<long, float>(3, 0.875f)); slots.push_back(std::pair<long, float>(4, 0.125f));
    unsigned long last = (unsigned long)k.tentacleDefs.size() - 1;
    long def = k.tentacleDefs[last < k.phase ? last : k.phase];
    for (unsigned long i = 0; i < slots.size(); ++i) {
        Tentacle t = { def, slots[i].second, slots[i].first, true, false, false, false };
        k.tentacles.push_back(t);
    }
    k.targetable = false;
    k.cursorAtEnd = true;
}

int main()
{
    Kraken k; k.tentacleDefs.push_back(10); k.tentacleDefs.push_back(20); k.phase = 5; k.targetable = true; k.cursorAtEnd = false;
    CreateTentacles(k, true);
    if (!k.tentacles.empty()) { std::printf("007fc210_TEST FAIL\n"); return 1; }
    CreateTentacles(k, false);
    if (k.tentacles.size() != 4 || k.tentacles.front().def != 20 || k.tentacles.back().angle != 0.125f || k.targetable ||
        !k.cursorAtEnd || k.tentacles.front().targetable) { std::printf("007fc210_TEST FAIL\n"); return 1; }
    Kraken k2; k2.tentacleDefs.push_back(10); k2.tentacleDefs.push_back(20); k2.phase = 0;
    CreateTentacles(k2, false);
    if (k2.tentacles.front().def != 10) { std::printf("007fc210_TEST FAIL\n"); return 1; }
    std::printf("007fc210_TEST PASS\n");
    return 0;
}