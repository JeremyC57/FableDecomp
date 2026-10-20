// Grade: DIFFER(302v306) -- frame placement: retail puts the fog matrix at +0 and the by-value parameter copy at +0x30 (W of the result then reuses the dead fog slot); the lane swaps the two 48-byte blocks, which reorders the loads and moves the W temporary. The product itself (only the first column computed, statement-by-statement term order) matches once the concatenation writes into an out-parameter matrix (a by-value return keeps all twelve elements). Tried: by-value result + C4DVector assignment (690v306), out-parameter product (349v306), direct component stores (this), result declared first.
// CEngineLightingManager::CalculateFogTransformMatrix(C4DVector* out, float fogStart, float fogEnd) -- the fog coordinate transform: the camera's no-scale view transform followed by the depth-to-fog matrix (z -> (z - start) / (end - start) in X), returned as that product's first column (the X row of the fog vertex-shader constant)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B451D0, length 304 bytes (`ret 0Ch`).
// Callees: none. Globals: the engine camera @ 0x01436EA0 (engine/CEngineCamera.h NoScaleViewTransform +0x114), 1.0f @ 0x0122DED8.
// Shape: the fog matrix is a zero-filled local with the two depth terms; it is passed BY VALUE to an inline matrix product (the
// `rep movsd` copy) whose elements are summed statement by statement (the second column term first, then the third, then the
// first, then the translation), and only the first column survives; the result vector is assigned whole (W through eax, X/Y/Z
// straight off the x87 stack).
struct C4DVector {
    float X, Y, Z, W;
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};
struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};

static __forceinline void Concatenate(CMatrix3x4& r, const CMatrix3x4& a, CMatrix3x4 b)   // a, then b
{
    r.E11 = a.E12 * b.E21; r.E11 += a.E13 * b.E31; r.E11 += a.E11 * b.E11;
    r.E12 = a.E12 * b.E22; r.E12 += a.E13 * b.E32; r.E12 += a.E11 * b.E12;
    r.E13 = a.E12 * b.E23; r.E13 += a.E13 * b.E33; r.E13 += a.E11 * b.E13;
    r.E21 = a.E22 * b.E21; r.E21 += a.E23 * b.E31; r.E21 += a.E21 * b.E11;
    r.E22 = a.E22 * b.E22; r.E22 += a.E23 * b.E32; r.E22 += a.E21 * b.E12;
    r.E23 = a.E22 * b.E23; r.E23 += a.E23 * b.E33; r.E23 += a.E21 * b.E13;
    r.E31 = a.E32 * b.E21; r.E31 += a.E33 * b.E31; r.E31 += a.E31 * b.E11;
    r.E32 = a.E32 * b.E22; r.E32 += a.E33 * b.E32; r.E32 += a.E31 * b.E12;
    r.E33 = a.E32 * b.E23; r.E33 += a.E33 * b.E33; r.E33 += a.E31 * b.E13;
    r.E41 = a.E42 * b.E21; r.E41 += a.E43 * b.E31; r.E41 += a.E41 * b.E11; r.E41 += b.E41;
    r.E42 = a.E42 * b.E22; r.E42 += a.E43 * b.E32; r.E42 += a.E41 * b.E12; r.E42 += b.E42;
    r.E43 = a.E42 * b.E23; r.E43 += a.E43 * b.E33; r.E43 += a.E41 * b.E13; r.E43 += b.E43;
}

struct CEngineCameraView {
    unsigned char _pad[0x114];
    CMatrix3x4    NoScaleViewTransform;            // +0x114
};
extern CEngineCameraView* g_pEngineCamera_01436ea0;   // 0x01436EA0

struct CEngineLightingManager {
    void CalculateFogTransformMatrix(C4DVector* out, float fogStart, float fogEnd);
};

void CEngineLightingManager::CalculateFogTransformMatrix(C4DVector* out, float fogStart, float fogEnd)
{
    float scale = 1.0f / (fogEnd - fogStart);
    CMatrix3x4 fog;
    fog.E11 = 0.0f; fog.E12 = 0.0f; fog.E13 = 0.0f;
    fog.E21 = 0.0f; fog.E22 = 0.0f; fog.E23 = 0.0f;
    fog.E31 = scale; fog.E32 = 0.0f; fog.E33 = 0.0f;
    fog.E41 = -(scale * fogStart); fog.E42 = 0.0f; fog.E43 = 0.0f;
    CMatrix3x4 result;
    Concatenate(result, g_pEngineCamera_01436ea0->NoScaleViewTransform, fog);
    out->X = result.E11;
    out->Y = result.E21;
    out->Z = result.E31;
    out->W = result.E41;
}
