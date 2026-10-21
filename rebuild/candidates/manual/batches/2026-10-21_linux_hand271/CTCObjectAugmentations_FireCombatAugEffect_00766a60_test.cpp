// Behaviour test for CTCObjectAugmentations::FireCombatAugEffect (0x00766A60): set InStrikeAction, kill the live running
// effects, empty the running list, then attempt every attack particle with its own type / frames / dummies.
#include <cstdio>

struct Thing { bool dead; int kills; };
struct Ptr { Thing* item; };
static bool Alive(const Ptr& p) { if (p.item != 0) { if (!p.item->dead) return true; } return false; }
struct Particle { int start, end; long type, frames; int attempts; };
struct Aug { bool inStrike; Ptr running[3]; int runningCount; Particle attack[2][3]; int inner[2]; int outer; long typeSum; };
static void FireCombatAugEffect(Aug& a)
{
    a.inStrike = true;
    for (long i = 0; i < a.runningCount; ++i)
        if (Alive(a.running[i])) a.running[i].item->kills++;
    a.runningCount = 0;
    for (long i = 0; i < a.outer; ++i)
        for (long j = 0; j < a.inner[i]; ++j) {
            Particle& p = a.attack[i][j];
            p.attempts++;
            a.typeSum += p.type * 10 + p.frames;
        }
}
int main()
{
    Thing r0 = { false, 0 }, r1 = { true, 0 };
    Aug a;
    a.inStrike = false; a.running[0].item = &r0; a.running[1].item = &r1; a.running[2].item = 0; a.runningCount = 3;
    for (int i = 0; i < 2; ++i) for (int j = 0; j < 3; ++j) { a.attack[i][j].type = i + 1; a.attack[i][j].frames = j; a.attack[i][j].attempts = 0; }
    a.inner[0] = 3; a.inner[1] = 1; a.outer = 2; a.typeSum = 0;
    FireCombatAugEffect(a);
    if (!a.inStrike || r0.kills != 1 || r1.kills != 0 || a.runningCount != 0 || a.attack[0][2].attempts != 1 ||
        a.attack[1][1].attempts != 0 || a.typeSum != (10 + 0) + (10 + 1) + (10 + 2) + 20) { std::printf("00766a60_TEST FAIL\n"); return 1; }
    FireCombatAugEffect(a);
    if (r0.kills != 1 || a.attack[1][0].attempts != 2) { std::printf("00766a60_TEST FAIL\n"); return 1; }
    std::printf("00766a60_TEST PASS\n");
    return 0;
}
