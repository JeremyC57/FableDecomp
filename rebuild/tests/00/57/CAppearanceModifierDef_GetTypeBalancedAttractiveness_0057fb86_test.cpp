// behaviour test for 0x0057FB86: with five scaling entries the value is scaled by the type's entry, else returned as is
#include <cstdio>
static float Balanced(const float* table, int n, int type, float value) { if (n == 5) return table[type] * value; return value; }
int main()
{
    float t[5] = { 1.0f, 2.0f, 0.5f, 3.0f, 4.0f };
    if (Balanced(t, 5, 2, 8.0f) != 4.0f || Balanced(t, 4, 2, 8.0f) != 8.0f) { std::printf("0057fb86_TEST FAIL\n"); return 1; }
    std::printf("0057fb86_TEST PASS\n");
    return 0;
}