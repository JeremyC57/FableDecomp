// CReactionMatch_MRPyramid::Match(const COpinionSet& opinions) -- the asymmetric-pyramid match plus the three MR opinion shifts, clamped to [0, 1]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00700320, length 122 bytes (virtual).
// Callees: CReactionMatch_AsymmetricPyramid::Match @ 0x007002A0 (the base override, called non-virtually; its real
// body is reconstructed above under `auto_inline(off)` because retail keeps `this` in ecx across the call, which
// VC7.1 only does when it has compiled the callee earlier in the same TU).
// Layout (Ego_r.pdb, engine/CReactionMatch_MRPyramid.h): the AsymmetricPyramid base +0x00..+0x24, ScarinessShift
// +0x28, AgreeablenessShift +0x2c, AttractivenessShift +0x30; opinion values 2, 3, 4 are scaled by them.
// Shape: the result is a named float local (in the dead argument slot) clamped through an inline reference
// Clamp(v, lo, hi) whose 0.0f/1.0f temporaries live in the frame (`lea eax,[esp] ; fld [eax]`).
// Leaf: Match@CReactionMatch_MRPyramid
#include "engine/CReactionMatch_AsymmetricPyramid.h"
#include "engine/CReactionMatch_MRPyramid.h"

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

#pragma auto_inline(off)
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
#pragma auto_inline(on)

struct CReactionMatch_MRPyramidRetail : CReactionMatch_AsymmetricPyramidRetail {
    float ScarinessShift;                               // +0x28
    float AgreeablenessShift;                           // +0x2c
    float AttractivenessShift;                          // +0x30
    float Match(const COpinionSet& opinions);
};
FABLE_STATIC_ASSERT(sizeof(CReactionMatch_MRPyramidRetail) == sizeof(CReactionMatch_MRPyramid));

inline const float& Clamp(const float& v, const float& lo, const float& hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

float CReactionMatch_MRPyramidRetail::Match(const COpinionSet& opinions)
{
    float r = CReactionMatch_AsymmetricPyramidRetail::Match(opinions);
    r += opinions.OpinionValues[2] * ScarinessShift;
    r += opinions.OpinionValues[3] * AgreeablenessShift;
    r += opinions.OpinionValues[4] * AttractivenessShift;
    return Clamp(r, 0.0f, 1.0f);
}