#include <cstdio>
#include <cmath>
// Host model of the CGetBestTargetInDirection constructor: flattened unit line from the base towards the point.
struct Vec { float X, Y, Z; };
struct Functor {
    const Vec* BasePos; Vec TestLine; float Direction; void* PBest; float CosBest;
    Functor(const Vec* base, const Vec& towards, float dir) : BasePos(base), Direction(dir), PBest(0), CosBest(-2.0f)
    {
        TestLine.X = towards.X - base->X; TestLine.Y = towards.Y - base->Y; TestLine.Z = towards.Z - base->Z;
        TestLine.Z = 0.0f;
        float l2 = TestLine.X * TestLine.X + TestLine.Y * TestLine.Y + TestLine.Z * TestLine.Z;
        if (l2 > 0.0001f * 0.0001f) {
            float inv = 1.0f / std::sqrt(l2);
            TestLine.X *= inv; TestLine.Y *= inv; TestLine.Z *= inv;
        }
    }
};
int main()
{
    int fail = 0;
    Vec base = { 1.0f, 1.0f, 5.0f };
    Vec to = { 4.0f, 5.0f, 9.0f };
    Functor f(&base, to, 0.5f);
    if (f.BasePos != &base || f.Direction != 0.5f || f.PBest != 0 || f.CosBest != -2.0f) fail = 1;
    if (std::fabs(f.TestLine.X - 0.6f) > 1e-5f || std::fabs(f.TestLine.Y - 0.8f) > 1e-5f || f.TestLine.Z != 0.0f) fail = 1;
    Functor g(&base, base, 0.0f);
    if (g.TestLine.X != 0.0f || g.TestLine.Y != 0.0f) fail = 1;
    if (fail) { std::printf("008059f0_TEST FAIL\n"); return 1; }
    std::printf("008059f0_TEST PASS\n");
    return 0;
}