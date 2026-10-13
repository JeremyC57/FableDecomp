// CCreatureAction_NymphGetHit::CCreatureAction_NymphGetHit() -- base CCombatAction_GenericStrikeResponseBase() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F5B00, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CCombatAction_GenericStrikeResponseBase::CCombatAction_GenericStrikeResponseBase ; mov dword ptr [esi],offset CCreatureAction_NymphGetHit::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00867950 runs, then the compiler stores this class's vtable (0x01263F84
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
struct CCombatAction_GenericStrikeResponseBase {
    virtual void v00();
    CCombatAction_GenericStrikeResponseBase();                           // 0x00867950
};

struct CCreatureAction_NymphGetHit : CCombatAction_GenericStrikeResponseBase {
    virtual void v00();                    // this class has its own vtable
    CCreatureAction_NymphGetHit();
};

CCreatureAction_NymphGetHit::CCreatureAction_NymphGetHit()
    : CCombatAction_GenericStrikeResponseBase()
{
}
