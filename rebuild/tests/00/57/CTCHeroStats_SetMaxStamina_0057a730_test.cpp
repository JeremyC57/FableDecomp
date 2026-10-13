#include <cstdio>

struct S { unsigned char pad[0x58]; long Stamina; long MaxStamina; unsigned char p2[0x30]; long Extra;
    void SetMaxStamina(long m) { MaxStamina = m; if (MaxStamina < Extra + 1) MaxStamina = Extra + 1; if (Stamina > MaxStamina) Stamina = MaxStamina; } };
int main()
{
    S s; s.Stamina = 50; s.Extra = 10; s.SetMaxStamina(5);
    if (s.MaxStamina != 11 || s.Stamina != 11) { std::printf("0057a730_TEST FAIL\n"); return 1; }
    s.SetMaxStamina(40); if (s.MaxStamina != 40 || s.Stamina != 11) { std::printf("0057a730_TEST FAIL\n"); return 1; }
    std::printf("0057a730_TEST PASS\n");
    return 0;
}