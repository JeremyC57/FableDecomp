// CAIStateGroup_MinionBehaviourPodiumEmoteNeutral::CAIStateGroup_MinionBehaviourPodiumEmoteNeutral() -- base CAIStateGroup_CarryCrateAndFollow() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00902873, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CAIStateGroup_CarryCrateAndFollow::CAIStateGroup_CarryCrateAndFollow ; mov dword ptr [esi],offset CAIStateGroup_MinionBehaviourPodiumEmoteNeutral::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x009009EA runs, then the compiler stores this class's vtable (0x0128D16C
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
struct CAIStateGroup_CarryCrateAndFollow {
    virtual void v00();
    CAIStateGroup_CarryCrateAndFollow();                           // 0x009009EA
};

struct CAIStateGroup_MinionBehaviourPodiumEmoteNeutral : CAIStateGroup_CarryCrateAndFollow {
    virtual void v00();                    // this class has its own vtable
    CAIStateGroup_MinionBehaviourPodiumEmoteNeutral();
};

CAIStateGroup_MinionBehaviourPodiumEmoteNeutral::CAIStateGroup_MinionBehaviourPodiumEmoteNeutral()
    : CAIStateGroup_CarryCrateAndFollow()
{
}
