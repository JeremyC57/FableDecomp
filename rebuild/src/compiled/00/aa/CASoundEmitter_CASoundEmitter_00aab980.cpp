// CASoundEmitter::CASoundEmitter() -- base CBase() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAB980, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase::CBase ; mov dword ptr [esi],offset CASoundEmitter::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x0099A2D0 runs, then the compiler stores this class's vtable (0x0129E684
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                           // 0x0099A2D0
}; } using N_0099a2d0::CBase;

struct CASoundEmitter : CBase {
    virtual void v00();                    // this class has its own vtable
    CASoundEmitter();
};

CASoundEmitter::CASoundEmitter()
    : CBase()
{
}