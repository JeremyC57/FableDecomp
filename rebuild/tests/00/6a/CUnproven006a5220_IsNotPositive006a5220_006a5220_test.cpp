// Behaviour test for CUnproven_006a5220::IsNotPositive_006a5220 (0x006A5220): 1 when the signed value is <= 0.
#include <cstdio>
class CUnproven_006a5220 { public:
    int Value;
    int IsNotPositive_006a5220() const { return Value <= 0; }
};
int main()
{
    CUnproven_006a5220 u;
    int vals[] = { -5, 0, 1, 2147483647, -2147483647 - 1 };
    int want[] = { 1, 1, 0, 0, 1 };
    for (int i = 0; i < 5; ++i) { u.Value = vals[i]; if (u.IsNotPositive_006a5220() != want[i]) { std::printf("006a5220_TEST FAIL\n"); return 1; } }
    std::printf("006a5220_TEST PASS\n");
    return 0;
}