#include <cstdio>

struct H { unsigned char pad[0x3c]; long Money; long x; long Conf;
    void SetMoney(long a, bool) { Money = a; } void AddMoney(long a, bool) { Money += a; }
    void Set(bool c) { if (c) { Conf += Money; SetMoney(0, false); } else { AddMoney(Conf, false); Conf = 0; } } };
int main()
{
    H h; h.Money = 100; h.Conf = 5; h.Set(true); if (h.Money != 0 || h.Conf != 105) { std::printf("0057c999_TEST FAIL\n"); return 1; }
    h.Set(false); if (h.Money != 105 || h.Conf != 0) { std::printf("0057c999_TEST FAIL\n"); return 1; }
    std::printf("0057c999_TEST PASS\n");
    return 0;
}
