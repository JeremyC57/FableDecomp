// behaviour test for 0x00635080 CPlayer::InitModeNavigateInventoryExperienceScreen(EPlayerMode, CProcessedInput*): the character's
// interface-0x16 component is opened once (looked up only when its bit is set), both input processes are attached to the
// player interface in order, and the character is fetched four times when alive (lookup, null test, dead test, discarded
// fetch) and three times when dead
#include <cstdio>
struct Entry { int Type; int Component; };
static int gFetch = 0, gOpened = -1, gAttach[2], gNAttach = 0;
struct Thing { unsigned int Bits[9]; Entry* First; Entry* Last; int Dead; };
static Thing* Fetch(Thing* t) { ++gFetch; return t; }
static Entry* LowerBound(Thing* t, int key) { Entry* e = t->First; while (e != t->Last && e->Type < key) ++e; return e; }
static void Attach(int process, int iface) { gAttach[gNAttach++] = process * 100 + iface; }
static void Run(Thing* character, int heroScreens, int screen, int iface)   // the function under test
{
    const int type = 0x16;
    int component = 0;
    Thing* t = Fetch(character);
    if (t->Bits[type / 32] & (1u << (type % 32))) {
        Entry* it = LowerBound(t, type);
        if (it == t->Last || type < it->Type) it = t->Last;
        component = it->Component;
    }
    gOpened = component;
    Attach(heroScreens, iface);
    Attach(screen, iface);
    if (Fetch(character) && !Fetch(character)->Dead) Fetch(character);
}
int main()
{
    int fail = 0;
    Entry e[3]; e[0].Type = 0x11; e[0].Component = 1; e[1].Type = 0x16; e[1].Component = 7; e[2].Type = 0x40; e[2].Component = 9;
    Thing t; for (int i = 0; i < 9; ++i) t.Bits[i] = 0;
    t.Bits[0x16 / 32] |= 1u << (0x16 % 32); t.First = e; t.Last = e + 3; t.Dead = 0;
    Run(&t, 3, 4, 5);
    if (gOpened != 7) fail |= 1;
    if (gNAttach != 2 || gAttach[0] != 305 || gAttach[1] != 405) fail |= 2;
    if (gFetch != 4) fail |= 4;
    gFetch = 0; gNAttach = 0; t.Dead = 1;
    Run(&t, 3, 4, 5);
    if (gFetch != 3) fail |= 8;
    if (sizeof(Entry) != (unsigned int)8) fail |= 16;
    if (fail) { std::printf("00635080_TEST FAIL %d\n", fail); return 1; }
    std::printf("00635080_TEST PASS\n");
    return 0;
}