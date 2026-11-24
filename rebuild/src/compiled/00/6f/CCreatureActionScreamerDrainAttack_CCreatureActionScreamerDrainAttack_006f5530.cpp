// CCreatureAction_ScreamerDrainAttack::CCreatureAction_ScreamerDrainAttack() -- base CCreatureAction_CastWillDrainLifeSpell() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F5530, oracle length 18 bytes.
// Name fix (NF380): the base at 0x008BBB10 is CCreatureAction_CastWillDrainLifeSpell (retail RTTI base list (ghidra_out/rtti_bases.tsv) and the FableWin (debug) caller agree; was C3DMeshFileXAnimatedPrimitiveChunk).
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CCreatureAction_CastWillDrainLifeSpell::CCreatureAction_CastWillDrainLifeSpell ; mov dword ptr [esi],offset CCreatureAction_ScreamerDrainAttack::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x008BBB10 runs, then the compiler stores this class's vtable (0x01261F7C
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
class CCreatureAction_CastWillDrainLifeSpell { public:
    virtual void v00();
    CCreatureAction_CastWillDrainLifeSpell();                           // 0x008BBB10
};

class CCreatureAction_ScreamerDrainAttack : public CCreatureAction_CastWillDrainLifeSpell { public:
    virtual void v00();                    // this class has its own vtable
    CCreatureAction_ScreamerDrainAttack();
};

CCreatureAction_ScreamerDrainAttack::CCreatureAction_ScreamerDrainAttack()
    : CCreatureAction_CastWillDrainLifeSpell()
{
}