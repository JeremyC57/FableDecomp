// CAIStateGroup_MinionSleep::CAIStateGroup_MinionSleep() -- base CAIStateGroup_GoToBed() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FEBB3, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CAIStateGroup_GoToBed::CAIStateGroup_GoToBed ; mov dword ptr [esi],offset CAIStateGroup_MinionSleep::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x008FEB6B runs, then the compiler stores this class's vtable (0x0128B140
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
struct CAIStateGroup_GoToBed {
    virtual void v00();
    CAIStateGroup_GoToBed();                           // 0x008FEB6B
};

struct CAIStateGroup_MinionSleep : CAIStateGroup_GoToBed {
    virtual void v00();                    // this class has its own vtable
    CAIStateGroup_MinionSleep();
};

CAIStateGroup_MinionSleep::CAIStateGroup_MinionSleep()
    : CAIStateGroup_GoToBed()
{
}