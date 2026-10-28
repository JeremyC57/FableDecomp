// Test: CAIStateGroup_SellToBuyer::IsRunnable -- runnable when the creature has an interaction of type 8 with exactly two participants and a buyer's order exists
#include <cstdio>
class CCreatureInteraction { public: long Type; int participants; };
struct CCountedPointer { CCreatureInteraction* Data; };
static bool IsRunnable(const CCountedPointer& interaction, bool haveOrder)
{
    if (interaction.Data && interaction.Data->Type == 8 && interaction.Data->participants == 2 && haveOrder) return true;
    return false;
}
int main()
{
    CCreatureInteraction ok = { 8, 2 }, wrongType = { 7, 2 }, three = { 8, 3 };
    CCountedPointer p0 = { 0 }, p1 = { &ok }, p2 = { &wrongType }, p3 = { &three };
    if (!IsRunnable(p1, true) || IsRunnable(p1, false) || IsRunnable(p0, true) || IsRunnable(p2, true) || IsRunnable(p3, true)) { std::printf("0094c010_TEST FAIL\n"); return 1; }
    std::printf("0094c010_TEST PASS\n");
    return 0;
}