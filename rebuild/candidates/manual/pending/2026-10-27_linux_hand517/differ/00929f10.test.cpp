// Test: CAIStateGroup_ApprenticeHero::IsRunnable -- needs the village-member TC (0x23), a village, and the village's cook time
#include <cstdio>
struct CTCVillage { bool cookTime; bool IsCookTime() const { return cookTime; } };
struct CTCVillageMember { CTCVillage* village; CTCVillage* GetPVillage() const { return village; } };
struct CCreature { bool hasMember; CTCVillageMember member; };
static bool IsRunnable(const CCreature* c)
{
    if (!c->hasMember) return false;
    CTCVillage* village = c->member.GetPVillage();
    if (village == 0) return false;
    if (village->IsCookTime()) return true;
    return false;
}
int main()
{
    CTCVillage cook = { true }, idle = { false };
    CCreature a = { true, { &cook } }, b = { true, { &idle } }, c = { true, { 0 } }, d = { false, { &cook } };
    if (!IsRunnable(&a) || IsRunnable(&b) || IsRunnable(&c) || IsRunnable(&d)) { std::printf("00929f10_TEST FAIL\n"); return 1; }
    std::printf("00929f10_TEST PASS\n");
    return 0;
}
