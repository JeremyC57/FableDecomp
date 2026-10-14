// CScriptRenderBinding_00c338f0::SetTargetDirection(CTrackedTransform_00c404a0* target, const C3DVector& position)
// -- forward a world vector to a tracked-transform object's second slot setter through a by-value
// conversion temporary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C338F0, length 45 bytes (+ int3 padding).
// Callee: CTrackedTransform_00c404a0::SetDirection(const CTrackedVector&) @ 0x00C404A0 (stores the
// three floats at +0x18 when they differ and sets the 0x10000 dirty bit at +0x48; the manifest's
// "CShaderRenderManager::SetCameraPos" label is propagated -- the PDB CShaderRenderManager holds
// a CArray at +0x0c). `this` is not used: a two-argument __fastcall whose body only touches its
// arguments (`ret 8`). The 12-byte temporary is built in the frame with three dword moves
// (through ecx/edx/eax) and passed by address. The sibling @ 0x00C338C0 forwards to the
// first setter @ 0x00C40450 (+0x0c, dirty bit 0x8000).
// The manifest label "CContainmentVolume::IsContainingPoint" is propagated and rejected (that
// shape is 0x00A75430); class and method names are descriptive (script module, no RTTI).
struct C3DVector {
    float X;
    float Y;
    float Z;
};

struct CTrackedVector {                        // the setter's argument: a position wrapper built from a C3DVector
    C3DVector Value;
    CTrackedVector(const C3DVector& value) : Value(value) { }
};

struct CTrackedTransform_00c404a0 {
    void SetDirection(const CTrackedVector& position);     // 0x00C404A0
};

struct CScriptRenderBinding_00c338f0 {
    void SetTargetDirection(CTrackedTransform_00c404a0* target, const C3DVector& position);
};

void CScriptRenderBinding_00c338f0::SetTargetDirection(CTrackedTransform_00c404a0* target, const C3DVector& position)
{
    target->SetDirection(CTrackedVector(position));
}