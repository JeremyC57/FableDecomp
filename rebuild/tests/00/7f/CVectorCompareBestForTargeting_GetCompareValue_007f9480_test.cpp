#include <cstdio>
#include <cmath>
// Behaviour test: CVectorCompare_BestForTargeting::GetCompareValue -- (exp(-d*SqrDistCompare) + factor*exp(-d*falloff)) * exp(-(1-dot)*AngleCompare).
#define CHECK(c) if (!(c)) { std::printf("007f9480_TEST FAIL\n"); return 1; }
struct CVectorCompare { float AngleCompare, SqrDistCompare, OverrideShortDistanceFactor, OverrideShortDistanceFalloff;
    float GetCompareValue(float sqrDist, float angleDot) const
    {
        return (float)((std::exp(-(sqrDist * SqrDistCompare)) + OverrideShortDistanceFactor * std::exp(-(sqrDist * OverrideShortDistanceFalloff)))
                       * std::exp(-((1.0f - angleDot) * AngleCompare)));
    }
};
static bool Near(float a, float b) { return a - b < 1e-5f && b - a < 1e-5f; }
int main()
{
    CVectorCompare c; c.AngleCompare = 2.0f; c.SqrDistCompare = 0.01f; c.OverrideShortDistanceFactor = 0.5f; c.OverrideShortDistanceFalloff = 1.0f;
    CHECK(Near(c.GetCompareValue(0.0f, 1.0f), 1.5f))
    CHECK(Near(c.GetCompareValue(100.0f, 1.0f), (float)(std::exp(-1.0) + 0.5 * std::exp(-100.0))))
    CHECK(Near(c.GetCompareValue(0.0f, 0.0f), (float)(1.5 * std::exp(-2.0))))
    CHECK(c.GetCompareValue(10.0f, 0.9f) > c.GetCompareValue(20.0f, 0.9f))
    std::printf("007f9480_TEST PASS\n");
    return 0;
}