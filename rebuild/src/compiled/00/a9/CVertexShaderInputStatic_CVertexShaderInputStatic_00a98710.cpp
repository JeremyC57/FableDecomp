// N3DPrimitive::CVertexShaderInputStatic::CVertexShaderInputStatic(unsigned long, bool) -- base CVertexShaderInput(), own vtable, then Initialise(flags, flag)
// Leaf: ??0CVertexShaderInputStatic@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A98710, length 35 bytes.
// Callees: CVertexShaderInput::CVertexShaderInput @ 0x00AB3B10 (manifest label "CAIStateGroup_HoverHornetQueen"
// is propagated; the body stores vtable 0x0129EA08 = CVertexShaderInput per rtti-vtables.tsv),
// CVertexShaderInputStatic::Initialise(unsigned long, bool) @ 0x00A90560.
// Shape: the two arguments are re-read from their stack slots and pushed right to left after the
// base constructor; the vtable (0x0129DFB8, rtti-vtables.tsv) is stored between the pushes and the
// call. The manifest's "CCombatAbilityBlockDefBase" label is propagated from a byte-alike body.
struct CVertexShaderInput {
    virtual void v00();
    CVertexShaderInput();                                   // 0x00AB3B10
};

struct CVertexShaderInputStatic : CVertexShaderInput {
    virtual void v00();                                     // own vtable 0x0129DFB8
    CVertexShaderInputStatic(unsigned long flags, bool dynamic);
    void Initialise(unsigned long flags, bool dynamic);    // 0x00A90560
};

CVertexShaderInputStatic::CVertexShaderInputStatic(unsigned long flags, bool dynamic)
    : CVertexShaderInput()
{
    Initialise(flags, dynamic);
}