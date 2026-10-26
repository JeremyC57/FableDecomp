// CReactionMatch_AsymmetricPyramid::Match(const COpinionSet& opinions) -- 1 minus the larger of the two scaled axis distances from the pyramid centre
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007002A0, length 123 bytes (vtable 0x0125B718 slot 1). No callees.
// Layout (Ego_r.pdb, engine/CReactionMatch_AsymmetricPyramid.h): XAxisOpinionType +8, YAxisOpinionType +0xc,
// XCentre +0x10, NegInvXRadius +0x14, PosInvXRadius +0x18, YCentre +0x1c, NegInvYRadius +0x20, PosInvYRadius +0x24.
// COpinionSet is one CArray<float> OpinionValues whose first word is the element pointer (retail 12 bytes).
// Shape: each axis distance picks the negative-side or positive-side inverse radius by `value < centre`
// (`test ah,5 ; jp`); the element address from the inline `operator[]` is kept in edx and re-read in both arms.
// The X distance is a named float local (spilled into the dead argument slot), then `dx > dy` picks the arm.
// Leaf: Match@CReactionMatch_AsymmetricPyramid
#include "engine/CReactionMatch_AsymmetricPyramid.h"

template <class T> class CArray { public:                      // retail CArray<T>: begin/end/capacity (0x0c; PDB says 0x10)
    T*   Data;                                          // +0x00
    T*   m_end;                                         // +0x04
    T*   m_capacity;                                    // +0x08
    const T& operator[](long i) const { return Data[i]; }
};

class COpinionSet { public:
    CArray<float> OpinionValues;                        // +0x00
};

struct CReactionMatch_AsymmetricPyramidRetail {
    virtual void v00();
    long  ReactionType;                                 // +0x04
    long  XAxisOpinionType;                             // +0x08 EOpinion
    long  YAxisOpinionType;                             // +0x0c EOpinion
    float XCentre;                                      // +0x10
    float NegInvXRadius;                                // +0x14
    float PosInvXRadius;                                // +0x18
    float YCentre;                                      // +0x1c
    float NegInvYRadius;                                // +0x20
    float PosInvYRadius;                                // +0x24
    float Match(const COpinionSet& opinions);
};
FABLE_STATIC_ASSERT(sizeof(CReactionMatch_AsymmetricPyramidRetail) == sizeof(CReactionMatch_AsymmetricPyramid));

float CReactionMatch_AsymmetricPyramidRetail::Match(const COpinionSet& opinions)
{
    const float& x = opinions.OpinionValues[XAxisOpinionType];
    float dx;
    if (x < XCentre)
        dx = (XCentre - x) * NegInvXRadius;
    else
        dx = (x - XCentre) * PosInvXRadius;
    const float& y = opinions.OpinionValues[YAxisOpinionType];
    float dy;
    if (y < YCentre)
        dy = (YCentre - y) * NegInvYRadius;
    else
        dy = (y - YCentre) * PosInvYRadius;
    if (dx > dy)
        return 1.0f - dx;
    return 1.0f - dy;
}