// CVertexShaderInputAnimated::CVertexShaderInputAnimated() -- base CVertexShaderInput() then this class's vtable
// Leaf: ??0CVertexShaderInputAnimated@
// Name fix (NF380): the base at 0x00AB3B10 is CVertexShaderInput (retail RTTI base list (ghidra_out/rtti_bases.tsv) and the FableWin (debug) caller agree; was CAIStateGroup_HoverHornetQueen, whose own ctor is 0x0090310E).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A986B0, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CVertexShaderInput::CVertexShaderInput ; mov dword ptr [esi],offset CVertexShaderInputAnimated::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00AB3B10 runs, then the compiler stores this class's vtable (0x0129DFB0
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
struct CVertexShaderInput {
    virtual void v00();
    CVertexShaderInput();                           // 0x00AB3B10
};

struct CVertexShaderInputAnimated : CVertexShaderInput {
    virtual void v00();                    // this class has its own vtable
    CVertexShaderInputAnimated();
};

CVertexShaderInputAnimated::CVertexShaderInputAnimated()
    : CVertexShaderInput()
{
}