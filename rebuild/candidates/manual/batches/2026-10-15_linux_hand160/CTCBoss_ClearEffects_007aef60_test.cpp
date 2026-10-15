// Behaviour test for CTCBoss::ClearEffects (0x007AEF60): every live effect is killed; null and dead ones are skipped.
#include <cstdio>
struct Thing { unsigned char Flags91; int kills; bool IsDead() const { return (Flags91 & 1) != 0; } void Kill(bool now) { if (now) ++kills; } };
struct IPtr { Thing* item; Thing* GetPItem() const { return item; } };
static bool IsThingAlive(const IPtr& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }
struct Boss {
    IPtr* first; IPtr* last;
    unsigned int size() const { return (unsigned int)(last - first); }
    void ClearEffects() { for (unsigned int i = 0; i < size(); ++i) if (IsThingAlive(*(first + i))) (first + i)->GetPItem()->Kill(true); }
};
int main()
{
    Thing a, b; a.Flags91 = 0; a.kills = 0; b.Flags91 = 1; b.kills = 0;
    IPtr fx[3]; fx[0].item = &a; fx[1].item = 0; fx[2].item = &b;
    Boss boss; boss.first = fx; boss.last = fx + 3;
    boss.ClearEffects();
    if (a.kills != 1 || b.kills != 0) { std::printf("007aef60_TEST FAIL\n"); return 1; }
    boss.last = fx; boss.ClearEffects();
    if (a.kills != 1) { std::printf("007aef60_TEST FAIL\n"); return 1; }
    std::printf("007aef60_TEST PASS\n");
    return 0;
}
