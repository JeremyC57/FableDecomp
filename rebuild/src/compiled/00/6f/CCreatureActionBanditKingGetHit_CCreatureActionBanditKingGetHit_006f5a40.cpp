// CCreatureAction_BanditKingGetHit::CCreatureAction_BanditKingGetHit() -- base CCombatAction_GenericStrikeResponseBase() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F5A40, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CCombatAction_GenericStrikeResponseBase::CCombatAction_GenericStrikeResponseBase ; mov dword ptr [esi],offset CCreatureAction_BanditKingGetHit::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00867950 runs, then the compiler stores this class's vtable (0x01263BAC
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
class CCombatAction_GenericStrikeResponseBase { public:
    virtual void v00();
    CCombatAction_GenericStrikeResponseBase();                           // 0x00867950
};

class CCreatureAction_BanditKingGetHit : public CCombatAction_GenericStrikeResponseBase { public:
    virtual void v00();                    // this class has its own vtable
    CCreatureAction_BanditKingGetHit();
};

CCreatureAction_BanditKingGetHit::CCreatureAction_BanditKingGetHit()
    : CCombatAction_GenericStrikeResponseBase()
{
}