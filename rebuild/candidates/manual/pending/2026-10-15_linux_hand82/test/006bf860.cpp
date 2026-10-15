// Behaviour test for 006bf860 CTCBuyableHouse::MovedInNotification: the next rent day becomes min(current, day + 1).
#include <cstdio>

template <class T>
inline const T& DinkumMin(const T& _Left, const T& _Right) { return _Right < _Left ? _Right : _Left; }

struct CTCBuyableHouse
{
    long DayNextRentIsDue;
    void MovedInNotification(long day)
    {
        ++day;
        DayNextRentIsDue = DinkumMin(DayNextRentIsDue, day);
    }
};

int main()
{
    CTCBuyableHouse house; house.DayNextRentIsDue = 30;
    house.MovedInNotification(10);
    if (house.DayNextRentIsDue != 11) { std::printf("006bf860_TEST FAIL\n"); return 1; }
    house.MovedInNotification(40);
    if (house.DayNextRentIsDue != 11) { std::printf("006bf860_TEST FAIL\n"); return 1; }
    house.MovedInNotification(10);
    if (house.DayNextRentIsDue != 11) { std::printf("006bf860_TEST FAIL\n"); return 1; }
    std::printf("006bf860_TEST PASS\n");
    return 0;
}
