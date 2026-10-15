// Behaviour test for 0x0069F230 CCreatureAction_UseObject constructor: base gets the creature, a zero sound pair and two
// zero longs; PUsedThing points at the used thing; the bookkeeping members are cleared and AllowTurn follows the argument.
#include <cstdio>
struct CThing { int id; };
struct CSoundPair { int a, b; CSoundPair() : a(0), b(0) {} };
static int g_soundA = -1, g_soundB = -1, g_x = -1, g_y = -1; static CThing* g_creature = 0;
struct CBase {
    CBase(CThing* c, const CSoundPair& s, int x, int y) { g_creature = c; g_soundA = s.a; g_soundB = s.b; g_x = x; g_y = y; }
};
struct CIntelligentPointer { CThing* m_p; CIntelligentPointer(CThing* p) : m_p(0) { m_p = p; } };
struct CUseObject : CBase {
    CIntelligentPointer PUsedThing;
    void* PreviousPhysicsComponent;
    unsigned int Proximity;
    bool UsingDummy, MovingToDummy, TurningToDummy, AllowTurn;
    CUseObject(CThing* creature, CThing* used, bool allowTurn)
        : CBase(creature, CSoundPair(), 0, 0), PUsedThing(used), PreviousPhysicsComponent(0), Proximity(0),
          UsingDummy(false), MovingToDummy(false), TurningToDummy(false), AllowTurn(allowTurn) {}
};
int main()
{
    CThing creature, thing;
    CUseObject a(&creature, &thing, true);
    if (g_creature != &creature || g_soundA != 0 || g_soundB != 0 || g_x != 0 || g_y != 0 || a.PUsedThing.m_p != &thing
        || a.PreviousPhysicsComponent != 0 || a.Proximity != 0 || a.UsingDummy || a.MovingToDummy || a.TurningToDummy || !a.AllowTurn)
        { std::printf("0069f230_TEST FAIL\n"); return 1; }
    CUseObject b(&creature, 0, false);
    if (b.AllowTurn || b.PUsedThing.m_p != 0) { std::printf("0069f230_TEST FAIL\n"); return 1; }
    std::printf("0069f230_TEST PASS\n");
    return 0;
}
