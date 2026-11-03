#include <cstdio>
// Model of CPlayer::PeekPControlledCreature: the const item of the ControlledCreature intelligent pointer (+0x2c), null when unset.
struct Object { int id; };
struct IntelligentPointer { Object* item; Object* GetPItem() const { return item; } };
struct Player { IntelligentPointer ControlledCreature, PlayerCharacter;
    const Object* PeekPControlledCreature() const { return ControlledCreature.GetPItem(); } };
int main()
{
    Object hero = { 7 }; Player p; p.ControlledCreature.item = &hero; p.PlayerCharacter.item = 0;
    if (p.PeekPControlledCreature() != &hero) { std::printf("00487dd0_TEST FAIL\n"); return 1; }
    p.ControlledCreature.item = 0;
    if (p.PeekPControlledCreature() != 0) { std::printf("00487dd0_TEST FAIL\n"); return 1; }
    std::printf("00487dd0_TEST PASS\n"); return 0;
}