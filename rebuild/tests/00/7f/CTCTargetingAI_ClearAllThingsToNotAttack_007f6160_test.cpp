// Behaviour test for CTCTargetingAI::ClearAllThingsToNotAttack 0x007F6160: the do-not-attack list is emptied. The body forwards to the member at +0x48
// (`add ecx,48h ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
#include <list>
struct CThing { int id; };
#pragma pack(push, 4)
struct CTCTargetingAI {                                  // retail is 32-bit: 4-byte packing keeps the +0x48 offset on a 64-bit host
    unsigned char Before[0x48];
    std::list<CThing*> ThingsToNotAttack;
    void ClearAllThingsToNotAttack() { ThingsToNotAttack.clear(); }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CTCTargetingAI, ThingsToNotAttack) != 0x48u) { std::printf("007f6160_TEST FAIL\n"); return 1; }
    CThing a1 = { 1 }, a2 = { 2 };
    CTCTargetingAI t; t.ThingsToNotAttack.push_back(&a1); t.ThingsToNotAttack.push_back(&a2);
    t.ClearAllThingsToNotAttack();
    if (!t.ThingsToNotAttack.empty()) { std::printf("004c7bc0_TEST FAIL\n"); return 1; }
    std::printf("007f6160_TEST PASS\n");
    return 0;
}