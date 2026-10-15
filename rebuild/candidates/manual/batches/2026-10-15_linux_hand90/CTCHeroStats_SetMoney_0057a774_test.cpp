#include <cstdio>
// Behaviour of CTCHeroStats::SetMoney (0x0057A774): with counting on, rises book to TotalMoneyEarned and falls to
// TotalMoneySpent; MaxMoney becomes the high-water mark; a negative purse is floored at 0.
template <class T> inline const T& Max(const T& left, const T& right) { return left < right ? right : left; }
static inline void ClampMin(long& value, long minimum) { if (value < minimum) value = minimum; }
struct CTCHeroStats_Retail {
    long Money, MaxMoney, ConfiscatedMoney, TotalMoneyEarned, TotalMoneySpent;
    void SetMoney(long amount, bool countTransaction)
    {
        if (countTransaction) {
            if (amount > Money)
                TotalMoneyEarned += amount - Money;
            else if (amount < Money)
                TotalMoneySpent += Money - amount;
        }
        Money = amount;
        MaxMoney = Max(MaxMoney, Money);
        ClampMin(Money, 0);
    }
};
int main()
{
    CTCHeroStats_Retail s; s.Money = 100; s.MaxMoney = 150; s.ConfiscatedMoney = 0; s.TotalMoneyEarned = 0; s.TotalMoneySpent = 0;
    s.SetMoney(250, true);
    if (s.Money != 250 || s.MaxMoney != 250 || s.TotalMoneyEarned != 150 || s.TotalMoneySpent != 0) { std::printf("0057a774_TEST FAIL\n"); return 1; }
    s.SetMoney(40, true);
    if (s.Money != 40 || s.MaxMoney != 250 || s.TotalMoneySpent != 210) { std::printf("0057a774_TEST FAIL\n"); return 1; }
    s.SetMoney(500, false);
    if (s.Money != 500 || s.MaxMoney != 500 || s.TotalMoneyEarned != 150) { std::printf("0057a774_TEST FAIL\n"); return 1; }
    s.SetMoney(-20, false);
    if (s.Money != 0 || s.MaxMoney != 500) { std::printf("0057a774_TEST FAIL\n"); return 1; }
    std::printf("0057a774_TEST PASS\n");
    return 0;
}
