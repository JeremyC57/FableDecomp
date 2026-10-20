// Behaviour test for 0x00700400 CReactionMatch_Sphere::Match
#include <cstdio>

struct CArrayFloat { float* m_begin; float* m_end; float* m_capacity; };
struct COpinionSet {
    CArrayFloat OpinionValues;
    float GetSqDistance(const COpinionSet& other) const
    {
        float sqDistance = 0.0f;
        for (long i = 0; i < 5; ++i) {
            float d = OpinionValues.m_begin[i] - other.OpinionValues.m_begin[i];
            sqDistance += d * d;
        }
        return sqDistance;
    }
};
static const float& Clamp(const float& value, const float& lo, const float& hi)
{
    if (value < lo) return lo;
    if (value > hi) return hi;
    return value;
}
struct CReactionMatch_Sphere {
    COpinionSet Centre;
    float SqRadius;
    float Match(const COpinionSet& opinions)
    {
        return Clamp(1.0f - opinions.GetSqDistance(Centre) / SqRadius, 0.0f, 1.0f);
    }
};

static COpinionSet Make(float* v) { COpinionSet s; s.OpinionValues.m_begin = v; s.OpinionValues.m_end = v + 5; s.OpinionValues.m_capacity = v + 5; return s; }

int main()
{
    float c[5] = { 0, 0, 0, 0, 0 };
    float near_[5] = { 1, 0, 0, 0, 1 };      // sq distance 2
    float far_[5] = { 3, 0, 0, 0, 0 };       // sq distance 9
    float same[5] = { 0, 0, 0, 0, 0 };
    CReactionMatch_Sphere m; m.Centre = Make(c); m.SqRadius = 4.0f;
    float r1 = m.Match(Make(near_));
    float r2 = m.Match(Make(far_));
    float r3 = m.Match(Make(same));
    float e1 = 0.5f, e2 = 0.0f, e3 = 1.0f;
    if (r1 != e1 || r2 != e2 || r3 != e3) { std::printf("00700400_TEST FAIL\n"); return 1; }
    m.SqRadius = -1.0f;                      // negative radius: 1 + d^2 > 1 clamps to the upper bound
    float r4 = m.Match(Make(near_));
    if (r4 != e3) { std::printf("00700400_TEST FAIL\n"); return 1; }
    std::printf("00700400_TEST PASS\n");
    return 0;
}
