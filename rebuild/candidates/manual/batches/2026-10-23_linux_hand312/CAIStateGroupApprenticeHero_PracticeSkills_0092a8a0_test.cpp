// Behaviour test for CAIStateGroup_ApprenticeHero::PracticeSkills (0x0092A8A0): dummy-type setup, 5..14 repetitions, failure exits, cleanup.
#include <cstdio>
#include <cstring>
struct Apprentice {
    bool hasDummy; int DummyType; unsigned int seed;
    int idled, looks, plays, wanders, given, destroyed; char lastGiven[32], lastDestroyed[32];
    int failLookAt, failMove, randomCalls;
    void Give(const char* obj, const char* slot) { ++given; std::strcpy(lastGiven, obj); (void)slot; }
    void Destroy(const char* slot) { ++destroyed; std::strcpy(lastDestroyed, slot); }
    bool MoveToDummy() { return failMove-- != 0; }
    int LookAt() { ++looks; return looks == failLookAt ? 1 : 2; }
    int Play() { ++plays; return 2; }
    unsigned int Random() { ++randomCalls; return 4; }
    bool Practice()
    {
        if (!hasDummy) { ++idled; return true; }
        const char* first = ""; const char* second = "";
        switch (DummyType) {
        case 0: first = "ST_TRAINEE_STRIKE_DUMMY"; Give("OBJECT_IRON_LONGSWORD", "CARRY_SLOT_RIGHT_HAND"); break;
        case 1: first = "ST_TRAINEE_FIRE_BOW_LOAD"; second = "ST_TRAINEE_FIRE_BOW_RELEASE"; Give("OBJECT_YEW_LONGBOW", "CARRY_SLOT_LEFT_HAND"); break;
        case 2: first = "ST_TRAINEE_SPELLS"; break;
        }
        seed = seed * 0x24A1U + 0x24DFU;
        seed = (seed >> 13) | (seed << 19);
        long reps = (long)(seed % 10) + 5;
        bool ok = true;
        if (!MoveToDummy()) ok = false;
        for (long i = 0; ok && i < reps; ++i) {
            if (LookAt() != 2) { ok = false; break; }
            if (std::strcmp(first, "") != 0 && Play() != 2) { ok = false; break; }
            if (std::strcmp(second, "") != 0 && Play() != 2) { ok = false; break; }
            if ((DummyType == 0 || DummyType == 2) && (Random() & 3) == 0) { ++wanders; if (!MoveToDummy()) { ok = false; break; } }
        }
        if (DummyType == 0) Destroy("CARRY_SLOT_RIGHT_HAND");
        else if (DummyType == 1) Destroy("CARRY_SLOT_LEFT_HAND");
        return ok;
    }
};
static Apprentice make(int type, unsigned int seed)
{
    Apprentice a; std::memset(&a, 0, sizeof(a)); a.hasDummy = true; a.DummyType = type; a.seed = seed; a.failMove = -1; return a;
}
int main()
{
    Apprentice a = make(1, 0);
    unsigned int s = 0x24DFU; s = (s >> 13) | (s << 19); long reps = (long)(s % 10) + 5;
    if (!a.Practice() || a.looks != reps || a.plays != 2 * reps || a.destroyed != 1 || std::strcmp(a.lastGiven, "OBJECT_YEW_LONGBOW")
        || std::strcmp(a.lastDestroyed, "CARRY_SLOT_LEFT_HAND")) { std::printf("0092a8a0_TEST FAIL\n"); return 1; }
    Apprentice b = make(0, 0); b.failLookAt = 2;
    if (b.Practice() || b.looks != 2 || b.plays != 1 || std::strcmp(b.lastDestroyed, "CARRY_SLOT_RIGHT_HAND")) { std::printf("0092a8a0_TEST FAIL\n"); return 1; }
    Apprentice c = make(2, 7); c.failMove = 0;
    if (c.Practice() || c.looks != 0 || c.destroyed != 0 || c.given != 0) { std::printf("0092a8a0_TEST FAIL\n"); return 1; }
    Apprentice d = make(2, 7); d.hasDummy = false;
    if (!d.Practice() || d.idled != 1) { std::printf("0092a8a0_TEST FAIL\n"); return 1; }
    std::printf("0092a8a0_TEST PASS\n");
    return 0;
}
