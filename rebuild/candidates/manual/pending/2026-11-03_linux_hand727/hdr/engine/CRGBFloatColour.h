#ifndef FABLE_ENGINE_CRGBFLOATCOLOUR_H
#define FABLE_ENGINE_CRGBFLOATCOLOUR_H
// CRGBFloatColour (chunk 723b): Ego_r.pdb class (V-keyed, 16 bytes: float R +0, G +4, B +8, A +0xC). Constructors from the Ego_r.pdb
// name set: ??0CRGBFloatColour@@QAE@XZ (empty), ??0CRGBFloatColour@@QAE@MMMM@Z; operator*= (??XCRGBFloatColour@@QAEAAV0@ABV0@@Z).
// The component-wise product the water passes expand (0x00B708DF: each product loaded straight from both operands, no copy) is
// the inline free operator* (no out-of-line copy in either PDB). Also defines the inline C4DVector(const CRGBFloatColour&)
// (??0C4DVector@@QAE@ABVCRGBFloatColour@@@Z: X=R, Y=G, Z=B, W=A).
#include "rebuild_abi.h"
#ifndef FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#endif
#include "engine/C4DVector.h"
class CRGBFloatColour {
public:
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour& operator*=(const CRGBFloatColour& other) { R *= other.R; G *= other.G; B *= other.B; A *= other.A; return *this; }
    float R;                                                             // +0x00
    float G;                                                             // +0x04
    float B;                                                             // +0x08
    float A;                                                             // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CRGBFloatColour) == 0x10);
inline CRGBFloatColour operator*(const CRGBFloatColour& a, const CRGBFloatColour& b)
{
    return CRGBFloatColour(a.R * b.R, a.G * b.G, a.B * b.B, a.A * b.A);
}
// (C4DVector.h must have been read with FABLE_RETAIL_C4DVECTOR: the retail headers that include it define the macro first.)
inline C4DVector::C4DVector(const CRGBFloatColour& colour) : X(colour.R), Y(colour.G), Z(colour.B), W(colour.A) { }
#endif // FABLE_ENGINE_CRGBFLOATCOLOUR_H
