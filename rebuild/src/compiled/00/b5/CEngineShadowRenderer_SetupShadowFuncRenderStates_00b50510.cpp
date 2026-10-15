// CEngineShadowRenderer::SetupShadowFuncRenderStates(long, CColorBufferDepthPackParameters*) -- forward to the matrix overload with an identity texture transform
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B50510, length 125 bytes.
// Callees: CEngineShadowRenderer::SetupShadowFuncRenderStates(long, CMatrix3x4*, CColorBufferDepthPackParameters*) @ 0x00B4FC40.
// Shape: a 0x30-byte CMatrix3x4 local (right/up/look axes + position, C3DVector rows) set to identity by an inline
// SetIdentity written as two CHAINED assignments: `a = b = ... = 0.0f` stores right to left, so the nine zero words go
// out highest address first (+0x2c .. +0x04), then `Right.X = Up.Y = Look.Z = 1.0f` stores +0x20, +0x10, +0x00.
// All of it is scheduled after the three argument pushes; `this` stays in ecx for the forwarded call (`ret 8`).
// (An aggregate initialiser stores ascending, a memset becomes `rep stosd`, C3DVector row constructors copy through
// temporaries.) The engine header (engine/CEngineShadowRenderer.h) has no member this body touches, so `this` is a bare declaration.
struct C3DVector { float X, Y, Z; };

struct CMatrix3x4 {
    C3DVector Right, Up, Look, Pos;
    void SetIdentity()
    {
        Right.Y = Right.Z = Up.X = Up.Z = Look.X = Look.Y = Pos.X = Pos.Y = Pos.Z = 0.0f;
        Right.X = Up.Y = Look.Z = 1.0f;
    }
};

struct CColorBufferDepthPackParameters;

struct CEngineShadowRenderer {
    void SetupShadowFuncRenderStates(long stage, CMatrix3x4* textureTransform, CColorBufferDepthPackParameters* pack);   // 0x00B4FC40
    void SetupShadowFuncRenderStates(long stage, CColorBufferDepthPackParameters* pack);
};

void CEngineShadowRenderer::SetupShadowFuncRenderStates(long stage, CColorBufferDepthPackParameters* pack)
{
    CMatrix3x4 identity;
    identity.SetIdentity();
    SetupShadowFuncRenderStates(stage, &identity, pack);
}