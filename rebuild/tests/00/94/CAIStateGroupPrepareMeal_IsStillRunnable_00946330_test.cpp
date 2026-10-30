// behaviour test for 0x00946330 CAIStateGroup_PrepareMeal::IsStillRunnable
#include <cstdio>
class CThing { public: unsigned char Dead : 1; bool IsDead() const { return Dead; } };
struct Group {
    CThing* home; bool mealTime; CThing* pot;
    bool IsStillRunnable() { return home && mealTime && pot && !pot->IsDead(); }
};
int main()
{
    CThing house = { 0 }, pot = { 0 }, deadPot = { 1 };
    Group a = { &house, true, &pot }, b = { 0, true, &pot }, c = { &house, false, &pot }, d = { &house, true, 0 }, e = { &house, true, &deadPot };
    bool ok = a.IsStillRunnable() && !b.IsStillRunnable() && !c.IsStillRunnable() && !d.IsStillRunnable() && !e.IsStillRunnable();
    std::printf(ok ? "00946330_TEST PASS\n" : "00946330_TEST FAIL\n");
    return ok ? 0 : 1;
}