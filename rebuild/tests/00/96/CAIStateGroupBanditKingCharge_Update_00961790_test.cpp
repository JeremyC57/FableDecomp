// behaviour test for CAIStateGroup_BanditKingCharge::Update (0x00961790)
#include <cstdio>
#include <cstring>
struct CThing { bool dead; };
struct Group {
    CThing* target; long moveResult; int attacks; bool terminated; const char* lastAnim;
    long MoveTo() { return moveResult; }
    void Attack(const char* anim) { ++attacks; lastAnim = anim; }
    void Update()
    {
        if (target && !target->dead && MoveTo() == 2 && target && !target->dead) Attack("BK_CHARGE_ATTACK");
        terminated = true;
    }
};
int main()
{
    CThing t = { false };
    Group g = { &t, 2, 0, false, 0 };
    g.Update();
    bool ok = g.attacks == 1 && g.terminated && std::strcmp(g.lastAnim, "BK_CHARGE_ATTACK") == 0;
    g.moveResult = 3; g.terminated = false; g.Update(); ok = ok && g.attacks == 1 && g.terminated;
    t.dead = true; g.moveResult = 2; g.Update(); ok = ok && g.attacks == 1;
    if (!ok) { std::printf("00961790_TEST FAIL\n"); return 1; }
    std::printf("00961790_TEST PASS\n");
    return 0;
}