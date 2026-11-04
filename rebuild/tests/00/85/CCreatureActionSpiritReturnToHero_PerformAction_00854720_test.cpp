// Behaviour test for CCreatureAction_SpiritReturnToHero::PerformAction (0x00854720): a model of the row -- the spirit's physics
// position is reset to the controlled hero creature's position, or to its own position when no creature is controlled.
#include <stdio.h>
struct V { float X, Y, Z; };
struct Physics { V Reset; int Calls; void ResetPosition(const V& p) { Reset = p; ++Calls; } };
struct Creature { V Pos; Physics* PhysicsTC; const V& GetPos() const { return Pos; } };
struct Player { Creature* Controlled; Creature* GetPControlledCreature() const { return Controlled; } };
struct Action { Creature* PCreature; Player* MainPlayer;
    void PerformAction()
    {
        Player& player = *MainPlayer;
        V target;
        if (player.GetPControlledCreature())
            target = player.GetPControlledCreature()->GetPos();
        else
            target = PCreature->GetPos();
        PCreature->PhysicsTC->ResetPosition(target);
    }
};
int main()
{
    Physics phys = { { 0, 0, 0 }, 0 };
    Creature spirit = { { 1, 2, 3 }, &phys }, hero = { { 7, 8, 9 }, 0 };
    Player player = { &hero };
    Action a = { &spirit, &player };
    a.PerformAction();
    int ok = phys.Calls == 1 && phys.Reset.X == 7 && phys.Reset.Z == 9;
    player.Controlled = 0; a.PerformAction();
    ok = ok && phys.Calls == 2 && phys.Reset.X == 1 && phys.Reset.Y == 2;
    printf(ok ? "00854720_TEST PASS\n" : "00854720_TEST FAIL\n");
    return ok ? 0 : 1;
}