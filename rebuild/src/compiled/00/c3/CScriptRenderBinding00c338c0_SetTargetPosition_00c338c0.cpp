// CScriptRenderBinding_00c338c0::SetTargetPosition(CTrackedTransform_00c40450* target, const C3DVector& position)
// -- forward a world position to a tracked-transform object's position setter through a by-value
// conversion temporary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C338C0, length 45 bytes (+ int3 padding).
// Callee: CTrackedTransform_00c40450::SetPosition(const CTrackedVector&) @ 0x00C40450 (stores the
// three floats at +0x0c when they differ and sets the 0x8000 dirty bit at +0x48; the manifest's
// "CShaderRenderManager::SetCameraPos" label is propagated -- the PDB CShaderRenderManager holds
// a CArray at +0x0c). `this` is not used: a two-argument __fastcall whose body only touches its
// arguments (`ret 8`). The 12-byte temporary is built in the frame with three dword moves
// (through ecx/edx/eax) and passed by address. The sibling @ 0x00C338F0 forwards to the
// second setter @ 0x00C404A0 (+0x18, dirty bit 0x10000).
// The manifest label "CContainmentVolume::IsContainingPoint" is propagated and rejected (that
// shape is 0x00A75430); class and method names are descriptive (script module, no RTTI).
class C3DVector { public:
    float X;
    float Y;
    float Z;
};

struct CTrackedVector {                        // the setter's argument: a position wrapper built from a C3DVector
    C3DVector Value;
    CTrackedVector(const C3DVector& value) : Value(value) { }
};

struct CTrackedTransform_00c40450 {
    void SetPosition(const CTrackedVector& position);     // 0x00C40450
};

struct CScriptRenderBinding_00c338c0 {
    void SetTargetPosition(CTrackedTransform_00c40450* target, const C3DVector& position);
};

void CScriptRenderBinding_00c338c0::SetTargetPosition(CTrackedTransform_00c40450* target, const C3DVector& position)
{
    target->SetPosition(CTrackedVector(position));
}