// Behaviour test for 0x00578C8D std::__uninitialized_fill_n over 12-byte ExperienceSpendInfo records: n copies of x
// placement-constructed from first, returns first + n; n == 0 touches nothing.
#include <cstdio>
#include <new>

struct E { int currentSpend; int initialSpend; int levelUpSpend; };
static E* FillN(E* first, unsigned int n, const E& x)
{
    E* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) E(x);
    return cur;
}

int main()
{
    E buf[5] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 9, 9, 9 } };
    E x = { 3, 30, -3 };
    E* r = FillN(buf, 4, x);
    bool ok = r == buf + 4 && buf[4].currentSpend == 9;
    for (int i = 0; i < 4; ++i) ok = ok && buf[i].currentSpend == 3 && buf[i].initialSpend == 30 && buf[i].levelUpSpend == -3;
    ok = ok && FillN(buf + 4, 0, x) == buf + 4 && buf[4].initialSpend == 9;
    std::printf(ok ? "00578c8d_TEST PASS\n" : "00578c8d_TEST FAIL\n");
    return ok ? 0 : 1;
}