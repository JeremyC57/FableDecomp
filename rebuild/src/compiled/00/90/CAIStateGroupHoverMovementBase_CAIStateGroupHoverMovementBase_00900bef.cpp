// CAIStateGroup_HoverMovementBase::CAIStateGroup_HoverMovementBase() -- base CAIStateGroupBehaviourBase() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00900BEF, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CAIStateGroupBehaviourBase::CAIStateGroupBehaviourBase ; mov dword ptr [esi],offset CAIStateGroup_HoverMovementBase::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x008FF720 runs, then the compiler stores this class's vtable (0x0128A1DC
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
namespace N_008ff720 { struct CAIStateGroupBehaviourBase {
    virtual void v00();
    CAIStateGroupBehaviourBase();                           // 0x008FF720
}; } using N_008ff720::CAIStateGroupBehaviourBase;

struct CAIStateGroup_HoverMovementBase : CAIStateGroupBehaviourBase {
    virtual void v00();                    // this class has its own vtable
    CAIStateGroup_HoverMovementBase();
};

CAIStateGroup_HoverMovementBase::CAIStateGroup_HoverMovementBase()
    : CAIStateGroupBehaviourBase()
{
}