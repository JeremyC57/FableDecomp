// N3DPrimitive::CVertexShaderInputAnimated::CVertexShaderInputAnimated(unsigned long, bool) -- base CVertexShaderInput(), own vtable, then Initialise(flags, flag)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A98680, length 35 bytes.
// Callees: CVertexShaderInput::CVertexShaderInput @ 0x00AB3B10 (manifest label "CAIStateGroup_HoverHornetQueen"
// is propagated; the body stores vtable 0x0129EA08 = CVertexShaderInput per rtti-vtables.tsv),
// CVertexShaderInputAnimated::Initialise(unsigned long, bool) @ 0x00A90630.
// Shape: the two arguments are re-read from their stack slots and pushed right to left after the
// base constructor; the vtable (0x0129DFB0, rtti-vtables.tsv) is stored between the pushes and the
// call. The manifest's "CCombatAbilityBlockDefBase" label is propagated from a byte-alike body.
struct CVertexShaderInput {
    virtual void v00();
    CVertexShaderInput();                                   // 0x00AB3B10
};

struct CVertexShaderInputAnimated : CVertexShaderInput {
    virtual void v00();                                     // own vtable 0x0129DFB0
    CVertexShaderInputAnimated(unsigned long flags, bool dynamic);
    void Initialise(unsigned long flags, bool dynamic);    // 0x00A90630
};

CVertexShaderInputAnimated::CVertexShaderInputAnimated(unsigned long flags, bool dynamic)
    : CVertexShaderInput()
{
    Initialise(flags, dynamic);
}