#ifndef FABLE_ENGINE_CRGBFLOATCOLOUR_H
#define FABLE_ENGINE_CRGBFLOATCOLOUR_H
// CRGBFloatColour: one retail model (merged 2026-11-03 from chunk 723b/723c and Codex pass 13). Ego_r.pdb class (V-keyed,
// 16 bytes: float R +0, G +4, B +8, A +0xC; CShaderRenderManager::AmbientColour +0x1AC / FogColour +0x1BC). Constructors from the
// Ego_r.pdb name set: ??0CRGBFloatColour@@QAE@XZ (empty), ??0CRGBFloatColour@@QAE@MMMM@Z, ??0CRGBFloatColour@@QAE@ABVCRGBColour@@@Z;
// ?Initialise@CRGBFloatColour@@QAEXMMMM@Z (CShaderRenderManager::Initialise 0x0098ACF0 stores four immediates in member order);
// operator*= (??XCRGBFloatColour@@QAEAAV0@ABV0@@Z). None has an out-of-line copy in retail (reloc_bindings): all inline.
// The component-wise product the water passes expand (0x00B708DF: each product loaded straight from both operands, no copy) is
// the inline free operator* (no out-of-line copy in either PDB). Also defines the inline C4DVector(const CRGBFloatColour&)
// (??0C4DVector@@QAE@ABVCRGBFloatColour@@@Z: X=R, Y=G, Z=B, W=A), so C4DVector.h is read with FABLE_RETAIL_C4DVECTOR.
#include "rebuild_abi.h"
#ifndef FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#endif
#include "engine/C4DVector.h"
#include "engine/CRGBColour.h"
#ifdef FABLE_RETAIL_CRGBFLOATCOLOUR
#include "engine/GFFloatToLongNear.h"
#endif
class CRGBFloatColour {
public:
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    // ??0CRGBFloatColour@@QAE@ABVCRGBColour@@@Z (inline; chunk 723c, RenderSingleSprite 0x00BC04F0): each byte channel times
    // 1/255 (the float pool 0x01231724), R, G, B, A in member order.
    __forceinline CRGBFloatColour(const CRGBColour& colour)
        : R(colour.R * (1.0f / 255.0f)), G(colour.G * (1.0f / 255.0f)), B(colour.B * (1.0f / 255.0f)), A(colour.A * (1.0f / 255.0f)) { }
    void Initialise(float r, float g, float b, float a) { R = r; G = g; B = b; A = a; }
    CRGBFloatColour& operator*=(const CRGBFloatColour& other) { R *= other.R; G *= other.G; B *= other.B; A *= other.A; return *this; }
#ifdef FABLE_RETAIL_CRGBFLOATCOLOUR
    // ?GetRGBColour@CRGBFloatColour@@QBE?AVCRGBColour@@XZ (FableWin.pdb; out-of-line COMDAT 0x00429880, landed): inline here so
    // a caller that passes `colour.GetRGBColour()` by value expands it in its argument list (CTCInventoryExperience::
    // AddAbilityToList 0x00570332: the four __ftol2 calls on the returned colour's pointer, A first, after the later arguments).
    // __forceinline, not plain inline: retail expands it in that /Os TU (calls 0x00570785..0x005707AB to __ftol2 0x00BFEA70, no
    // call to 0x00429880) while the out-of-line COMDAT exists; plain in-class inline is called out of line under /Os.
    __forceinline CRGBColour GetRGBColour() const
    {
        return CRGBColour((unsigned char)(R * 255.0f), (unsigned char)(G * 255.0f), (unsigned char)(B * 255.0f),
                          (unsigned char)(A * 255.0f));
    }
    // Chunk 737 (CPSCLight::PreparePrimitives 0x00B19C60, CPSCSingleSprite::PreparePrimitives 0x00B087C0): FableWin.pdb
    // ??HCRGBFloatColour@@QBE?AV0@ABV0@@Z / ??GCRGBFloatColour@@QBE?AV0@ABV0@@Z / ??DCRGBFloatColour@@QBE?AV0@M@Z (no retail copy),
    // ?Interpolate@CRGBFloatColour@@QBE?AV1@ABV1@M@Z (out-of-line copy 0x00B09B90, landed; expanded in both rows) and
    // ?GetRGBColourClamped@CRGBFloatColour@@QBE?AVCRGBColour@@XZ (out-of-line copy 0x00B09AA0, landed; expanded in both rows), with
    // the landed bodies.
    CRGBFloatColour operator+(const CRGBFloatColour& c) const { return CRGBFloatColour(R + c.R, G + c.G, B + c.B, A + c.A); }
    CRGBFloatColour operator-(const CRGBFloatColour& c) const { return CRGBFloatColour(R - c.R, G - c.G, B - c.B, A - c.A); }
    CRGBFloatColour operator*(float f) const { return CRGBFloatColour(R * f, G * f, B * f, A * f); }
    __forceinline CRGBFloatColour Interpolate(const CRGBFloatColour& colour, float t) const { return *this * (1.0f - t) + colour * t; }
    __forceinline CRGBColour GetRGBColourClamped() const
    {
        long r = GFFloatToLongNear(R * 255.0f);
        long g = GFFloatToLongNear(G * 255.0f);
        long b = GFFloatToLongNear(B * 255.0f);
        long a = GFFloatToLongNear(A * 255.0f);
        if (r < 0) r = 0; else if (r > 255) r = 255;
        if (g < 0) g = 0; else if (g > 255) g = 255;
        if (b < 0) b = 0; else if (b > 255) b = 255;
        if (a < 0) a = 0; else if (a > 255) a = 255;
        return CRGBColour((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a);
    }
#endif
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
