// CAIStateGroup_RockTroll::CAIStateGroup_RockTroll() -- base CAIStateGroupBehaviourBase() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009025EE, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CAIStateGroupBehaviourBase::CAIStateGroupBehaviourBase ; mov dword ptr [esi],offset CAIStateGroup_RockTroll::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x0090035D runs, then the compiler stores this class's vtable (0x0128D0A0
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
namespace N_0090035d { struct CAIStateGroupBehaviourBase {
    virtual void v00();
    CAIStateGroupBehaviourBase();                           // 0x0090035D
}; } using N_0090035d::CAIStateGroupBehaviourBase;

struct CAIStateGroup_RockTroll : CAIStateGroupBehaviourBase {
    virtual void v00();                    // this class has its own vtable
    CAIStateGroup_RockTroll();
};

CAIStateGroup_RockTroll::CAIStateGroup_RockTroll()
    : CAIStateGroupBehaviourBase()
{
}