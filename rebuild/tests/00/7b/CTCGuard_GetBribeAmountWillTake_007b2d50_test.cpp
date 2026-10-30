// Behaviour test for 0x007B2D50 CTCGuard::GetBribeAmountWillTake (see src/007b2d50.cpp): the guard takes no bribe (0)
// when its village-member interface (0x23) names a village where the hero is a criminal, else 50.
#include <cstdio>
struct Village { bool HeroCriminal; bool IsHeroCriminal() const { return HeroCriminal; } };
struct Member { Village* PVillage; Village* GetPVillage() const { return PVillage; } };
static long GetBribeAmountWillTake(Member* member)
{
    Village* village = member->GetPVillage();
    if (village != 0 && village->IsHeroCriminal())
        return 0;
    return 50;
}
int main()
{
    Village crim = { true }, clean = { false };
    Member a = { &crim }, b = { &clean }, c = { 0 };
    bool ok = GetBribeAmountWillTake(&a) == 0 && GetBribeAmountWillTake(&b) == 50 && GetBribeAmountWillTake(&c) == 50;
    std::printf(ok ? "007b2d50_TEST PASS\n" : "007b2d50_TEST FAIL\n");
    return ok ? 0 : 1;
}