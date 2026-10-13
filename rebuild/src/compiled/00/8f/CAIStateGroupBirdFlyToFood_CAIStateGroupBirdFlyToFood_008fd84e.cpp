// CAIStateGroup_BirdFlyToFood::CAIStateGroup_BirdFlyToFood() -- base CAIStateGroup_FlyingThingBase() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FD84E, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CAIStateGroup_FlyingThingBase::CAIStateGroup_FlyingThingBase ; mov dword ptr [esi],offset CAIStateGroup_BirdFlyToFood::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x009126C0 runs, then the compiler stores this class's vtable (0x0128961C
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
struct CAIStateGroup_FlyingThingBase {
    virtual void v00();
    CAIStateGroup_FlyingThingBase();                           // 0x009126C0
};

struct CAIStateGroup_BirdFlyToFood : CAIStateGroup_FlyingThingBase {
    virtual void v00();                    // this class has its own vtable
    CAIStateGroup_BirdFlyToFood();
};

CAIStateGroup_BirdFlyToFood::CAIStateGroup_BirdFlyToFood()
    : CAIStateGroup_FlyingThingBase()
{
}