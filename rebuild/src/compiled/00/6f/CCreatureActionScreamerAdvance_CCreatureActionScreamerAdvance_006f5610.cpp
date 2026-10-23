// CCreatureAction_ScreamerAdvance::CCreatureAction_ScreamerAdvance() -- base C3DAnimFileHelpersChunk() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F5610, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call C3DAnimFileHelpersChunk::C3DAnimFileHelpersChunk ; mov dword ptr [esi],offset CCreatureAction_ScreamerAdvance::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x008BFCE0 runs, then the compiler stores this class's vtable (0x01262354
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
namespace N_008bfce0 { struct C3DAnimFileHelpersChunk {
    virtual void v00();
    C3DAnimFileHelpersChunk();                           // 0x008BFCE0
}; } using N_008bfce0::C3DAnimFileHelpersChunk;

struct CCreatureAction_ScreamerAdvance : C3DAnimFileHelpersChunk {
    virtual void v00();                    // this class has its own vtable
    CCreatureAction_ScreamerAdvance();
};

CCreatureAction_ScreamerAdvance::CCreatureAction_ScreamerAdvance()
    : C3DAnimFileHelpersChunk()
{
}