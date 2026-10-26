// Behaviour test for CAIStateGroup_ScorpionKing::IsRunnable (0x0097B900): same logic over mocks.
#include <cstdio>
#include <cstring>
class CThing { public: int health; bool visible; };
struct CCombat { CThing* creature; CThing* enemy; bool flag; int calls; };
struct CState {
    CThing* creature; CThing* enemyOfSource; CThing* arenaMarker; CCombat combat; const char* lastName;
    CThing* PArenaCentre; CThing* PEnemy; int HealthAtLastTailStrike;
    bool GetPFirstThing(const char* name, CThing** out) { lastName = name; if (!arenaMarker) return false; *out = arenaMarker; return true; }
    bool IsRunnable()
    {
        CThing* arena = 0;
        GetPFirstThing("ScorpionArenaCentre", &arena);
        PArenaCentre = arena;
        CThing* enemy = enemyOfSource;
        if (enemy != 0) {
            if (enemy->visible) {
                PEnemy = enemy;
                HealthAtLastTailStrike = creature->health;
                combat.creature = creature; combat.enemy = PEnemy; combat.flag = true; ++combat.calls;
                return true;
            }
            PEnemy = 0;
        }
        return false;
    }
};
int main()
{
    CThing me; me.health = 77; me.visible = true;
    CThing enemy; enemy.health = 5; enemy.visible = true;
    CThing marker; marker.health = 0; marker.visible = false;
    CState s; s.creature = &me; s.enemyOfSource = &enemy; s.arenaMarker = &marker; s.lastName = 0;
    s.combat.creature = 0; s.combat.enemy = 0; s.combat.flag = false; s.combat.calls = 0;
    s.PArenaCentre = 0; s.PEnemy = 0; s.HealthAtLastTailStrike = 0;
    bool ok = s.IsRunnable() && s.PArenaCentre == &marker && s.PEnemy == &enemy && s.HealthAtLastTailStrike == 77
        && s.combat.calls == 1 && s.combat.enemy == &enemy && s.combat.flag && std::strcmp(s.lastName, "ScorpionArenaCentre") == 0;
    s.arenaMarker = 0; enemy.visible = false; ok = ok && !s.IsRunnable() && s.PArenaCentre == 0 && s.PEnemy == 0 && s.combat.calls == 1;
    s.PEnemy = &marker; s.enemyOfSource = 0; ok = ok && !s.IsRunnable() && s.PEnemy == &marker;
    if (!ok) { std::printf("0097b900_TEST FAIL\n"); return 1; }
    std::printf("0097b900_TEST PASS\n");
    return 0;
}