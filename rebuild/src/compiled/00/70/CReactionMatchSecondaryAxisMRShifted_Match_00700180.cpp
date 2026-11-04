// CReactionMatch_SecondaryAxisMRShifted::Match(const COpinionSet& opinions) -- position along the secondary axis plus the M/R opinion shifts, clamped to [0, 1]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00700180, length 118 bytes (virtual). No callees.
// Layout (Ego_r.pdb, engine/CReactionMatch_SecondaryAxisMRShifted.h): Axis +8 (EOpinion), LowerBoundOnAxis +0xc,
// InvIntervalOnAxis +0x10, MShiftZero +0x14, MShiftWeight +0x18, RShiftZero +0x1c, RShiftWeight +0x20 (polymorphic
// retail model, same names and size). COpinionSet (PDB) is one CArray<float> whose first word is the element pointer;
// opinion 0 drives the M shift and opinion 1 the R shift.
// Shape: one sum expression passed straight to the clamp (its temporary is stored with `fst` into the dead
// argument slot and compared without a reload); the clamp is through an
// inline reference Clamp(v, lo, hi) whose 0.0f/1.0f temporaries live in the frame.
// Leaf: Match@CReactionMatch_SecondaryAxisMRShifted
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
#include "engine/CReactionMatch_SecondaryAxisMRShifted.h"

#include "engine/CArray.h"

class COpinionSet { public:
    CArray<float> OpinionValues;                        // +0x00
};

struct CReactionMatch_SecondaryAxisMRShiftedRetail {
    virtual void v00();
    long  ReactionType;                                 // +0x04 EOpinionReactionType
    long  Axis;                                         // +0x08 EOpinion
    float LowerBoundOnAxis;                             // +0x0c
    float InvIntervalOnAxis;                            // +0x10
    float MShiftZero;                                   // +0x14
    float MShiftWeight;                                 // +0x18
    float RShiftZero;                                   // +0x1c
    float RShiftWeight;                                 // +0x20
    float Match(const COpinionSet& opinions);
};
FABLE_STATIC_ASSERT(sizeof(CReactionMatch_SecondaryAxisMRShiftedRetail) == sizeof(CReactionMatch_SecondaryAxisMRShifted));

inline const float& Clamp(const float& v, const float& lo, const float& hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

float CReactionMatch_SecondaryAxisMRShiftedRetail::Match(const COpinionSet& opinions)
{
    return Clamp((opinions.OpinionValues[Axis] - LowerBoundOnAxis) * InvIntervalOnAxis
            + (opinions.OpinionValues[0] - MShiftZero) * MShiftWeight
            + (opinions.OpinionValues[1] - RShiftZero) * RShiftWeight, 0.0f, 1.0f);
}