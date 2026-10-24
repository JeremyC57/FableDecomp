// CEngineSurfaceCompositionManager::CalcColourDifference(const CRGBColour& a, const CRGBColour& b) -- Euclidean distance between two 8-bit RGBA colours (all four channels)
// Leaf: CalcColourDifference
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2AF50, length 115 bytes, `ret 8` (`this` unused), the float in st(0).
// Signature from Ego_r.pdb: ?CalcColourDifference@CEngineSurfaceCompositionManager@@AAEMABVCRGBColour@@0@Z (private).
//   Callers: CEngineSurfaceCompositionManager::IsNearlyIdentical @ 0x00B2B050 (two calls per blend, the manager in ecx).
// Callees: none (the square root is the inline fsqrt intrinsic).
// Layout: CRGBColour is the D3DCOLOR byte order B +0, G +1, R +2, A +3 (Ego_r.pdb CRGBColour union).
// Shape: four int channel differences (movzx/sub) converted with `fild` through the dead argument slots in R, G, B, A
//   order; VC sums the squares from the top of the x87 stack (A, B, G, R) and takes one fsqrt.
// Label: the catalog's `$E2` is a propagated initialiser name -- this is CalcColourDifference.
#include <math.h>
#include "engine/CEngineSurfaceCompositionManager.h"

struct CRGBColour {                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
};

struct CEngineSurfaceCompositionManager_Methods : CEngineSurfaceCompositionManager {
    float CalcColourDifference(const CRGBColour& a, const CRGBColour& b);
};

float CEngineSurfaceCompositionManager_Methods::CalcColourDifference(const CRGBColour& a, const CRGBColour& b)
{
    float r = (float)(a.R - b.R);
    float g = (float)(a.G - b.G);
    float bl = (float)(a.B - b.B);
    float al = (float)(a.A - b.A);
    return (float)sqrt(r * r + g * g + bl * bl + al * al);
}