// CAIStateGroup_CreatureAdvanceInCover::CAIStateGroup_CreatureAdvanceInCover() -- base CAIStateGroup_CreatureUseCoverBase() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090313F, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CAIStateGroup_CreatureUseCoverBase::CAIStateGroup_CreatureUseCoverBase ; mov dword ptr [esi],offset CAIStateGroup_CreatureAdvanceInCover::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00902FB9 runs, then the compiler stores this class's vtable (0x0128D4E4
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
struct CAIStateGroup_CreatureUseCoverBase {
    virtual void v00();
    CAIStateGroup_CreatureUseCoverBase();                           // 0x00902FB9
};

struct CAIStateGroup_CreatureAdvanceInCover : CAIStateGroup_CreatureUseCoverBase {
    virtual void v00();                    // this class has its own vtable
    CAIStateGroup_CreatureAdvanceInCover();
};

CAIStateGroup_CreatureAdvanceInCover::CAIStateGroup_CreatureAdvanceInCover()
    : CAIStateGroup_CreatureUseCoverBase()
{
}