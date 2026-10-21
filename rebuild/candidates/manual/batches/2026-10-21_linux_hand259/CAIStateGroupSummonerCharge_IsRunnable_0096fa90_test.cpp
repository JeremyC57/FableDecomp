// Behaviour test for CAIStateGroup_SummonerCharge::IsRunnable (0x0096FA90): same logic over mocks.
#include <cstdio>
struct CThing { bool dead; float x, y, z; bool hasStealth; long runAwayTimer; bool isCreature; bool fighting; bool chargeable; };
static CThing Make(float x, float y)
{
    CThing t; t.dead = false; t.x = x; t.y = y; t.z = 100.0f; t.hasStealth = false; t.runAwayTimer = -1;
    t.isCreature = true; t.fighting = true; t.chargeable = true; return t;
}
struct CState {
    long frame; long NextFrameToCharge; bool hasCarrying; CThing* weapon; bool hasSource; CThing* enemy; CThing* me;
    float nearRange, farRange; CThing* PTarget;
    bool IsRunnable()
    {
        if (frame < NextFrameToCharge) return false;
        if (!hasCarrying) return false;
        if (!(weapon && !weapon->dead)) return false;
        if (hasSource) {
            CThing* e = enemy;
            if (e && !e->dead) {
                float dx = e->x - me->x, dy = e->y - me->y;
                float d = dx * dx + dy * dy;
                if (d > nearRange * nearRange && d < farRange * farRange) {
                    if (e->hasStealth && e->runAwayTimer >= 0) { PTarget = e; return true; }
                    if (e->isCreature && e->fighting && e->chargeable) { PTarget = e; return true; }
                }
            }
        }
        return false;
    }
};
int main()
{
    CThing me = Make(0, 0), sword = Make(0, 0), enemy = Make(3, 4);
    CState s; s.frame = 10; s.NextFrameToCharge = 10; s.hasCarrying = true; s.weapon = &sword; s.hasSource = true;
    s.enemy = &enemy; s.me = &me; s.nearRange = 2.0f; s.farRange = 6.0f; s.PTarget = 0;
    bool ok = s.IsRunnable() && s.PTarget == &enemy;
    s.PTarget = 0; enemy.chargeable = false; ok = ok && !s.IsRunnable() && s.PTarget == 0;
    enemy.hasStealth = true; enemy.runAwayTimer = 0; enemy.isCreature = false; ok = ok && s.IsRunnable() && s.PTarget == &enemy;
    s.PTarget = 0; s.farRange = 5.0f; ok = ok && !s.IsRunnable();
    s.farRange = 6.0f; s.nearRange = 5.0f; ok = ok && !s.IsRunnable();
    s.nearRange = 2.0f; sword.dead = true; ok = ok && !s.IsRunnable();
    sword.dead = false; s.NextFrameToCharge = 11; ok = ok && !s.IsRunnable() && s.PTarget == 0;
    if (!ok) { std::printf("0096fa90_TEST FAIL\n"); return 1; }
    std::printf("0096fa90_TEST PASS\n");
    return 0;
}
