// Behaviour test for 0x00677620: AsymmetricPyramid ctor stores axes/centres and inverse half-widths.
#include <cstdio>
struct M { long ReactionType, XAxisOpinionType, YAxisOpinionType; float XCentre, NegInvXRadius, PosInvXRadius, YCentre, NegInvYRadius, PosInvYRadius;
  M(long t, long xa, long ya, float xMin, float xC, float xMax, float yMin, float yC, float yMax)
    : ReactionType(t), XAxisOpinionType(xa), YAxisOpinionType(ya), XCentre(xC), NegInvXRadius(1.0f / (xC - xMin)), PosInvXRadius(1.0f / (xMax - xC)),
      YCentre(yC), NegInvYRadius(1.0f / (yC - yMin)), PosInvYRadius(1.0f / (yMax - yC)) {} };
int main()
{
    M m(3, 1, 2, -2.0f, 0.0f, 4.0f, 1.0f, 2.0f, 2.5f);
    bool ok = m.ReactionType == 3 && m.XAxisOpinionType == 1 && m.YAxisOpinionType == 2 && m.XCentre == 0.0f && m.NegInvXRadius == 0.5f
           && m.PosInvXRadius == 0.25f && m.YCentre == 2.0f && m.NegInvYRadius == 1.0f && m.PosInvYRadius == 2.0f;
    if (!ok) { std::printf("00677620_TEST FAIL\n"); return 1; }
    std::printf("00677620_TEST PASS\n");
    return 0;
}