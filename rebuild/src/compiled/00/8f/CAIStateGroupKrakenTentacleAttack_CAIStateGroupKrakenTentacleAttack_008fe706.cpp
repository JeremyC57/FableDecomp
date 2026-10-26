// CAIStateGroup_KrakenTentacleAttack::CAIStateGroup_KrakenTentacleAttack() -- base CAIStateGroupBase() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FE706, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CAIStateGroupBase::CAIStateGroupBase ; mov dword ptr [esi],offset CAIStateGroup_KrakenTentacleAttack::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x008FD255 runs, then the compiler stores this class's vtable (0x0128ABF4
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
class CAIStateGroupBase { public:
    virtual void v00();
    CAIStateGroupBase();                           // 0x008FD255
};

struct CAIStateGroup_KrakenTentacleAttack : CAIStateGroupBase {
    virtual void v00();                    // this class has its own vtable
    CAIStateGroup_KrakenTentacleAttack();
};

CAIStateGroup_KrakenTentacleAttack::CAIStateGroup_KrakenTentacleAttack()
    : CAIStateGroupBase()
{
}