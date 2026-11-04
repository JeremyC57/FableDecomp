// C4DVector::C4DVector(const CRGBColour&) -- build a 4-float vector from a BGRA byte colour, each channel times 1/255 (X=R, Y=G, Z=B, W=A)
// Leaf: ??0C4DVector@@QAE@ABVCRGBColour@@@Z
// Flags: none
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_C4DVECTOR_RGBCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#include "engine/CRGBFloatColour.h"
C4DVector::C4DVector(const CRGBColour& colour)
{
    CRGBFloatColour c(colour);
    X = c.R; Y = c.G; Z = c.B; W = c.A;
}