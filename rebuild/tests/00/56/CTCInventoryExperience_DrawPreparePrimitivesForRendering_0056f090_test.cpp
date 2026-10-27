// behaviour test for 0x0056F090: the backdrop fraction is accumulated time / fade time, clamped to 1
#include <cstdio>
static float Fraction(float& accum, float delta, float fadeTime) { accum += delta; float a = accum / fadeTime; if (a > 1.0f) a = 1.0f; return a; }
int main()
{
    float accum = 0; float a = Fraction(accum, 0.5f, 2.0f), b = Fraction(accum, 1.0f, 2.0f), c = Fraction(accum, 1.0f, 2.0f);
    if (a != 0.25f || b != 0.75f || c != 1.0f) { std::printf("0056f090_TEST FAIL\n"); return 1; }
    std::printf("0056f090_TEST PASS\n");
    return 0;
}