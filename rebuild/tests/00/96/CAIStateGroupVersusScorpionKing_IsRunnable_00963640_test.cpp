// Behaviour test for CAIStateGroup_VersusScorpionKing::IsRunnable (0x00963640): same logic over mocks.
#include <cstdio>
#include <cstring>
struct CDef { int refs; int destroyed; int type; };
struct CThing { bool dead; CDef* subDef; };
static unsigned int Ror13(unsigned int v) { return (v >> 13) | (v << 19); }
struct CState {
    CThing* enemyOfSource; CThing* arenaMarker; unsigned int seed;
    CThing* PEnemy; bool GoClockwise; CThing* PArenaCentreThing; const char* lastName;
    bool GetSubDef(CThing* t, CDef** out) { if (!t->subDef) return false; *out = t->subDef; ++t->subDef->refs; return true; }
    bool GetPFirstThing(const char* name, CThing** out) { lastName = name; if (!arenaMarker) return false; *out = arenaMarker; return true; }
    bool IsRunnable()
    {
        CThing* enemy = enemyOfSource;
        CDef* def = 0;
        bool result = false;
        if (enemy != 0 && !enemy->dead) {
            if (GetSubDef(enemy, &def) && def->type == 0x80000) {
                CThing* centre;
                bool missing = !GetPFirstThing("ARENA_CentrePoint", &centre);
                PArenaCentreThing = missing ? enemy : centre;
                seed = seed * 0x24A1u + 0x24DFu;
                seed = Ror13(seed);
                GoClockwise = seed % 100 > 50;
                PEnemy = enemy;
                result = true;
            }
        }
        if (def && --def->refs == 0) ++def->destroyed;
        return result;
    }
};
int main()
{
    CDef d; d.refs = 1; d.destroyed = 0; d.type = 0x80000;
    CThing enemy; enemy.dead = false; enemy.subDef = &d;
    CThing marker; marker.dead = false; marker.subDef = 0;
    CState s; s.enemyOfSource = &enemy; s.arenaMarker = &marker; s.seed = 12345u; s.PEnemy = 0; s.GoClockwise = false;
    s.PArenaCentreThing = 0; s.lastName = 0;
    unsigned int expect = Ror13(12345u * 0x24A1u + 0x24DFu);
    bool ok = s.IsRunnable() && s.PArenaCentreThing == &marker && s.PEnemy == &enemy && d.refs == 1
        && s.seed == expect && s.GoClockwise == (expect % 100 > 50) && std::strcmp(s.lastName, "ARENA_CentrePoint") == 0;
    s.arenaMarker = 0; ok = ok && s.IsRunnable() && s.PArenaCentreThing == &enemy;
    d.type = 1; s.PEnemy = 0; ok = ok && !s.IsRunnable() && s.PEnemy == 0 && d.refs == 1;
    d.type = 0x80000; enemy.dead = true; ok = ok && !s.IsRunnable();
    enemy.dead = false; s.enemyOfSource = 0; ok = ok && !s.IsRunnable();
    if (!ok) { std::printf("00963640_TEST FAIL\n"); return 1; }
    std::printf("00963640_TEST PASS\n");
    return 0;
}