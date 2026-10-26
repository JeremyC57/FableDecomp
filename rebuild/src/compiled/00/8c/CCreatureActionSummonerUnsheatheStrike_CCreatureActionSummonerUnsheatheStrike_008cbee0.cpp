// CCreatureAction_SummonerUnsheatheStrike::CCreatureAction_SummonerUnsheatheStrike() -- base CCreatureAction_UnsheatheWeapons() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CBEE0, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CCreatureAction_UnsheatheWeapons::CCreatureAction_UnsheatheWeapons ; mov dword ptr [esi],offset CCreatureAction_SummonerUnsheatheStrike::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x008B8560 runs, then the compiler stores this class's vtable (0x012834C4
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
class CCreatureAction_UnsheatheWeapons { public:
    virtual void v00();
    CCreatureAction_UnsheatheWeapons();                           // 0x008B8560
};

class CCreatureAction_SummonerUnsheatheStrike : public CCreatureAction_UnsheatheWeapons { public:
    virtual void v00();                    // this class has its own vtable
    CCreatureAction_SummonerUnsheatheStrike();
};

CCreatureAction_SummonerUnsheatheStrike::CCreatureAction_SummonerUnsheatheStrike()
    : CCreatureAction_UnsheatheWeapons()
{
}