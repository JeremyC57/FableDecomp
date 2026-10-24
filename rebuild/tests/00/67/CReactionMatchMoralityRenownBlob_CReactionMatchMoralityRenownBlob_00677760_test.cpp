// Behaviour test for 0x00677760 CReactionMatch_MoralityRenownBlob::CReactionMatch_MoralityRenownBlob: the pyramid gets
// axes 0/1, the x range from the 4th/5th arguments and the y range from the 2nd/3rd, centres midway, inverse half-widths
// from the base ctor (modelled from the landed 0x00677620), and the three shifts land at +0x28/+0x2c/+0x30.
#include <cstdio>

enum EOpinionReactionType { OPINION_REACTION_TYPE_NONE = -1, OPINION_REACTION_TYPE_0 = 0 };

class CReactionMatch {
public:
    virtual ~CReactionMatch() {}
    int ReactionType;
};

class CReactionMatch_AsymmetricPyramidRetail : public CReactionMatch {
public:
    int   XAxisOpinionType, YAxisOpinionType;
    float XCentre, NegInvXRadius, PosInvXRadius, YCentre, NegInvYRadius, PosInvYRadius;
    CReactionMatch_AsymmetricPyramidRetail(EOpinionReactionType type, int xAxis, int yAxis, float xMin, float xCentre,
                                           float xMax, float yMin, float yCentre, float yMax)   // models 0x00677620
    {
        ReactionType = type; XAxisOpinionType = xAxis; YAxisOpinionType = yAxis;
        XCentre = xCentre; NegInvXRadius = 1.0f / (xCentre - xMin); PosInvXRadius = 1.0f / (xMax - xCentre);
        YCentre = yCentre; NegInvYRadius = 1.0f / (yCentre - yMin); PosInvYRadius = 1.0f / (yMax - yCentre);
    }
};

class CReactionMatch_MRPyramidRetail : public CReactionMatch_AsymmetricPyramidRetail {
public:
    float ScarinessShift, AgreeablenessShift, AttractivenessShift;
    CReactionMatch_MRPyramidRetail(EOpinionReactionType type, float xMin, float xCentre, float xMax, float yMin,
                                   float yCentre, float yMax, float s, float a, float t)
        : CReactionMatch_AsymmetricPyramidRetail(type, 0, 1, xMin, xCentre, xMax, yMin, yCentre, yMax),
          ScarinessShift(s), AgreeablenessShift(a), AttractivenessShift(t) {}
};

class CReactionMatch_MoralityRenownBlob : public CReactionMatch_MRPyramidRetail {
public:
    CReactionMatch_MoralityRenownBlob(EOpinionReactionType type, float yMin, float yMax, float xMin, float xMax,
                                      float s, float a, float t)
        : CReactionMatch_MRPyramidRetail(type, xMin, (xMax - xMin) * 0.5f + xMin, xMax, yMin, (yMax - yMin) * 0.5f + yMin,
                                         yMax, s, a, t) {}
};

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    CReactionMatch_MoralityRenownBlob b((EOpinionReactionType)3, -1.0f, 3.0f, 2.0f, 6.0f, 0.25f, 0.5f, 0.75f);
    if (b.ReactionType != 3 || b.XAxisOpinionType != 0 || b.YAxisOpinionType != 1 || !Near(b.XCentre, 4.0f) ||
        !Near(b.YCentre, 1.0f) || !Near(b.NegInvXRadius, 0.5f) || !Near(b.PosInvXRadius, 0.5f) ||
        !Near(b.NegInvYRadius, 0.5f) || !Near(b.PosInvYRadius, 0.5f) || !Near(b.ScarinessShift, 0.25f) ||
        !Near(b.AgreeablenessShift, 0.5f) || !Near(b.AttractivenessShift, 0.75f)) {
        std::printf("00677760_TEST FAIL\n"); return 1;
    }
    std::printf("00677760_TEST PASS\n");
    return 0;
}