// CCreatureAction_ScreamerDrainAttack::CCreatureAction_ScreamerDrainAttack() -- base C3DMeshFileXAnimatedPrimitiveChunk() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F5530, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call C3DMeshFileXAnimatedPrimitiveChunk::C3DMeshFileXAnimatedPrimitiveChunk ; mov dword ptr [esi],offset CCreatureAction_ScreamerDrainAttack::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x008BBB10 runs, then the compiler stores this class's vtable (0x01261F7C
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
struct C3DMeshFileXAnimatedPrimitiveChunk {
    virtual void v00();
    C3DMeshFileXAnimatedPrimitiveChunk();                           // 0x008BBB10
};

struct CCreatureAction_ScreamerDrainAttack : C3DMeshFileXAnimatedPrimitiveChunk {
    virtual void v00();                    // this class has its own vtable
    CCreatureAction_ScreamerDrainAttack();
};

CCreatureAction_ScreamerDrainAttack::CCreatureAction_ScreamerDrainAttack()
    : C3DMeshFileXAnimatedPrimitiveChunk()
{
}
