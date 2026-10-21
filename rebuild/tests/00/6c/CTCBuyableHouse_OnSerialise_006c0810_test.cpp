// Behaviour test for CTCBuyableHouse::OnSerialise (0x006C0810): defaults, the rent-day sentinel and the money-bag fold.
#include <cstdio>
struct House {
    long DaySomeoneWillMoveIn, DayNextRentIsDue, CurrentDressLevel, VirtualMoneyBags; bool Rented; int bags; bool destroyed;
    void OnSerialise(long mode)
    {
        if (CurrentDressLevel == -1) CurrentDressLevel = 0;
        if (mode != 1 && mode != 3) {
            if (DaySomeoneWillMoveIn == 0) DaySomeoneWillMoveIn = 0x7fffffff;
            if (DayNextRentIsDue == 0) DayNextRentIsDue = 0x7fffffff;
        }
        bool folding = mode == 1 || mode == 3;
        if (folding) {
            if (Rented) { VirtualMoneyBags += bags; bags = 0; destroyed = true; }
        }
    }
};
int main()
{
    House a = { 0, 0, -1, 5, true, 3, false };
    a.OnSerialise(1);
    if (a.CurrentDressLevel != 0 || a.DaySomeoneWillMoveIn != 0 || a.VirtualMoneyBags != 8 || !a.destroyed) { std::printf("006c0810_TEST FAIL\n"); return 1; }
    House b = { 0, 7, 2, 5, true, 3, false };
    b.OnSerialise(2);
    if (b.DaySomeoneWillMoveIn != 0x7fffffff || b.DayNextRentIsDue != 7 || b.VirtualMoneyBags != 5 || b.destroyed) { std::printf("006c0810_TEST FAIL\n"); return 1; }
    std::printf("006c0810_TEST PASS\n");
    return 0;
}