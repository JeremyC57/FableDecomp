#include <cstdio>
// Model of CPlayer::GetPPlayerCharacterThing: the item of the PlayerCharacter intelligent pointer (+0x34), null when unset.
struct Object { int id; };
struct IntelligentPointer { Object* item; Object* GetPItem() const { return item; } };
struct Player { IntelligentPointer ControlledCreature, PlayerCharacter;
    Object* GetPPlayerCharacterThing() const { return PlayerCharacter.GetPItem(); } };
int main()
{
    Object hero = { 7 }; Player p; p.PlayerCharacter.item = &hero; p.ControlledCreature.item = 0;
    if (p.GetPPlayerCharacterThing() != &hero) { std::printf("00487b70_TEST FAIL\n"); return 1; }
    p.PlayerCharacter.item = 0;
    if (p.GetPPlayerCharacterThing() != 0) { std::printf("00487b70_TEST FAIL\n"); return 1; }
    std::printf("00487b70_TEST PASS\n"); return 0;
}
