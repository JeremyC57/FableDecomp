// CVertexShaderInputStatic::CVertexShaderInputStatic() -- base CAIStateGroup_HoverHornetQueen() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A98740, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CAIStateGroup_HoverHornetQueen::CAIStateGroup_HoverHornetQueen ; mov dword ptr [esi],offset CVertexShaderInputStatic::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00AB3B10 runs, then the compiler stores this class's vtable (0x0129DFB8
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
struct CAIStateGroup_HoverHornetQueen {
    virtual void v00();
    CAIStateGroup_HoverHornetQueen();                           // 0x00AB3B10
};

struct CVertexShaderInputStatic : CAIStateGroup_HoverHornetQueen {
    virtual void v00();                    // this class has its own vtable
    CVertexShaderInputStatic();
};

CVertexShaderInputStatic::CVertexShaderInputStatic()
    : CAIStateGroup_HoverHornetQueen()
{
}
