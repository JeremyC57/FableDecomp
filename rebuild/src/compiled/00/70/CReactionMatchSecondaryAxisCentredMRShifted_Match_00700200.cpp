// CReactionMatch_SecondaryAxisCentredMRShifted::Match(const COpinionSet& opinions) -- closeness to the axis centre (floored at 0) plus the M/R opinion shifts, clamped to [0, 1]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00700200, length 153 bytes (virtual). No callees.
// Layout (Ego_r.pdb, engine/CReactionMatch_SecondaryAxisCentredMRShifted.h): Axis +8 (EOpinion), CentreOnAxis +0xc,
// RadiusOnAxis +0x10, MShiftZero +0x14, MShiftWeight +0x18, RShiftZero +0x1c, RShiftWeight +0x20 (polymorphic
// retail model, same names and size). COpinionSet (PDB) is one CArray<float> whose first word is the element
// pointer; opinion 0 drives the M shift and opinion 1 the R shift.
// Shape: the closeness `1 - |v - centre| / radius` stays on the FPU (`fcom` then `fstp st(0) ; fld 0` for the
// floor); the sum is a named float local (dead argument slot, `fstp ; fld`) clamped through an inline reference
// Clamp(v, lo, hi) whose 0.0f/1.0f temporaries live in the frame.
// Leaf: Match@CReactionMatch_SecondaryAxisCentredMRShifted
#include <math.h>
#include "engine/CReactionMatch_SecondaryAxisCentredMRShifted.h"

template <class T> class CArray { public:                      // retail CArray<T>: begin/end/capacity (0x0c; PDB says 0x10)
    T*   Data;                                          // +0x00
    T*   m_end;                                         // +0x04
    T*   m_capacity;                                    // +0x08
    const T& operator[](long i) const { return Data[i]; }
};

class COpinionSet { public:
    CArray<float> OpinionValues;                        // +0x00
};

struct CReactionMatch_SecondaryAxisCentredMRShiftedRetail {
    virtual void v00();
    long  ReactionType;                                 // +0x04 EOpinionReactionType
    long  Axis;                                         // +0x08 EOpinion
    float CentreOnAxis;                                 // +0x0c
    float RadiusOnAxis;                                 // +0x10
    float MShiftZero;                                   // +0x14
    float MShiftWeight;                                 // +0x18
    float RShiftZero;                                   // +0x1c
    float RShiftWeight;                                 // +0x20
    float Match(const COpinionSet& opinions);
};
FABLE_STATIC_ASSERT(sizeof(CReactionMatch_SecondaryAxisCentredMRShiftedRetail) == sizeof(CReactionMatch_SecondaryAxisCentredMRShifted));

inline const float& Clamp(const float& v, const float& lo, const float& hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

float CReactionMatch_SecondaryAxisCentredMRShiftedRetail::Match(const COpinionSet& opinions)
{
    float closeness = 1.0f - (float)fabs(opinions.OpinionValues[Axis] - CentreOnAxis) / RadiusOnAxis;
    if (closeness < 0.0f)
        closeness = 0.0f;
    float r = closeness + (opinions.OpinionValues[0] - MShiftZero) * MShiftWeight
                        + (opinions.OpinionValues[1] - RShiftZero) * RShiftWeight;
    return Clamp(r, 0.0f, 1.0f);
}